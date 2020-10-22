// scrollbit.c
// Copyright (c) 2019 J. M. Spivey

/* This example displays scrolling text on the "scroll:bit", a
   micro:bit add-on from Pimoroni.  Its matrix of 17 x 7 white LEDs
   has an I2C-based driver chip */

#include "microbian.h"
#include "hardware.h"
#include "lib.h"
#include "font.h"
#include <string.h>

void i2c_write_buf(int addr, int cmd, byte *buf2, int n) {
     byte buf1 = cmd;
     int status = i2c_xfer(WRITE, addr, &buf1, 1, buf2, n);
     assert(status == OK);
}

#define CHARWD 6
#define CHARHT 7
#define DISPWD 17

/* map -- layout of the LED board */
static const byte map[CHARHT][DISPWD] = {
{ 134, 118, 102, 86, 70, 54, 38, 22, 6,  8, 24, 40, 56, 72, 88, 104, 120 }, 
{ 133, 117, 101, 85, 69, 53, 37, 21, 5,  9, 25, 41, 57, 73, 89, 105, 121 },
{ 132, 116, 100, 84, 68, 52, 36, 20, 4, 10, 26, 42, 58, 74, 90, 106, 122 },
{ 131, 115,  99, 83, 67, 51, 35, 19, 3, 11, 27, 43, 59, 75, 91, 107, 123 },
{ 130, 114,  98, 82, 66, 50, 34, 18, 2, 12, 28, 44, 60, 76, 92, 108, 124 },
{ 129, 113,  97, 81, 65, 49, 33, 17, 1, 13, 29, 45, 61, 77, 93, 109, 125 },
{ 128, 112,  96, 80, 64, 48, 32, 16, 0, 14, 30, 46, 62, 78, 94, 110, 126 }
};

#define BRIGHTNESS 64

#define I2C_ADDR 0x74
#define REG_MODE 0x0
#define REG_FRAME 0x1
#define REG_AUDIOSYNC 0x6
#define REG_SHUTDOWN 0xa

#define REG_ENABLE 0x00
#define REG_BLINK 0x12
#define REG_COLOR 0x24

#define REG_BANK 0xfd
#define BANK_CONFIG 0xb

/* imgbuf -- brightness data for each LED */
static byte imgbuf[144];

static void show(void) {
     static int frame = 0;
     i2c_write_reg(I2C_ADDR, REG_BANK, frame);
     i2c_write_buf(I2C_ADDR, REG_COLOR, imgbuf, 144);
     i2c_write_reg(I2C_ADDR, REG_BANK, BANK_CONFIG);
     i2c_write_reg(I2C_ADDR, REG_FRAME, frame);
     frame = 1-frame;
}

static void init_display(void) {
     i2c_write_reg(I2C_ADDR, REG_BANK, BANK_CONFIG);
     timer_delay(1);
     i2c_write_reg(I2C_ADDR, REG_SHUTDOWN, 0);
     timer_delay(1);
     i2c_write_reg(I2C_ADDR, REG_SHUTDOWN, 1);
     timer_delay(1);
     i2c_write_reg(I2C_ADDR, REG_MODE, 0);
     i2c_write_reg(I2C_ADDR, REG_AUDIOSYNC, 0);

     // Enable each LED: unwired LEDs must be disabled for Charlieplexing.
     memset(imgbuf, 0x7f, 17);  // LEDs 0-6, 8-14, ..., 128-134 on
     imgbuf[17] = 0;            // LEDs 136-143 off
     i2c_write_reg(I2C_ADDR, REG_BANK, 0);
     i2c_write_buf(I2C_ADDR, REG_ENABLE, imgbuf, 18);
     i2c_write_reg(I2C_ADDR, REG_BANK, 1);
     i2c_write_buf(I2C_ADDR, REG_ENABLE, imgbuf, 18);

     // Disable blinking for each LED and blank the display
     memset(imgbuf, 0, 144);
     i2c_write_reg(I2C_ADDR, REG_BANK, 0);
     i2c_write_buf(I2C_ADDR, REG_BLINK, imgbuf, 18);
     i2c_write_reg(I2C_ADDR, REG_BANK, 1);
     i2c_write_buf(I2C_ADDR, REG_BLINK, imgbuf, 18);
     show();
}

#define DISPMAX 1024

static byte display[DISPMAX];

static void frame(int x) {
    memset(imgbuf, 0, 144);

    for (int i = 0; i < DISPWD; i++) {
        byte col = display[x+i];
        for (int j = 0; j < CHARHT; j++)
            imgbuf[map[j][i]] =
                (col & BIT(j) ? BRIGHTNESS : 0);
    }

    show();
}

static int render(char *text) {
    int i = DISPWD;
    memset(display, 0, DISPWD);
    for (int j = 0; text[j] != '\0'; j++) {
        const struct glyph *g = &font[(int) text[j]];
        for (int k = 0; k < g->width; k++)
            display[i++] = g->column[k];
    }
    memset(&display[i], 0, DISPWD);
    return i;
}

int SCROLL, STORE, INPUT;

void scroll_task(int dummy) {
    int w;
    char text[80];
    message m;

    init_display();

    while (1) {
        m.m_p1 = text;
        sendrec(STORE, READ, &m);

        w = render(text);
        for (int x = 0; x <= w; x++) {
            frame(x);
            timer_delay(200);
        }
    }
}

void store_task(int dummy) {
    int client;
    char text[80];
    message m;

    // Initial value
    strcpy(text, "...");

    while (1) {
        receive(ANY, &m);
        client = m.m_sender;
        switch (m.m_type) {
        case READ:
            strcpy(m.m_p1, text);
            break;
            
        case WRITE:
            strcpy(text, m.m_p1);
            break;

        default:
            badmesg(m.m_type);
        }

        send(client, REPLY, NULL);
    }
}

void input_task(int dummy) {
    int n;
    char ch;
    char buf[80];
    message m;

    while (1) {
        printf("> ");
        n = 0;
        while (1) {
            ch = serial_getc();
            if (ch == '\n') break;
            buf[n++] = ch;
        }
        buf[n++] = '\0';

        if (strlen(buf) > 0) {
            m.m_p1 = buf;
            sendrec(STORE, WRITE, &m);
        }
     }
}

void init(void) {
     timer_init();
     i2c_init();
     serial_init();
     SCROLL = start("Scroll", scroll_task, 0, STACK);
     STORE = start("Store", store_task, 0, STACK);
     INPUT = start("Input", input_task, 0, STACK);
}
