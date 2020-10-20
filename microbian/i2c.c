// i2c.c
// Copyright (c) 2019 J. M. Spivey

#include "microbian.h"
#include "hardware.h"
#include <stddef.h>

/* i2c_wait -- wait for an expected interrupt event and detect error */
static int i2c_wait(unsigned volatile *event) {
    receive(INTERRUPT, NULL);

    if (I2C_ERROR) {
        I2C_ERROR = 0; 
        clear_pending(I2C_IRQ);
        enable_irq(I2C_IRQ);
        return ERROR;
    }      

    assert(*event);
    *event = 0;                                 
    clear_pending(I2C_IRQ);
    enable_irq(I2C_IRQ);
    return OK;
}

/* i2c_read -- read one or more bytes */
static int i2c_read(char *buf, int n) {
    int status = OK;

    for (int i = 0; i < n; i++) {
        if (i == n-1)
            I2C_SHORTS = BIT(I2C_BB_STOP);
        else
            I2C_SHORTS = BIT(I2C_BB_SUSPEND);
        
        if (i == 0)
            I2C_STARTRX = 1;
        else
            I2C_RESUME = 1;
        
        status = i2c_wait(&I2C_RXDREADY);
        if (status != OK) return status;
        buf[i] = I2C_RXD;
    }

    i2c_wait(&I2C_STOPPED);
    return OK;
}          
          
/* i2c_start_write -- start write transaction */
static void i2c_start_write(void) {
     I2C_SHORTS = 0;
     I2C_STARTTX = 1;
}

/* i2c_write -- send one or more bytes */
static int i2c_write(char *buf, int n) {
     int status = OK;

     /* The I2C hardware makes zero-length writes impossible, because
        there is no event generated when the address has been sent. */

     for (int i = 0; i < n; i++) {
          I2C_TXD = buf[i];
          status = i2c_wait(&I2C_TXDSENT);
          if (status != OK) return status;
     }

     return OK;
}

/* i2c_stop -- signal stop condition */
static void i2c_stop(void) {
     I2C_STOP = 1;
     i2c_wait(&I2C_STOPPED);
}

/* i2c_task -- driver process for I2C hardware */
static void i2c_task(int dummy) {
     message m;
     int client, addr, n1, n2, status, error = 0;
     char *buf1, *buf2;

     I2C_ENABLE = 0;

     // Connect pins as inputs and select open-drain mode
     SET_FIELD(GPIO_PINCNF[I2C_SCL], GPIO_PINCNF_INPUT, GPIO_INPUT_Connect);
     SET_FIELD(GPIO_PINCNF[I2C_SCL], GPIO_PINCNF_DRIVE, GPIO_DRIVE_S0D1);
     SET_FIELD(GPIO_PINCNF[I2C_SDA], GPIO_PINCNF_INPUT, GPIO_INPUT_Connect);
     SET_FIELD(GPIO_PINCNF[I2C_SDA], GPIO_PINCNF_DRIVE, GPIO_DRIVE_S0D1);

     // Configure I2C hardware
     I2C_PSELSCL = I2C_SCL;
     I2C_PSELSDA = I2C_SDA;
     I2C_FREQUENCY = I2C_FREQUENCY_100kHz;
     I2C_ENABLE = I2C_ENABLE_Enabled;

     // Enable interrupts
     I2C_INTEN = BIT(I2C_INT_RXDREADY) | BIT(I2C_INT_TXDSENT)
          | BIT(I2C_INT_STOPPED) | BIT(I2C_INT_ERROR);
     connect(I2C_IRQ);

     while (1) {
         receive(ANY, &m);
         client = m.m_sender;
         addr = m.m_b1;        // Address [0..127] without R/W flag
         n1 = m.m_b2;          // Number of bytes in command
         n2 = m.m_b3;          // Number of bytes to transfer (R/W)
         buf1 = m.m_p2;        // Buffer for command
         buf2 = m.m_p3;        // Buffer for transfer

         switch (m.m_type) {
         case READ:
             I2C_ADDRESS = addr;
             status = OK;
             
             // Write followed by read, with repeated start
               if (n1 > 0) {
                    i2c_start_write();
                    status = i2c_write(buf1, n1);
               }
               if (status == OK)
                    status = i2c_read(buf2, n2);

               if (status != OK) {
                   i2c_stop();
                   error = I2C_ERRORSRC;
                   I2C_ERRORSRC = I2C_ERRORSRC_All;
               }

               m.m_i1 = status;
               m.m_i2 = error;
               send(client, REPLY, &m);
               break;

          case WRITE:
               I2C_ADDRESS = addr;
               status = OK;

               // A single write transaction
               i2c_start_write();
               if (n1 > 0)
                    status = i2c_write(buf1, n1);
               if (status == OK)
                    status = i2c_write(buf2, n2);
               i2c_stop();

               if (status != OK) {
                    error = I2C_ERRORSRC;
                    I2C_ERRORSRC = I2C_ERRORSRC_All;
               }
               
               m.m_i1 = status;
               m.m_i2 = error;
               send(client, REPLY, &m);
               break;

          default:
               badmesg(m.m_type);
          }
     }
}

static int I2C;

/* i2c_init -- start I2C driver process */
void i2c_init(void) {
     I2C = start("I2C", i2c_task, 0, 256);
}

/* i2c_xfer -- i2c transaction with command write then data read or write */
int i2c_xfer(int kind, int addr, byte *buf1, int n1, byte *buf2, int n2) {
     message m;
     m.m_b1 = addr;
     m.m_b2 = n1;
     m.m_b3 = n2;
     m.m_p2 = buf1;
     m.m_p3 = buf2;
     sendrec(I2C, kind, &m);
     return m.m_i1;
}

/* i2c_try_read -- try to read from I2C device */
int i2c_try_read(int addr, int cmd, byte *buf2) {
     byte buf1 = cmd;
     return i2c_xfer(READ, addr, &buf1, 1, buf2, 1);
}
     
/* i2c_read_bytes -- send command and read multi-byte result */
void i2c_read_bytes(int addr, int cmd, byte *buf2, int n2) {
     byte buf1 = cmd;
     int status = i2c_xfer(READ, addr, &buf1, 1, buf2, n2);
     assert(status == OK);
}

/* i2c_read_reg -- send command and read one byte */
int i2c_read_reg(int addr, int cmd) {
    byte buf;
    i2c_read_bytes(addr, cmd, &buf, 1);
    return buf;
}

/* i2c_write_bytes -- send command and write multi-byte data */
void i2c_write_bytes(int addr, int cmd, byte *buf2, int n2) {
    byte buf1 = cmd;
    int status = i2c_xfer(WRITE, addr, &buf1, 1, buf2, n2);
    assert(status == OK);
}

/* i2c_write_reg -- send command and write data */
void i2c_write_reg(int addr, int cmd, int val) {
    byte buf = val;
    i2c_write_bytes(addr, cmd, &buf, 1);
}
