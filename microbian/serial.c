// serial.c
// Copyright (c) 2018 J. M. Spivey

#include "microbian.h"
#include "hardware.h"
#include <stdarg.h>

#define TX USB_TX
#define RX USB_RX

/* Message types for serial task */
#define PUTC 16
#define GETC 17

/* There are two buffers, one for characters waiting to be output, and
   another for input characters waiting to be read by other processes.
   The input buffer has |n_edit| characters in the current line, still
   subject to editing, and |n_avail| characters in previous lines that
   are available to other processes. */

/* NBUF -- size of input and output buffers.  Should be a power of 2. */
#define NBUF 128

/* wrap -- reduce index to range [0..NBUF) */
#define wrap(x) ((x) & (NBUF-1))

/* Input buffer */
static char rxbuf[NBUF];        /* Circular buffer for input */
static int rx_inp = 0;          /* In pointer */
static int rx_outp = 0;         /* Out pointer */
static int n_avail = 0;         /* Number of chars avail for input */
static int n_edit = 0;          /* Number of chars in current line */

/* Output buffer */
static char txbuf[NBUF];        /* Circular buffer for output */
static int tx_inp = 0;          /* In pointer */
static int tx_outp = 0;         /* Out pointer */
static int n_tx = 0;            /* Character count */

static int reader = -1;         /* Process waiting to read */

static int txidle = 1;          /* True if transmitter is idle */

/* echo -- echo input character */
static void echo(char ch) {
    if (n_tx == NBUF) return;
    txbuf[tx_inp] = ch;
    tx_inp = wrap(tx_inp+1);
    n_tx++;
}

#define CTRL(x) ((x) & 0x1f)

/* keypress -- deal with keyboard character by editing buffer */
static void keypress(char ch) {
    switch (ch) {
    case '\b':
    case 0177:
        /* Delete last character */
        if (n_edit > 0) {
            n_edit--;
            rx_inp = wrap(rx_inp-1);
            /* This doesn't work well with TAB and other control chars */
            echo('\b'); echo(' '); echo('\b');
        }
        break;

    case '\r':
    case '\n':
        /* Make line available to clients */
        if (n_avail + n_edit == NBUF) break;
        rxbuf[rx_inp] = '\n';
        rx_inp = wrap(rx_inp+1);
        n_edit++;
        n_avail += n_edit; n_edit = 0;
        echo('\r'); echo('\n');
        break;
        
    case CTRL('B'):
        /* Print process table dump */
        dump();
        break;

    default:
        /* Ignore other control characters */
        if (ch < 040 || ch >= 0177) break;

        /* Add character to line */
        if (n_avail + n_edit == NBUF) break;
        rxbuf[rx_inp] = ch;
        rx_inp = wrap(rx_inp+1);
        n_edit++;
        echo(ch);
    }
}

/* The clear_pending() call below is needed because the UART interrupt
   handler disables the IRQ for the UART in the NVIC, but doesn't
   disable the UART itself from sending interrupts.  The pending bit
   is cleared on return from the interrupt handler, but that doesn't
   stop the UART from setting it again. */

/* serial_interrupt -- handle serial interrupt */
static void serial_interrupt(void) {
    if (UART_RXDRDY) {
        char ch = UART_RXD;
        keypress(ch);
        UART_RXDRDY = 0;
    }

    if (UART_TXDRDY) {
        txidle = 1;
        UART_TXDRDY = 0;
    }

    clear_pending(UART_IRQ);
    enable_irq(UART_IRQ);
}

/* reply -- send reply or start transmitter if possible */
static void reply(void) {
    message m;
    
    // Can we satisfy a reader?
    if (reader >= 0 && n_avail > 0) {
        m.m_i1 = rxbuf[rx_outp];
        send(reader, REPLY, &m);
        rx_outp = wrap(rx_outp+1);
        n_avail--;
        reader = -1;
    }

    // Can we start transmitting a character?
    if (txidle && n_tx > 0) {
        UART_TXD = txbuf[tx_outp];
        tx_outp = wrap(tx_outp+1);
        n_tx--;
        txidle = 0;
    }
}

/* serial_task -- driver process for UART */
static void serial_task(int n) {
    message m;
    int client;
    char ch;

    UART_ENABLE = 0;

    GPIO_DIRSET = BIT(TX);
    GPIO_DIRCLR = BIT(RX);
    SET_FIELD(GPIO_PINCNF[TX], GPIO_PINCNF_PULL, GPIO_PULL_Pullup);
    SET_FIELD(GPIO_PINCNF[RX], GPIO_PINCNF_PULL, GPIO_PULL_Pullup);

    UART_BAUDRATE = UART_BAUDRATE_9600; // 9600 baud
    UART_CONFIG = 0;                    // format 8N1
    UART_PSELTXD = TX;                  // choose pins
    UART_PSELRXD = RX;
    UART_ENABLE = UART_ENABLE_Enabled;
    UART_STARTTX = 1;
    UART_STARTRX = 1;
    UART_RXDRDY = 0;
    UART_TXDRDY = 0;

    UART_INTENSET = BIT(UART_INT_RXDRDY) | BIT(UART_INT_TXDRDY);
    connect(UART_IRQ);

    txidle = 1;

    while (1) {
        receive(ANY, &m);
        client = m.m_sender;

        switch (m.m_type) {
        case INTERRUPT:
            serial_interrupt();
            break;

        case GETC:
            if (reader >= 0)
                panic("Two cannot wait for the input at once");
            reader = client;
            break;
            
        case PUTC:
            ch = m.m_i1;

            while (n_tx == NBUF) {
                // The buffer is full -- wait for a space to appear
                receive(INTERRUPT, NULL);
                serial_interrupt();
                reply();
            }

            txbuf[tx_inp] = ch;
            tx_inp = wrap(tx_inp+1);
            n_tx++;
            break;

        default:
            badmesg(m.m_type);
        }
          
        reply();
    }
}

static int SERIAL;

/* serial_init -- start the serial driver task */
void serial_init(void) {
    SERIAL = start("Serial", serial_task, 0, 256);
}

/* serial_putc -- queue a character for output */
void serial_putc(char ch) {
    message m;

    m.m_i1 = ch;
    send(SERIAL, PUTC, &m);
}

/* serial_getc -- request an input character */
char serial_getc(void) {
    message m;

    sendrec(SERIAL, GETC, &m);
    return m.m_i1;
}

/* putchar -- character output for use by printf */
void putchar(char c) {
    if (c == '\n') serial_putc('\r');
    serial_putc(c);
}