// x2000/heart.c
// Copyright (c) 2018 J. M. Spivey

#include "hardware.h"

/* delay -- pause for n microseconds */
void delay(unsigned n) {
    unsigned t = 2*n;
    while (t > 0) {
        // 500nsec per iteration at 16MHz
        nop(); nop(); nop();
        t--;
    }
}

/* heart -- GPIO values for heart image */
const unsigned heart[] = {
    0x28f0, 0x5e00, 0x8060
};

/* small -- GPIO values for small heart */
const unsigned small[] = {
    0x2df0, 0x5fb0, 0x8af0
};

#define JIFFY 5000              // Delay in microsecs

/* show -- display three rows of a picture n times */
void show(const unsigned *img, int n) {
    while (n-- > 0) {
        // Takes 15msec per iteration
        for (int p = 0; p < 3; p++) {
            GPIO_OUT = img[p];
            delay(JIFFY);
        }
    }
}

/* pressed -- test if a button is pressed */
int pressed(int button) {
    return (GPIO_IN & BIT(button)) == 0;
}

/* init -- main program */
void init(void) {
    GPIO_DIR = 0xfff0;
    GPIO_PINCNF[BUTTON_A] = 0;
    GPIO_PINCNF[BUTTON_B] = 0;

    // Set row pins to high-drive mode to increase brightness
    SET_FIELD(GPIO_PINCNF[13], GPIO_PINCNF_DRIVE, GPIO_DRIVE_S0H1);
    SET_FIELD(GPIO_PINCNF[14], GPIO_PINCNF_DRIVE, GPIO_DRIVE_S0H1);
    SET_FIELD(GPIO_PINCNF[15], GPIO_PINCNF_DRIVE, GPIO_DRIVE_S0H1);

    while (1) {
        show(heart, 70);
        show(small, 10);
        show(heart, 10);
        show(small, 10);
    }
}
