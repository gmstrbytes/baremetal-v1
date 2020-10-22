// level.c
// Copyright (c) 2019 J. M. Spivey

#include "microbian.h"
#include "hardware.h"
#include "lib.h"
#include "accel.h"

#define LED(i,j) ((0x1000 << i) | (~(0x8 << j) & 0x1ff0))

/* Table of GPIO settings for each LED */
static const int ledval[][5] = {
    { LED(3,3), LED(2,7), LED(3,1), LED(2,6), LED(3,2) },
    { LED(1,8), LED(1,7), LED(1,6), LED(1,5), LED(1,4) },
    { LED(2,2), LED(1,9), LED(2,3), LED(3,9), LED(2,1) },
    { LED(3,4), LED(3,5), LED(3,6), LED(3,7), LED(3,8) },
    { LED(1,1), LED(2,4), LED(1,2), LED(2,5), LED(1,3) }
};

/* scale -- map acceleration to coordinate */
static int scale(int x) {
     if (x < -20) return 4;
     if (x < -10) return 3;
     if (x <= 10) return 2;
     if (x <= 20) return 1;
     return 0;
}

/* main_task -- show the spirit level */
static void main(int n) {
    int x, y, z;

     printf("Hello\n\n");
     accel_start();
     GPIO_DIR = 0xfff0;

     while (1) {
          timer_delay(200);
          accel_reading(&x, &y, &z);
          printf("x=%d y=%d z=%d\n", x, y, z);
          x = scale(x); y = scale(y);
          GPIO_OUT = ledval[y][x];
     }
}

void init(void) {
     serial_init();
     timer_init();
     i2c_init();
     start("Main", main, 0, STACK);
}
