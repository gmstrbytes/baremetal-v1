// x3300/accel.c
// Copyright (c) 2020 J. M. Spivey

#include "microbian.h"
#include "accel.h"

/* Accelerometer */

/* Different revisions of the micro:bit have different accelerometer
   chips: a MMA8653FC on the first revision, and a LSM303AGR on the
   second revision (also including the magnetometer in the same chip).
   Each chip has its own I2C address and its own internal
   structure. */

#define ACC1 0x1d               // I2C address of accelerometer
#define ACC1_CTRL_REG1 0x2a     // Control register
#define ACC1_X_DATA 0x01        // Acceleration data

#define ACC2 0x19               // I2C address of mk2 accelerometer
#define ACC2_CTRL_REG1 0x20     // Control register
#define ACC2_OUT 0x28           // Acceleration data (different format)

static int accel = 0;           // 1 or 2 to select a chip

/* acc1_read -- read acceleration data (v1) */
static void acc1_read(int *x, int *y, int *z) {
     signed char buf[3];
     i2c_read_bytes(ACC1, ACC1_X_DATA, (byte *) buf, 3);
     *x = -buf[0]; *y = buf[1]; *z = -buf[2];
}

/* acc2_read -- read acceleration data (v2) */
static void acc2_read(int *x, int *y, int *z) {
     signed char buf[6];
     i2c_read_bytes(ACC2, ACC2_OUT|0x80,(byte *) buf, 6);
     *x = buf[1]; *y = buf[3]; *z = -buf[5];
}

/* accel_start -- initialise accelerometer */
void accel_start(void) {
     byte buf;

     if (i2c_try_read(ACC1, 0x0d, &buf) == OK) {
          i2c_write_reg(ACC1, ACC1_CTRL_REG1, 0x23); // 50Hz, 8 bit, Active
          accel = 1;
     } else if (i2c_try_read(ACC2, 0x0f, &buf) == OK) {
          i2c_write_reg(ACC2, ACC2_CTRL_REG1, 0x4f); // 50Hz, 8 bit, Active
          accel = 2;
     } else {
          panic("Can't find accelerometer");
     }
}

/* accel_reading -- obtain accelerometer reading */
void accel_reading(int *x, int *y, int *z) {
    switch (accel) {
    case 1:
        acc1_read(x, y, z);
        break;
    case 2:
        acc2_read(x, y, z);
        break;
    default:
        panic("Unknown accelerometer");
    }
}
