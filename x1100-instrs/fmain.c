// common/fmain.c
// Copyright (c) J. M. Spivey 2018-19

#include "hardware.h"
#include "lib.h"
#include <stdarg.h>

/* Pins to use for serial communication */
#define TX USB_TX
#define RX USB_RX

int txinit;              // UART ready to transmit first char

/* serial_init -- set up UART connection to host */
void serial_init(void) {
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
    txinit = 1;
}

/* serial_getc -- wait for input character and return it */
int serial_getc(void) {
    while (! UART_RXDRDY) { }
    char ch = UART_RXD;
    UART_RXDRDY = 0;
    return ch;
}

/* serial_putc -- send output character */
void serial_putc(char ch) {
    if (! txinit) {
        while (! UART_TXDRDY) { }
    }
    txinit = 0;
    UART_TXDRDY = 0;
    UART_TXD = ch;
}

/* serial_puts -- send a string character by character */
void serial_puts(const char *s) {
    while (*s != '\0')
        serial_putc(*s++);
}

/* serial_getline -- input a line of text into buf with line editing */
void serial_getline(const char *prompt, char *buf, int nbuf) {
    char *p = buf;

    serial_puts(prompt);

    while (1) {
        char x = serial_getc();

        switch (x) {
        case '\b':
        case 0177:
            if (p > buf) {
                p--;
                serial_puts("\b \b");
            }
            break;

        case '\r':
            *p = '\0';
            serial_puts("\r\n");
            return;

        default:
            /* Ignore other non-printing characters */
            if (x >= 040 && x < 0177 && p < &buf[nbuf]) {
                *p++ = x;
                serial_putc(x);
            }
        }
    }
}

/* putchar -- character output for use by printf */
void putchar(char c) {
    if (c == '\n') serial_putc('\r');
    serial_putc(c);
}

#define NBUF 80

/* getnum -- input a decimal or hexadecimal number */
int getnum(char *prompt) {
    char buf[NBUF];
    serial_getline(prompt, buf, NBUF);
    if (buf[0] == '0' && (buf[1] == 'x' || buf[1] == 'X'))
        return xtou(&buf[2]);
    else
        return atoi(buf);
}

/* fmt_fixed -- format n*t/10^k as a decimal */
char *fmt_fixed(unsigned t, unsigned n, int k) {
    static char buf[32];
    char *p = &buf[32], *p0;
    int d = 0;
    unsigned q = 1, v, x;
    for (int i = 0; i < k; i++) q *= 10;
    v = n * (t % q);
    *(--p) = '\0';
    p0 = p;
    while (d < k) {
        x = v % 10;
        if (x > 0 || p < p0) *(--p) = x + '0';
        v /= 10; d++;
    }
    v += n * (t / q);
    *(--p) = '.';
    do {
        *(--p) = v % 10 + '0';
        v /= 10;
    } while (v > 0);
    return p;
}

#define ROW_MASK 0x0000e000
#define COL_MASK 0x00001ff0

#define ROW0 13
#define COL0 4

#define R 1
#define C 2

#define LIGHT (1<<(ROW0+R)) | (~(1<<(COL0+C)) & COL_MASK)

#define FUDGE 20                // cycles of overhead for function call

extern int func(int a, int b);

/* init -- main program */
void init(void) {
    unsigned time;

    serial_init();
    printf("\nHello micro:world!\n\n");
    GPIO_DIRSET = ROW_MASK | COL_MASK;

    // Set up TIMER0 in 32 bit mode
    TIMER0_MODE = TIMER_MODE_Timer;
    TIMER0_BITMODE = TIMER_BITMODE_32Bit;
    TIMER0_PRESCALER = 0; // Count at 16MHz
    TIMER0_START = 1;

    while (1) {
        int a, b, c;
        a = getnum("a = ");
        b = getnum("b = ");

        TIMER0_CLEAR = 1;
        GPIO_OUT = LIGHT;
        c = func(a, b);
        GPIO_OUT = 0;
        TIMER0_CAPTURE[0] = 1;

        printf("func(%d, %d) = %d\n", a, b, c);
        printf("func(%x, %x) = %x\n", a, b, c);

        time = TIMER0_CC[0] - FUDGE;
        printf("%d cycle%s, %s microsec\n\n",
               time, (time == 1 ? "" : "s"), fmt_fixed(time, 625, 4));
    }
}
