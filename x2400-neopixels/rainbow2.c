// rainbow2.c
// Copyright (c) 2019 J. M. Spivey

#include "hardware.h"

#define NPIX 12
#define STEP 2
#define NEO PAD0

/* delay -- pause for n microseconds */
void delay(unsigned n) {
    unsigned t = n << 1;
    while (t > 0) {
        // 500nsec per iteration at 16MHz
        nop(); nop(); nop();
        t--;
    }
}

void neoframe(unsigned pin, unsigned *buf, int n) {
    // With carefully adjusted delays, this gives exact timing for each
    // pixel, and a tolerable delay of 11 cycles between pixels

    for (int i = 0; i < n; i++) {
        unsigned pix = buf[i];
        for (int bit = 23; bit >= 0; bit--) {
            GPIO_OUTSET = BIT(pin);
            if (((pix >> bit) & 0x1) == 0) 
                GPIO_OUTCLR = BIT(pin);
            else {
                nop(); nop(); nop();
            }
            nop(); nop();
            GPIO_OUTCLR = BIT(pin);
            nop(); nop(); nop();
        }
    }
}

/* rgb -- assemble a colour from RGB components */
unsigned RGB(unsigned r, unsigned g, unsigned b) {
    return ((g & 0xff) << 16) | ((r & 0xff) << 8) | (b & 0xff);
}

/* getR, getG, get B -- access R, G, B components of colour */
unsigned getR(unsigned rgb) {
    return (rgb >> 8) & 0xff;
}

unsigned getG(unsigned rgb) {
    return (rgb >> 16) & 0xff;
}

unsigned getB(unsigned rgb) {
    return rgb & 0xff;
}

#define ISTEP 32

/* interp -- interpolate between two colours */
unsigned interp(int i, unsigned x, unsigned y) {
     int r = (ISTEP-i) * getR(x) + i * getR(y);
     int g = (ISTEP-i) * getG(x) + i * getG(y);
     int b = (ISTEP-i) * getB(x) + i * getB(y);
     return RGB(r>>5, g>>5, b>>5);
}

#define INTEN 31

/* hues 0 to 95 and cycle */
unsigned hue(int i) {
     int j = i & 0x1f;

     switch ((i >> 5) % 3) {
     case 0:
          return interp(j, RGB(INTEN, 0, 0), RGB(0, INTEN, 0));
     case 1:
          return interp(j, RGB(0, INTEN, 0), RGB(0, 0, INTEN));
     case 2:
          return interp(j, RGB(0, 0, INTEN), RGB(INTEN, 0, 0));
     default:
          return 0;
     }
}

unsigned pix[NPIX];

void init(void) {
     int u = 0;

     GPIO_OUT = 0;
     GPIO_DIRSET = BIT(NEO);
     neoframe(NEO, pix, NPIX);

     while (1) {
          for (int i = 0; i < NPIX; i++)
               pix[i] = hue(u + STEP * i);
          neoframe(NEO, pix, NPIX);
          u++;
          delay(100000);
     }
}
