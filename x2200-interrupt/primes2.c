// x2200-interrupt/primes.c
// Copyright (c) 2018 J. M. Spivey

#include "hardware.h"
#include "lib.h"
#include <stdarg.h>

/* Pins to use for serial communication */
#define TX USB_TX
#define RX USB_RX

#define NBUF 64

static volatile int txidle;       // Whether UART is idle
static volatile int bufcnt = 0;   // Number of chars in buffer
static unsigned bufin = 0;        // Index of first free slot
static unsigned bufout = 0;       // Index of first occupied slot
static volatile char txbuf[NBUF]; // The buffer

/* serial_init -- set up UART connection to host */
void serial_init(void) {
    // When disabled, TX is output high, RX is input
    GPIO_DIRSET = BIT(TX);
    GPIO_DIRCLR = BIT(RX);
    GPIO_OUTSET = BIT(TX);

    UART_ENABLE = UART_ENABLE_Disabled;
    UART_BAUDRATE = UART_BAUDRATE_9600; // 9600 baud
    UART_CONFIG = FIELD(UART_CONFIG_PARITY, UART_PARITY_None);
                                        // format 8N1
    UART_PSELTXD = TX;                  // choose pins
    UART_PSELRXD = RX;
    UART_ENABLE = UART_ENABLE_Enabled;
    UART_STARTTX = 1;
    UART_STARTRX = 1;
    UART_RXDRDY = 0;
    UART_TXDRDY = 0;

    // Interrupt for transmit only
    UART_INTENSET = BIT(UART_INT_TXDRDY);
    enable_irq(UART_IRQ);
    txidle = 1;
}

void uart_handler(void) {
    if (UART_TXDRDY) {
        UART_TXDRDY = 0;
        if (bufcnt == 0)
            txidle = 1;
        else {
            UART_TXD = txbuf[bufout];
            bufcnt--;
            bufout = (bufout+1) % NBUF;
        }
    }
}

/* serial_putc -- send output character */
void serial_putc(char ch) {
    while (bufcnt == NBUF) pause();

    intr_disable();
    if (txidle) {
        UART_TXD = ch;
        txidle = 0;
    } else {
        txbuf[bufin] = ch;
        bufcnt++;
        bufin = (bufin+1) % NBUF;
    }
    intr_enable();
}

/* putchar -- character output for use by printf */
void putchar(char c) {
    if (c == '\n') serial_putc('\r');
    serial_putc(c);
}

int modulo(int a, int b) {
    int r = a;
    while (r >= b) r -= b;
    return r;
}

int prime(int n) {
    for (int k = 2; k * k <= n; k++) {
        if (modulo(n, k) == 0)
            return 0;
    }

    return 1;
}

#define LEDS 0x0000fff0
#define LITE 0x00005fbf

void start_timer(void) {
    // Light an LED
    GPIO_OUT = LITE;

    // Start a timer
    TIMER0_MODE = TIMER_MODE_Timer;
    TIMER0_BITMODE = TIMER_BITMODE_32Bit;
    TIMER0_PRESCALER = 4; // Count at 1MHz
    TIMER0_START = 1;
}

void stop_timer(void) {
    // Turn LED off
    GPIO_OUT = 0;

    // Fetch timer result
    TIMER0_CAPTURE[0] = 1;
    unsigned time1 = TIMER0_CC[0];
    while (bufcnt > 0) pause();
    TIMER0_CAPTURE[0] = 1;
    unsigned time2 = TIMER0_CC[0];
    printf("%d+%d millisec\n",
           (time1+500)/1000, (time2-time1+500)/1000);
}

/* delay -- pause for n microseconds */
void delay(unsigned n) {
    unsigned t = n << 1;
    while (t > 0) {
        // 500nsec per iteration at 16MHz
        nop(); nop(); nop();
        t--;
    }
}

void init(void) {
    int n = 2, count = 0;

    GPIO_DIRSET = LEDS;
    serial_init();
    delay(10000);
    start_timer();

    while (count < 500) {
        if (prime(n)) {
            count++;
            printf("prime(%d) = %d\r\n", count, n);
        }
        n++;
    }
    
    stop_timer();
}