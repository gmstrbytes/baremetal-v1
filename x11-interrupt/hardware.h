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
#define _DEVSTRUCT union
#define __concat(x, y) x##y /* Don't ask! */
#define _PADDING(offset, line) unsigned char __concat(_pad_, line)[offset]
#define _REGISTER(decl, offset) \
    struct { _PADDING(offset, __LINE__); decl; }


/* System contol block */
_DEVSTRUCT _scb {
    _REGISTER(unsigned volatile CPUID, 0x00);
    _REGISTER(unsigned volatile ICSR, 0x04);
#define   SCB_ICSR_PENDSVSET 28
#define   SCB_ICSR_VECTACTIVE 0, 8
    _REGISTER(unsigned volatile SCR, 0x10);
#define   SCB_SCR_SLEEPONEXIT 1
#define   SCB_SCR_SLEEPDEEP 2
#define   SCB_SCR_SEVONPEND 4
    _REGISTER(unsigned volatile SHPR[3], 0x18);
};

#define SCB (* (_DEVSTRUCT _scb *) 0xe000ed00)


/* Nested vectored interupt controller */
_DEVSTRUCT _nvic {
    _REGISTER(unsigned volatile ISER[8], 0x100);
    _REGISTER(unsigned volatile ICER[8], 0x180);
    _REGISTER(unsigned volatile ISPR[8], 0x200);
    _REGISTER(unsigned volatile ICPR[8], 0x280);
    _REGISTER(unsigned volatile IPR[60], 0x400);
};

#define NVIC (* (_DEVSTRUCT _nvic *) 0xe000e000)


/* Clock control */
_DEVSTRUCT _clock {
    _REGISTER(unsigned volatile HFCLKSTART, 0x000);
    _REGISTER(unsigned volatile LFCLKSTART, 0x008);
    _REGISTER(unsigned volatile HFCLKSTARTED, 0x100);
    _REGISTER(unsigned volatile LFCLKSTARTED, 0x104);
    _REGISTER(unsigned volatile LFCLKSRC, 0x518);
#define   CLOCK_LFCLKSRC_RC 0
    _REGISTER(unsigned volatile XTALFREQ, 0x550);
#define   CLOCK_XTALFREQ_16MHz 0xFF
};

#define CLOCK (* (_DEVSTRUCT _clock *) 0x40000000)


/* Memory protection unit */
_DEVSTRUCT _mpu {
    _REGISTER(unsigned volatile DISABLEINDEBUG, 0x608);
};

#define MPU (* (_DEVSTRUCT _mpu *) 0x40000000)


/* Factory information */
_DEVSTRUCT _ficr {
    _REGISTER(unsigned volatile DEVICEID[2], 0x060);
    _REGISTER(unsigned volatile DEVICEADDR[2], 0x0a4);
    _REGISTER(unsigned volatile OVERRIDEEN, 0x0a0);
#define   FICR_OVERRIDEEN_NRF 0
    _REGISTER(unsigned volatile NRF_1MBIT[5], 0x0b0);
};

#define FICR (* (_DEVSTRUCT _ficr *) 0x10000000)


/* Non-Volatile Memory Controller */
_DEVSTRUCT _nvmc {
    _REGISTER(unsigned volatile READY, 0x400);
    _REGISTER(unsigned volatile CONFIG, 0x504);
#define   NVMC_CONFIG_WEN 0
#define   NVMC_CONFIG_EEN 1   
    _REGISTER(void * volatile ERASEPAGE, 0x508);
    _REGISTER(unsigned volatile ICACHECONF, 0x540);
#define   NVMC_ICACHECONF_CACHEEN 0
};

#define NVMC (* (_DEVSTRUCT _nvmc *) 0x4001e000)


/* GPIO */
_DEVSTRUCT _gpio {
/* Registers */
    _REGISTER(unsigned volatile OUT, 0x004);
    _REGISTER(unsigned volatile OUTSET, 0x008);
    _REGISTER(unsigned volatile OUTCLR, 0x00c);
    _REGISTER(unsigned volatile IN, 0x010);
    _REGISTER(unsigned volatile DIR, 0x014);
    _REGISTER(unsigned volatile DIRSET, 0x018);
    _REGISTER(unsigned volatile DIRCLR, 0x01c);
    _REGISTER(unsigned volatile PINCNF[32], 0x200);
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

#define GPIO (* (_DEVSTRUCT _gpio *) 0x50000500)


/* GPIOTE */
_DEVSTRUCT _gpiote {
/* Tasks */
    _REGISTER(unsigned volatile OUT[4], 0x000);
    _REGISTER(unsigned volatile SET[4], 0x030);
    _REGISTER(unsigned volatile CLR[4], 0x060);
/* Events */
    _REGISTER(unsigned volatile IN[4], 0x100);
    _REGISTER(unsigned volatile PORT, 0x17c);
/* Registers */
    _REGISTER(unsigned volatile INTENSET, 0x304);
    _REGISTER(unsigned volatile INTENCLR, 0x308);
    _REGISTER(unsigned volatile CONFIG[4], 0x510);
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

#define GPIOTE (* (_DEVSTRUCT _gpiote *) 0x40006000)


/* PPI */
_DEVSTRUCT _ppi {
/* Tasks */
    _REGISTER(struct {
        unsigned volatile EN;
        unsigned volatile DIS;
    } CHG[6], 0x000);
/* Registers */
    _REGISTER(unsigned volatile CHEN, 0x500);
    _REGISTER(unsigned volatile CHENSET, 0x504);
    _REGISTER(unsigned volatile CHENCLR, 0x508);
    _REGISTER(struct {
        unsigned volatile * volatile EEP;
        unsigned volatile * volatile TEP;       
    } CH[20], 0x510);
    _REGISTER(unsigned volatile CHGRP[6], 0x800);
};

#define PPI (* (_DEVSTRUCT _ppi *) 0x4001f000)


/* Radio */
_DEVSTRUCT _radio {
/* Tasks */
    _REGISTER(unsigned volatile TXEN, 0x000);
    _REGISTER(unsigned volatile RXEN, 0x004);
    _REGISTER(unsigned volatile START, 0x008);
    _REGISTER(unsigned volatile STOP, 0x00c);
    _REGISTER(unsigned volatile DISABLE, 0x010);
    _REGISTER(unsigned volatile RSSISTART, 0x014);
    _REGISTER(unsigned volatile RSSISTOP, 0x018);
    _REGISTER(unsigned volatile BCSTART, 0x01c);
    _REGISTER(unsigned volatile BCSTOP, 0x020);
/* Events */
    _REGISTER(unsigned volatile READY, 0x100);
    _REGISTER(unsigned volatile ADDRESS, 0x104);
    _REGISTER(unsigned volatile PAYLOAD, 0x108);
    _REGISTER(unsigned volatile END, 0x10c);
    _REGISTER(unsigned volatile DISABLED, 0x110);
    _REGISTER(unsigned volatile DEVMATCH, 0x114);
    _REGISTER(unsigned volatile DEVMISS, 0x118);
    _REGISTER(unsigned volatile RSSIEND, 0x11c);
    _REGISTER(unsigned volatile BCMATCH, 0x128);
/* Registers */
    _REGISTER(unsigned volatile SHORTS, 0x200);
    _REGISTER(unsigned volatile INTENSET, 0x304);
    _REGISTER(unsigned volatile INTENCLR, 0x308);
    _REGISTER(unsigned volatile CRCSTATUS, 0x400);
    _REGISTER(unsigned volatile RXMATCH, 0x408);
    _REGISTER(unsigned volatile RXCRC, 0x40c);
    _REGISTER(unsigned volatile DAI, 0x410);
    _REGISTER(void * volatile PACKETPTR, 0x504);
    _REGISTER(unsigned volatile FREQUENCY, 0x508);
    _REGISTER(unsigned volatile TXPOWER, 0x50c);
    _REGISTER(unsigned volatile MODE, 0x510);
#define   RADIO_MODE_NRF_1Mbit 0
    _REGISTER(unsigned volatile PCNF0, 0x514);
#define   RADIO_PCNF0_LFLEN 0, 3
#define   RADIO_PCNF0_S0LEN 8, 1
#define   RADIO_PCNF0_S1LEN 16, 4
    _REGISTER(unsigned volatile PCNF1, 0x518);
#define   RADIO_PCNF1_MAXLEN 0, 8
#define   RADIO_PCNF1_STATLEN 8, 8
#define   RADIO_PCNF1_BALEN 16, 3
#define   RADIO_PCNF1_ENDIAN 24, 1
#define     RADIO_ENDIAN_Little 0
#define     RADIO_ENDIAN_Big 1
#define   RADIO_PCNF1_WHITEEN 25
    _REGISTER(unsigned volatile BASE0, 0x51c);
    _REGISTER(unsigned volatile BASE1, 0x520);
    _REGISTER(unsigned volatile PREFIX0, 0x524);
    _REGISTER(unsigned volatile PREFIX1, 0x528);
    _REGISTER(unsigned volatile TXADDRESS, 0x52c);
    _REGISTER(unsigned volatile RXADDRESSES, 0x530);
    _REGISTER(unsigned volatile CRCCNF, 0x534);
    _REGISTER(unsigned volatile CRCPOLY, 0x538);
    _REGISTER(unsigned volatile CRCINIT, 0x53c);
    _REGISTER(unsigned volatile TEST, 0x540);
    _REGISTER(unsigned volatile TIFS, 0x544);
    _REGISTER(unsigned volatile RSSISAMPLE, 0x548);
    _REGISTER(unsigned volatile STATE, 0x550);
    _REGISTER(unsigned volatile DATAWHITEIV, 0x554);
    _REGISTER(unsigned volatile BCC, 0x560);
    _REGISTER(unsigned volatile DAB[8], 0x600);
    _REGISTER(unsigned volatile DAP[8], 0x620);
    _REGISTER(unsigned volatile DACNF, 0x640);
    _REGISTER(unsigned volatile OVERRIDE[5], 0x724);
    _REGISTER(unsigned volatile POWER, 0xffc);
};

/* Interrupts */
#define RADIO_INT_READY 0
#define RADIO_INT_END 3
#define RADIO_INT_DISABLED 4

#define RADIO (* (_DEVSTRUCT _radio *) 0x40001000)


/* TIMERS: Timer 0 is 8/16/24/32 bit, Timers 1 and 2 are 8/16 bit. */

_DEVSTRUCT _timer {
/* Tasks */
    _REGISTER(unsigned volatile START, 0x000);
    _REGISTER(unsigned volatile STOP, 0x004);
    _REGISTER(unsigned volatile COUNT, 0x008);
    _REGISTER(unsigned volatile CLEAR, 0x00c);
    _REGISTER(unsigned volatile SHUTDOWN, 0x010);
    _REGISTER(unsigned volatile CAPTURE[4], 0x040);
/* Events */
    _REGISTER(unsigned volatile COMPARE[4], 0x140);
/* Registers */
    _REGISTER(unsigned volatile SHORTS, 0x200);
    _REGISTER(unsigned volatile INTENSET, 0x304);
    _REGISTER(unsigned volatile INTENCLR, 0x308);
    _REGISTER(unsigned volatile MODE, 0x504);
#define   TIMER_MODE_Timer 0
#define   TIMER_MODE_Counter 1
    _REGISTER(unsigned volatile BITMODE, 0x508);
#define   TIMER_BITMODE_16Bit 0
#define   TIMER_BITMODE_8Bit 1
#define   TIMER_BITMODE_24Bit 2
#define   TIMER_BITMODE_32Bit 3
    _REGISTER(unsigned volatile PRESCALER, 0x510);
    _REGISTER(unsigned volatile CC[4], 0x540);
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

#define TIMER0 (* (_DEVSTRUCT _timer *) 0x40008000)
#define TIMER1 (* (_DEVSTRUCT _timer *) 0x40009000)
#define TIMER2 (* (_DEVSTRUCT _timer *) 0x4000a000)

extern _DEVSTRUCT _timer * const TIMER[3];


/* Random Number Generator */
_DEVSTRUCT _rng {
/* Tasks */
    _REGISTER(unsigned volatile START, 0x000);
    _REGISTER(unsigned volatile STOP, 0x004);
/* Events */
    _REGISTER(unsigned volatile VALRDY, 0x100);
/* Registers */
    _REGISTER(unsigned volatile SHORTS, 0x200);
    _REGISTER(unsigned volatile INTEN, 0x300);
    _REGISTER(unsigned volatile INTENSET, 0x304);
    _REGISTER(unsigned volatile INTENCLR, 0x308);
    _REGISTER(unsigned volatile CONFIG, 0x504);
#define   RNG_CONFIG_DERCEN 0
    _REGISTER(unsigned volatile VALUE, 0x508);
};

/* Interrupts */
#define RNG_INT_VALRDY 0

#define RNG (* (_DEVSTRUCT _rng *) 0x4000d000)


/* Temperature sensor */
_DEVSTRUCT _temp {
/* Tasks */
    _REGISTER(unsigned volatile START, 0x000);
    _REGISTER(unsigned volatile STOP, 0x004);
/* Events */
    _REGISTER(unsigned volatile DATARDY, 0x100);
/* Registers */
    _REGISTER(unsigned volatile INTEN, 0x300);
    _REGISTER(unsigned volatile INTENSET, 0x304);
    _REGISTER(unsigned volatile INTENCLR, 0x308);
    _REGISTER(unsigned volatile VALUE, 0x508);
};

/* Interrupts */
#define TEMP_INT_DATARDY 0

#define TEMP (* (_DEVSTRUCT _temp *) 0x4000c000)


/* I2C */
_DEVSTRUCT _i2c {
/* Tasks */
    _REGISTER(unsigned volatile STARTRX, 0x000);
    _REGISTER(unsigned volatile STARTTX, 0x008);
    _REGISTER(unsigned volatile STOP, 0x014);
    _REGISTER(unsigned volatile SUSPEND, 0x01c);
    _REGISTER(unsigned volatile RESUME, 0x020);
/* Events */
    _REGISTER(unsigned volatile STOPPED, 0x104);
    _REGISTER(unsigned volatile RXDREADY, 0x108);
    _REGISTER(unsigned volatile TXDSENT, 0x11c);
    _REGISTER(unsigned volatile ERROR, 0x124);
    _REGISTER(unsigned volatile BB, 0x138);
    _REGISTER(unsigned volatile SUSPENDED, 0x148);
/* Registers */
    _REGISTER(unsigned volatile SHORTS, 0x200);
    _REGISTER(unsigned volatile INTEN, 0x300);
    _REGISTER(unsigned volatile INTENSET, 0x304);
    _REGISTER(unsigned volatile INTENCLR, 0x308);
    _REGISTER(unsigned volatile ERRORSRC, 0x4c4);
#define   I2C_ERRORSRC_OVERRUN 0
#define   I2C_ERRORSRC_ANACK 1
#define   I2C_ERRORSRC_DNACK 2
#define   I2C_ERRORSRC_All 0x7
    _REGISTER(unsigned volatile ENABLE, 0x500) ;
#define   I2C_ENABLE_Disabled 0
#define   I2C_ENABLE_Enabled 5
    _REGISTER(unsigned volatile PSELSCL, 0x508);
    _REGISTER(unsigned volatile PSELSDA, 0x50c);
    _REGISTER(unsigned volatile RXD, 0x518);
    _REGISTER(unsigned volatile TXD, 0x51c) ;
    _REGISTER(unsigned volatile FREQUENCY, 0x524);
#define   I2C_FREQUENCY_100kHz 0x01980000
    _REGISTER(unsigned volatile ADDRESS, 0x588);
    _REGISTER(unsigned volatile POWER, 0xffc);
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

#define I2C0 (* (_DEVSTRUCT _i2c *) 0x40003000)

extern _DEVSTRUCT _i2c * const I2C[1];


/* UART */
_DEVSTRUCT _uart {
/* Tasks */
    _REGISTER(unsigned volatile STARTRX, 0x000);
    _REGISTER(unsigned volatile STARTTX, 0x008);
/* Events */
    _REGISTER(unsigned volatile RXDRDY, 0x108);
    _REGISTER(unsigned volatile TXDRDY, 0x11c);
/* Registers */
    _REGISTER(unsigned volatile INTENSET, 0x304);
    _REGISTER(unsigned volatile INTENCLR, 0x308);
    _REGISTER(unsigned volatile ENABLE, 0x500);
#define   UART_ENABLE_Disabled 0
#define   UART_ENABLE_Enabled 4
    _REGISTER(unsigned volatile PSELTXD, 0x50c);
    _REGISTER(unsigned volatile PSELRXD, 0x514);
    _REGISTER(unsigned volatile RXD, 0x518);
    _REGISTER(unsigned volatile TXD, 0x51c);
    _REGISTER(unsigned volatile BAUDRATE, 0x524);
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
    _REGISTER(unsigned volatile CONFIG, 0x56c);
#define   UART_CONFIG_HWFC 0
#define   UART_CONFIG_PARITY 1, 3
#define     UART_PARITY_None 0
#define     UART_PARITY_Even 7
};

/* Interrupts */
#define   UART_INT_RXDRDY 2
#define   UART_INT_TXDRDY 7

#define UART (* (_DEVSTRUCT _uart *) 0x40002000)


/* ADC */
_DEVSTRUCT _adc {
/* Tasks */
    _REGISTER(unsigned volatile START, 0x000);
    _REGISTER(unsigned volatile STOP, 0x004);
/* Events */
    _REGISTER(unsigned volatile END, 0x100);
/* Registers */
    _REGISTER(unsigned volatile INTEN, 0x300);
    _REGISTER(unsigned volatile INTENSET, 0x304);
    _REGISTER(unsigned volatile INTENCLR, 0x308);
    _REGISTER(unsigned volatile BUSY, 0x400);
    _REGISTER(unsigned volatile ENABLE, 0x500);
    _REGISTER(unsigned volatile CONFIG, 0x504);
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
    _REGISTER(unsigned volatile RESULT, 0x508);
};

/* Interrupts */
#define ADC_INT_END 0

#define ADC (* (_DEVSTRUCT _adc *) 0x40007000)

    
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
