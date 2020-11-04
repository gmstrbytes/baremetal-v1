// bitbang.c
// Copyright (c) 2020 J. M. Spivey

#include "hardware.h"

/* Square wave outputs on pads 0 (hardware control) and 3 (bit-banged
   in software).  Meanwhile, the random number generator is causing
   interrupts at irregular intervals. */

/* rng_init -- initialise hardware random number generator */
void rng_init(void) {
    /* Setting the DERCEN bit enables the bias elimination algorithm,
       and makes the intervals between random bytes irregular. */
    SET_BIT(RNG_CONFIG, RNG_CONFIG_DERCEN);
    RNG_VALRDY = 0;
    RNG_INTENSET = BIT(RNG_INT_VALRDY);
    enable_irq(RNG_IRQ);
    RNG_START = 1;
}

/* rng_handler -- interrupt handler for random number generator */
void rng_handler(void) {
    if (RNG_VALRDY) {
        /* Just acknowledge the interrupt */
        RNG_VALRDY = 0;
    }
}

/* sqwave_init -- set up high-frequency square wave on pad 0 */
void sqwave_init(void) {
    /* Set up timer 2 to count at 16Mhz, and link it via the 'programmable
       peripheral interconnect' to a task that toggles the GPIO pin on each
       compare event. */

    // Set PAD0 as output
    GPIO_DIRSET = BIT(PAD0);

    // Set up channel 0 of the GPIO-tasks-and-events system to toggle the pin
    GPIOTE_CONFIG[0] =
        FIELD(GPIOTE_CONFIG_MODE, GPIOTE_MODE_Task)
        | FIELD(GPIOTE_CONFIG_PSEL, PAD0)
        | FIELD(GPIOTE_CONFIG_POLARITY, GPIOTE_POLARITY_Toggle);

    // Use channel 0 of the PPI to activate the GPIO task on each timer event
    PPI_CH[0].EEP = &TIMER2_COMPARE[0];
    PPI_CH[0].TEP = &GPIOTE_OUT[0];
    PPI_CHENSET = BIT(0);

    // Timer 2 counts repeatedly at 16MHz, resetting on each cycle
    TIMER2_MODE = TIMER_MODE_Timer;
    TIMER2_BITMODE = TIMER_BITMODE_16Bit;
    TIMER2_PRESCALER = 0;
    TIMER2_CLEAR = 1;
    TIMER2_CC[0] = 1;
    TIMER2_SHORTS = BIT(TIMER_COMPARE0_CLEAR);
    TIMER2_START = 1;
}

void init(void) {
    rng_init();
    sqwave_init();

    GPIO_DIR = 0xffff;

    /* Since pad 0 is under control of GPIOITE, it is disabled as an
       oridinary GPIO pin, and our assignments to GPIO_OUT do not
       affect it. */

    while (1) {
        GPIO_OUT = 0x5fb0;
        nop(); nop(); nop();
        GPIO_OUT = 0;
    }
}
