/* common/hardware.h */
/* Copyright (c) 2018-20 J. M. Spivey */

#define UBIT 1
#define UBIT_V1 1

/* Hardware register definitions for nRF51822 */

#define BIT(i) (1 << (i))
#define GET_BIT(reg, n) (((reg) >> (n)) & 0x1)
#define SET_BIT(reg, n) reg |= BIT(n)
#define CLR_BIT(reg, n) reg &= ~BIT(n)

#define GET_BYTE(reg, n) (((reg) >> (8*(n))) & 0xff)
#define SET_BYTE(reg, n, v) \
    reg = (reg & ~(0xff << 8*n)) | ((v & 0xff) << 8*n)

/* The macros SET_FIELD, etc., are defined in an indirect way that
permits (because of the timing of CPP macro expansion) the 'field'
argument to be a macro that expands the a 'position, width' pair. */

#define SET_FIELD(reg, field, val) __SET_FIELD(reg, field, val)
#define __SET_FIELD(reg, pos, wid, val) \
    reg = (reg & ~__MASK(pos, wid)) | __FIELD(pos, wid, val)

#define GET_FIELD(reg, field) __GET_FIELD(reg, field)
#define __GET_FIELD(reg, pos, wid)  ((reg >> pos) & __MASK0(wid))

#define FIELD(field, val) __FIELD(field, val)
#define __FIELD(pos, wid, val)  (((val) & __MASK0(wid)) << pos)

#define MASK(field) __MASK(field)
#define __MASK(pos, wid)  (__MASK0(wid) << pos)

#define __MASK0(wid)  (~((-2) << (wid-1)))

    
/* Device pins */
#define PAD19 0
#define  I2C_SCL PAD19
#define PAD2 1
#define PAD1 2
#define PAD0 3
/* LED columns are GPIO 4-12 */
#define PAD3 4
#define PAD4 5
#define PAD10 6
#define PAD9 10
#define PAD7 11
#define PAD6 12
#define ROW1 13
#define ROW2 14
#define ROW3 15
#define PAD16 16
#define PAD5 17
#define  BUTTON_A PAD5
#define PAD8 18
#define PAD12 20
#define PAD15 21
#define   SPI_MOSI PAD15
#define PAD14 22
#define  SPI_MISO PAD14
#define PAD13 23
#define  SPI_SCK PAD13
#define USB_TX 24
#define USB_RX 25
#define PAD11 26
#define  BUTTON_B PAD11
#define PAD20 30
#define  I2C_SDA PAD20

/* Only one I2C interface */
#define I2C_INTERNAL 0
#define I2C_EXTERNAL 0
#define N_I2CS 1


/* Interrupts */
#define SVC_IRQ    -5
#define PENDSV_IRQ -2
#define RADIO_IRQ   1
#define UART_IRQ    2
#define I2C_IRQ     3
#define SPI_IRQ     4
#define GPIOTE_IRQ  6
#define ADC_IRQ     7
#define TIMER0_IRQ  8
#define TIMER1_IRQ  9
#define TIMER2_IRQ 10
#define RTC0_IRQ   11
#define TEMP_IRQ   12
#define RNG_IRQ    13
#define RTC1_IRQ   17

#define N_INTERRUPTS 32


/* Device register structures */
#define _DEVICE  union
#define _REGISTER(decl, offset) \
    struct { unsigned char __pad##offset[offset]; decl; }


/* System contol block */
_DEVICE _scb {
    _REGISTER(unsigned CPUID, 0x00);
    _REGISTER(unsigned ICSR, 0x04);
#define   SCB_ICSR_PENDSVSET 28
#define   SCB_ICSR_VECTACTIVE 0, 8
    _REGISTER(unsigned SCR, 0x10);
#define   SCB_SCR_SLEEPONEXIT 1
#define   SCB_SCR_SLEEPDEEP 2
#define   SCB_SCR_SEVONPEND 4
    _REGISTER(unsigned SHPR[3], 0x18);
};

#define SCB (* (volatile _DEVICE _scb *) 0xe000ed00)


/* Nested vectored interupt controller */
_DEVICE _nvic {
    _REGISTER(unsigned ISER[8], 0x100);
    _REGISTER(unsigned ICER[8], 0x180);
    _REGISTER(unsigned ISPR[8], 0x200);
    _REGISTER(unsigned ICPR[8], 0x280);
    _REGISTER(unsigned IPR[60], 0x400);
};

#define NVIC (* (volatile _DEVICE _nvic *) 0xe000e000)


/* Clock control */
_DEVICE _clock {
    _REGISTER(unsigned HFCLKSTART, 0x000);
    _REGISTER(unsigned LFCLKSTART, 0x008);
    _REGISTER(unsigned HFCLKSTARTED, 0x100);
    _REGISTER(unsigned LFCLKSTARTED, 0x104);
    _REGISTER(unsigned LFCLKSRC, 0x518);
#define   CLOCK_LFCLKSRC_RC 0
    _REGISTER(unsigned XTALFREQ, 0x550);
#define   CLOCK_XTALFREQ_16MHz 0xFF
};

#define CLOCK (* (volatile _DEVICE _clock *) 0x40000000)


/* Memory protection unit */
_DEVICE _mpu {
    _REGISTER(unsigned DISABLEINDEBUG, 0x608);
};

#define MPU (* (volatile _DEVICE _mpu *) 0x40000000)


/* Factory information */
_DEVICE _ficr {
    _REGISTER(unsigned DEVICEID[2], 0x060);
    _REGISTER(unsigned DEVICEADDR[2], 0x0a4);
    _REGISTER(unsigned OVERRIDEEN, 0x0a0);
#define   FICR_OVERRIDEEN_NRF 0
    _REGISTER(unsigned NRF_1MBIT[5], 0x0b0);
};

#define FICR (* (volatile _DEVICE _ficr *) 0x10000000)


/* Non-Volatile Memory Controller */
_DEVICE _nvmc {
    _REGISTER(unsigned READY, 0x400);
    _REGISTER(unsigned CONFIG, 0x504);
#define   NVMC_CONFIG_WEN 0
#define   NVMC_CONFIG_EEN 1   
    _REGISTER(void *ERASEPAGE, 0x508);
    _REGISTER(unsigned ICACHECONF, 0x540);
#define   NVMC_ICACHECONF_CACHEEN 0
};

#define NVMC (* (volatile _DEVICE _nvmc *) 0x4001e000)


/* GPIO */
_DEVICE _gpio {
/* Registers */
    _REGISTER(unsigned OUT, 0x004);
    _REGISTER(unsigned OUTSET, 0x008);
    _REGISTER(unsigned OUTCLR, 0x00c);
    _REGISTER(unsigned IN, 0x010);
    _REGISTER(unsigned DIR, 0x014);
    _REGISTER(unsigned DIRSET, 0x018);
    _REGISTER(unsigned DIRCLR, 0x01c);
    _REGISTER(unsigned PINCNF[32], 0x200);
#define   GPIO_PINCNF_DIR 0, 1
#define     GPIO_DIR_Input 0
#define     GPIO_DIR_Output 1
#define   GPIO_PINCNF_INPUT 1, 1
#define     GPIO_INPUT_Connect 0
#define     GPIO_INPUT_Disconnect 1
#define   GPIO_PINCNF_PULL 2, 2
#define     GPIO_PULL_Disabled 0
#define     GPII_PULL_Pulldown 1    
#define     GPIO_PULL_Pullup 3
#define   GPIO_PINCNF_DRIVE 8, 3
#define     GPIO_DRIVE_S0S1 0
#define     GPIO_DRIVE_H0S1 1
#define     GPIO_DRIVE_S0H1 2
#define     GPIO_DRIVE_S0D1 6 /* Open drain */
#define   GPIO_PINCNF_SENSE 16, 2
#define     GPIO_SENSE_Disabled 0
#define     GPIO_SENSE_High 2
#define     GPIO_SENSE_Low 3
};

#define GPIO (* (volatile _DEVICE _gpio *) 0x50000500)


/* GPIOTE */
_DEVICE _gpiote {
/* Tasks */
    _REGISTER(unsigned OUT[4], 0x000);
    _REGISTER(unsigned SET[4], 0x030);
    _REGISTER(unsigned CLR[4], 0x060);
/* Events */
    _REGISTER(unsigned IN[4], 0x100);
    _REGISTER(unsigned PORT, 0x17c);
/* Registers */
    _REGISTER(unsigned INTENSET, 0x304);
    _REGISTER(unsigned INTENCLR, 0x308);
    _REGISTER(unsigned CONFIG[4], 0x510);
#define   GPIOTE_CONFIG_MODE 0, 2
#define     GPIOTE_MODE_Event 1
#define     GPIOTE_MODE_Task 3
#define   GPIOTE_CONFIG_PSEL 8, 6 /* Note 6 bits including port number */
#define   GPIOTE_CONFIG_POLARITY 16, 2
#define     GPIOTE_POLARITY_LoToHi 1
#define     GPIOTE_POLARITY_HiToLo 2
#define     GPIOTE_POLARITY_Toggle 3
#define   GPIOTE_CONFIG_OUTINIT 20, 1
};

/* Interrupts */
#define GPIOTE_INT_IN0 0
#define GPIOTE_INT_IN1 1
#define GPIOTE_INT_IN2 2
#define GPIOTE_INT_IN3 3
#define GPIOTE_INT_PORT 31

#define GPIOTE (* (volatile _DEVICE _gpiote *) 0x40006000)


/* PPI */
_DEVICE _ppi {
/* Tasks */
    _REGISTER(struct {
        unsigned EN;
        unsigned DIS;
    } CHG[6], 0x000);
/* Registers */
    _REGISTER(unsigned CHEN, 0x500);
    _REGISTER(unsigned CHENSET, 0x504);
    _REGISTER(unsigned CHENCLR, 0x508);
    _REGISTER(struct {
        unsigned volatile *EEP;
        unsigned volatile *TEP;       
    } CH[20], 0x510);
    _REGISTER(unsigned CHGRP[6], 0x800);
};

#define PPI (* (volatile _DEVICE _ppi *) 0x4001f000)


/* Radio */
_DEVICE _radio {
/* Tasks */
    _REGISTER(unsigned TXEN, 0x000);
    _REGISTER(unsigned RXEN, 0x004);
    _REGISTER(unsigned START, 0x008);
    _REGISTER(unsigned STOP, 0x00c);
    _REGISTER(unsigned DISABLE, 0x010);
    _REGISTER(unsigned RSSISTART, 0x014);
    _REGISTER(unsigned RSSISTOP, 0x018);
    _REGISTER(unsigned BCSTART, 0x01c);
    _REGISTER(unsigned BCSTOP, 0x020);
/* Events */
    _REGISTER(unsigned READY, 0x100);
    _REGISTER(unsigned ADDRESS, 0x104);
    _REGISTER(unsigned PAYLOAD, 0x108);
    _REGISTER(unsigned END, 0x10c);
    _REGISTER(unsigned DISABLED, 0x110);
    _REGISTER(unsigned DEVMATCH, 0x114);
    _REGISTER(unsigned DEVMISS, 0x118);
    _REGISTER(unsigned RSSIEND, 0x11c);
    _REGISTER(unsigned BCMATCH, 0x128);
/* Registers */
    _REGISTER(unsigned SHORTS, 0x200);
    _REGISTER(unsigned INTENSET, 0x304);
    _REGISTER(unsigned INTENCLR, 0x308);
    _REGISTER(unsigned CRCSTATUS, 0x400);
    _REGISTER(unsigned RXMATCH, 0x408);
    _REGISTER(unsigned RXCRC, 0x40c);
    _REGISTER(unsigned DAI, 0x410);
    _REGISTER(void *PACKETPTR, 0x504);
    _REGISTER(unsigned FREQUENCY, 0x508);
    _REGISTER(unsigned TXPOWER, 0x50c);
    _REGISTER(unsigned MODE, 0x510);
#define   RADIO_MODE_NRF_1Mbit 0
    _REGISTER(unsigned PCNF0, 0x514);
#define   RADIO_PCNF0_LFLEN 0, 3
#define   RADIO_PCNF0_S0LEN 8, 1
#define   RADIO_PCNF0_S1LEN 16, 4
    _REGISTER(unsigned PCNF1, 0x518);
#define   RADIO_PCNF1_MAXLEN 0, 8
#define   RADIO_PCNF1_STATLEN 8, 8
#define   RADIO_PCNF1_BALEN 16, 3
#define   RADIO_PCNF1_ENDIAN 24, 1
#define     RADIO_ENDIAN_Little 0
#define     RADIO_ENDIAN_Big 1
#define   RADIO_PCNF1_WHITEEN 25
    _REGISTER(unsigned BASE0, 0x51c);
    _REGISTER(unsigned BASE1, 0x520);
    _REGISTER(unsigned PREFIX0, 0x524);
    _REGISTER(unsigned PREFIX1, 0x528);
    _REGISTER(unsigned TXADDRESS, 0x52c);
    _REGISTER(unsigned RXADDRESSES, 0x530);
    _REGISTER(unsigned CRCCNF, 0x534);
    _REGISTER(unsigned CRCPOLY, 0x538);
    _REGISTER(unsigned CRCINIT, 0x53c);
    _REGISTER(unsigned TEST, 0x540);
    _REGISTER(unsigned TIFS, 0x544);
    _REGISTER(unsigned RSSISAMPLE, 0x548);
    _REGISTER(unsigned STATE, 0x550);
    _REGISTER(unsigned DATAWHITEIV, 0x554);
    _REGISTER(unsigned BCC, 0x560);
    _REGISTER(unsigned DAB[8], 0x600);
    _REGISTER(unsigned DAP[8], 0x620);
    _REGISTER(unsigned DACNF, 0x640);
    _REGISTER(unsigned OVERRIDE[5], 0x724);
    _REGISTER(unsigned POWER, 0xffc);
};

/* Interrupts */
#define RADIO_INT_READY 0
#define RADIO_INT_END 3
#define RADIO_INT_DISABLED 4

#define RADIO (* (volatile _DEVICE _radio *) 0x40001000)


/* TIMERS: Timer 0 is 8/16/24/32 bit, Timers 1 and 2 are 8/16 bit. */

_DEVICE _timer {
/* Tasks */
    _REGISTER(unsigned START, 0x000);
    _REGISTER(unsigned STOP, 0x004);
    _REGISTER(unsigned COUNT, 0x008);
    _REGISTER(unsigned CLEAR, 0x00c);
    _REGISTER(unsigned SHUTDOWN, 0x010);
    _REGISTER(unsigned CAPTURE[4], 0x040);
/* Events */
    _REGISTER(unsigned COMPARE[4], 0x140);
/* Registers */
    _REGISTER(unsigned SHORTS, 0x200);
    _REGISTER(unsigned INTENSET, 0x304);
    _REGISTER(unsigned INTENCLR, 0x308);
    _REGISTER(unsigned MODE, 0x504);
#define   TIMER_MODE_Timer 0
#define   TIMER_MODE_Counter 1
    _REGISTER(unsigned BITMODE, 0x508);
#define   TIMER_BITMODE_16Bit 0
#define   TIMER_BITMODE_8Bit 1
#define   TIMER_BITMODE_24Bit 2
#define   TIMER_BITMODE_32Bit 3
    _REGISTER(unsigned PRESCALER, 0x510);
    _REGISTER(unsigned CC[4], 0x540);
};

/* Interrupts */
#define TIMER_INT_COMPARE0 16
#define TIMER_INT_COMPARE1 17
#define TIMER_INT_COMPARE2 18
#define TIMER_INT_COMPARE3 19
/* Shortcuts */
#define TIMER_COMPARE0_CLEAR 0
#define TIMER_COMPARE1_CLEAR 1
#define TIMER_COMPARE2_CLEAR 2
#define TIMER_COMPARE3_CLEAR 3
#define TIMER_COMPARE0_STOP 8
#define TIMER_COMPARE1_STOP 9
#define TIMER_COMPARE2_STOP 10
#define TIMER_COMPARE3_STOP 11

#define TIMER0 (* (volatile _DEVICE _timer *) 0x40008000)
#define TIMER1 (* (volatile _DEVICE _timer *) 0x40009000)
#define TIMER2 (* (volatile _DEVICE _timer *) 0x4000a000)

extern volatile _DEVICE _timer * const TIMER[3];


/* Random Number Generator */
_DEVICE _rng {
/* Tasks */
    _REGISTER(unsigned START, 0x000);
    _REGISTER(unsigned STOP, 0x004);
/* Events */
    _REGISTER(unsigned VALRDY, 0x100);
/* Registers */
    _REGISTER(unsigned SHORTS, 0x200);
    _REGISTER(unsigned INTEN, 0x300);
    _REGISTER(unsigned INTENSET, 0x304);
    _REGISTER(unsigned INTENCLR, 0x308);
    _REGISTER(unsigned CONFIG, 0x504);
#define   RNG_CONFIG_DERCEN 0
    _REGISTER(unsigned VALUE, 0x508);
};

/* Interrupts */
#define RNG_INT_VALRDY 0

#define RNG (* (volatile _DEVICE _rng *) 0x4000d000)


/* Temperature sensor */
_DEVICE _temp {
/* Tasks */
    _REGISTER(unsigned START, 0x000);
    _REGISTER(unsigned STOP, 0x004);
/* Events */
    _REGISTER(unsigned DATARDY, 0x100);
/* Registers */
    _REGISTER(unsigned INTEN, 0x300);
    _REGISTER(unsigned INTENSET, 0x304);
    _REGISTER(unsigned INTENCLR, 0x308);
    _REGISTER(unsigned VALUE, 0x508);
};

/* Interrupts */
#define TEMP_INT_DATARDY 0

#define TEMP (* (volatile _DEVICE _temp *) 0x4000c000)


/* I2C */
_DEVICE _i2c {
/* Tasks */
    _REGISTER(unsigned STARTRX, 0x000);
    _REGISTER(unsigned STARTTX, 0x008);
    _REGISTER(unsigned STOP, 0x014);
    _REGISTER(unsigned SUSPEND, 0x01c);
    _REGISTER(unsigned RESUME, 0x020);
/* Events */
    _REGISTER(unsigned STOPPED, 0x104);
    _REGISTER(unsigned RXDREADY, 0x108);
    _REGISTER(unsigned TXDSENT, 0x11c);
    _REGISTER(unsigned ERROR, 0x124);
    _REGISTER(unsigned BB, 0x138);
    _REGISTER(unsigned SUSPENDED, 0x148);
/* Registers */
    _REGISTER(unsigned SHORTS, 0x200);
    _REGISTER(unsigned INTEN, 0x300);
    _REGISTER(unsigned INTENSET, 0x304);
    _REGISTER(unsigned INTENCLR, 0x308);
    _REGISTER(unsigned ERRORSRC, 0x4c4);
#define   I2C_ERRORSRC_OVERRUN 0
#define   I2C_ERRORSRC_ANACK 1
#define   I2C_ERRORSRC_DNACK 2
#define   I2C_ERRORSRC_All 0x7
    _REGISTER(unsigned ENABLE, 0x500) ;
#define   I2C_ENABLE_Disabled 0
#define   I2C_ENABLE_Enabled 5
    _REGISTER(unsigned PSELSCL, 0x508);
    _REGISTER(unsigned PSELSDA, 0x50c);
    _REGISTER(unsigned RXD, 0x518);
    _REGISTER(unsigned TXD, 0x51c) ;
    _REGISTER(unsigned FREQUENCY, 0x524);
#define   I2C_FREQUENCY_100kHz 0x01980000
    _REGISTER(unsigned ADDRESS, 0x588);
    _REGISTER(unsigned POWER, 0xffc);
};
    
/* Interrupts */
#define I2C_INT_STOPPED 1
#define I2C_INT_RXDREADY 2
#define I2C_INT_TXDSENT 7
#define I2C_INT_ERROR 9
#define I2C_INT_BB 14
/* Shortcuts */
#define I2C_BB_SUSPEND 0
#define I2C_BB_STOP 1

/* Microbian's I2C driver is written to support multiple instances, so
let's make an array, even though the V1 has only one I2C interface. */

#define I2C0 (* (volatile _DEVICE _i2c *) 0x40003000)

extern volatile _DEVICE _i2c * const I2C[1];


/* UART */
_DEVICE _uart {
/* Tasks */
    _REGISTER(unsigned STARTRX, 0x000);
    _REGISTER(unsigned STARTTX, 0x008);
/* Events */
    _REGISTER(unsigned RXDRDY, 0x108);
    _REGISTER(unsigned TXDRDY, 0x11c);
/* Registers */
    _REGISTER(unsigned INTENSET, 0x304);
    _REGISTER(unsigned INTENCLR, 0x308);
    _REGISTER(unsigned ENABLE, 0x500);
#define   UART_ENABLE_Disabled 0
#define   UART_ENABLE_Enabled 4
    _REGISTER(unsigned PSELTXD, 0x50c);
    _REGISTER(unsigned PSELRXD, 0x514);
    _REGISTER(unsigned RXD, 0x518);
    _REGISTER(unsigned TXD, 0x51c);
    _REGISTER(unsigned BAUDRATE, 0x524);
#define   UART_BAUDRATE_1200   0x0004f000
#define   UART_BAUDRATE_2400   0x0009d000
#define   UART_BAUDRATE_4800   0x0013b000
#define   UART_BAUDRATE_9600   0x00275000
#define   UART_BAUDRATE_14400  0x003af000
#define   UART_BAUDRATE_19200  0x004ea000
#define   UART_BAUDRATE_28800  0x0075c000
#define   UART_BAUDRATE_31250  0x00800000
#define   UART_BAUDRATE_38400  0x009d0000
#define   UART_BAUDRATE_56000  0x00e50000
#define   UART_BAUDRATE_57600  0x00eb0000
#define   UART_BAUDRATE_76800  0x013a9000
#define   UART_BAUDRATE_115200 0x01d60000
#define   UART_BAUDRATE_230400 0x03b00000
#define   UART_BAUDRATE_250000 0x04000000
#define   UART_BAUDRATE_460800 0x07400000
#define   UART_BAUDRATE_921600 0x0f000000
#define   UART_BAUDRATE_1M     0x10000000
    _REGISTER(unsigned CONFIG, 0x56c);
#define   UART_CONFIG_HWFC 0
#define   UART_CONFIG_PARITY 1, 3
#define     UART_PARITY_None 0
#define     UART_PARITY_Even 7
};

/* Interrupts */
#define   UART_INT_RXDRDY 2
#define   UART_INT_TXDRDY 7

#define UART (* (volatile _DEVICE _uart *) 0x40002000)


/* ADC */
_DEVICE _adc {
/* Tasks */
    _REGISTER(unsigned START, 0x000);
    _REGISTER(unsigned STOP, 0x004);
/* Events */
    _REGISTER(unsigned END, 0x100);
/* Registers */
    _REGISTER(unsigned INTEN, 0x300);
    _REGISTER(unsigned INTENSET, 0x304);
    _REGISTER(unsigned INTENCLR, 0x308);
    _REGISTER(unsigned BUSY, 0x400);
    _REGISTER(unsigned ENABLE, 0x500);
    _REGISTER(unsigned CONFIG, 0x504);
#define   ADC_CONFIG_RES 0, 2
#define     ADC_RES_8Bit 0
#define     ADC_RES_9bit 1
#define     ADC_RES_10bit 2
#define   ADC_CONFIG_INPSEL 2, 3
#define     ADC_INPSEL_AIn_1_1 0
#define     ADC_INPSEL_AIn_2_3 1
#define     ADC_INPSEL_AIn_1_3 2
#define     ADC_INPSEL_Vdd_2_3 5
#define     ADC_INPSEL_Vdd_1_3 6
#define   ADC_CONFIG_REFSEL 5, 2
#define     ADC_REFSEL_BGap 0
#define     ADC_REFSEL_Ext 1
#define     ADC_REFSEL_Vdd_1_2 2
#define     ADC_REFSEL_Vdd_1_3 3
#define   ADC_CONFIG_PSEL 8, 8
#define   ADC_CONFIG_EXTREFSEL 16, 2
#define     ADC_EXTREFSEL_Ref0 1
#define     ADC_EXTREFSEL_Ref1 2
    _REGISTER(unsigned RESULT, 0x508);
};

/* Interrupts */
#define ADC_INT_END 0

#define ADC (* (volatile _DEVICE _adc *) 0x40007000)

    
/* NVIC stuff */

/* irq_priority -- set priority of an IRQ from 0 (highest) to 255 */
void irq_priority(int irq, unsigned priority);

/* enable_irq -- enable interrupts from an IRQ */
#define enable_irq(irq)  NVIC.ISER[0] = BIT(irq)

/* disable_irq -- disable interrupts from a specific IRQ */
#define disable_irq(irq)  NVIC.ICER[0] = BIT(irq)

/* clear_pending -- clear pending interrupt from an IRQ */
#define clear_pending(irq)  NVIC.ICPR[0] = BIT(irq)

/* reschedule -- request PendSV interrupt */
#define reschedule()  SCB.ICSR = BIT(SCB_ICSR_PENDSVSET)

/* active_irq -- find active interrupt: returns -16 to 31 */
#define active_irq()  (GET_FIELD(SCB.ICSR, SCB_ICSR_VECTACTIVE) - 16)

/* delay_loop -- timed delay */
void delay_loop(unsigned usec);


/* GPIO convenience */

/* gpio_dir -- set GPIO direction */
inline void gpio_dir(unsigned pin, unsigned dir) {
    if (dir)
        GPIO.DIRSET = BIT(pin);
    else
        GPIO.DIRCLR = BIT(pin);
}

/* gpio_connect -- connect pin for input */
inline void gpio_connect(unsigned pin) {
    SET_FIELD(GPIO.PINCNF[pin], GPIO_PINCNF_INPUT, GPIO_INPUT_Connect);
}

/* gpio_drive -- set GPIO drive strength */
inline void gpio_drive(unsigned pin, unsigned mode) {
    SET_FIELD(GPIO.PINCNF[pin], GPIO_PINCNF_DRIVE, mode);
}

/* gpio_out -- set GPIO output value */
inline void gpio_out(unsigned pin, unsigned value) {
    if (value)
        GPIO.OUTSET = BIT(pin);
    else
        GPIO.OUTCLR = BIT(pin);
}

/* gpio_in -- get GPIO input bit */
inline unsigned gpio_in(unsigned pin) {
    return GET_BIT(GPIO.IN, pin);
}


/* Image constants */

#define NIMG 3

typedef unsigned image[NIMG];

#define __ROW(r, c1, c2, c3, c4, c5, c6, c7, c8, c9)                 \
    (BIT(r) | !c1<<4 | !c2<<5 | !c3<<6 | !c4<<7 | !c5<<8             \
     | !c6<<9 | !c7<<10 | !c8<<11 | !c9<<12)

#define IMAGE(x11, x24, x12, x25, x13,                               \
              x34, x35, x36, x37, x38,                               \
              x22, x19, x23, x39, x21,                               \
              x18, x17, x16, x15, x14,                               \
              x33, x27, x31, x26, x32)                               \
    { __ROW(ROW1, x11, x12, x13, x14, x15, x16, x17, x18, x19),      \
      __ROW(ROW2, x21, x22, x23, x24, x25, x26, x27, 0, 0),          \
      __ROW(ROW3, x31, x32, x33, x34, x35, x36, x37, x38, x39) }

#define LED_MASK 0xfff0

#define led_init()  GPIO.DIRSET = LED_MASK
#define led_dot()   GPIO.OUTSET = 0x5fbf
#define led_off()   GPIO.OUTCLR = LED_MASK


/* CODERAM -- mark function for copying to RAM (disabled on V1) */
#define CODERAM

/* A few assembler macros for single instructions. */
#define pause()         asm volatile ("wfe")
#define intr_disable()  asm volatile ("cpsid i")
#define intr_enable()   asm volatile ("cpsie i")
#define get_primask()   ({unsigned x; asm ("mrs %0, primask" : "=r" (x)); x;})
#define set_primask(x)  asm ("msr primask, %0" : : "r" (x))
#define nop()           asm volatile ("nop")
#define syscall(op)     asm volatile ("svc %0" : : "i"(op))
