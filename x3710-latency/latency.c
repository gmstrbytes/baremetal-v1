#include "microbian.h"
#include "hardware.h"

int DRIVER;

void timer2_handler(void) {
    // Reset one of the GPIO pins
    GPIOTE_OUT[0] = 1;

    // Disable the interrupt
    disable_irq(TIMER2_IRQ);

    // Send INTERRUPT message to the driver
    interrupt(DRIVER);
}

void timer_task(int arg) {
    GPIO_DIRSET = 0xfff0;
    GPIO_OUT = 0x3ff0;

    // Set up timer 2 so that on overflow (once every millisecond)
    // it toggles two I/O pins and causes an interrupt
    TIMER2_STOP = 1;
    TIMER2_MODE = TIMER_MODE_Timer;
    TIMER2_BITMODE = TIMER_BITMODE_16Bit;
    TIMER2_PRESCALER = 0; // 16 MHz
    TIMER2_CLEAR = 1;
    TIMER2_CC[0] = 16000; // Period 1 ms
    TIMER2_SHORTS = BIT(TIMER_COMPARE0_CLEAR);
    TIMER2_INTENSET = BIT(TIMER_INT_COMPARE0);

    // PPI channels 0, 1, 2 activate three GPIO tasks on overflow
    PPI_CH[0].EEP = &TIMER2_COMPARE[0];
    PPI_CH[0].TEP = &GPIOTE_OUT[0];
    PPI_CH[1].EEP = &TIMER2_COMPARE[0];
    PPI_CH[1].TEP = &GPIOTE_OUT[1];
    PPI_CHENSET = BIT(0) | BIT(1);

    // GPIOTE channels 0 and 1 toggle two pins
    GPIOTE_CONFIG[0] =
        FIELD(GPIOTE_CONFIG_MODE, GPIOTE_MODE_Task)
        | FIELD(GPIOTE_CONFIG_PSEL, PAD3)
        | FIELD(GPIOTE_CONFIG_POLARITY, GPIOTE_POLARITY_Toggle)
        | FIELD(GPIOTE_CONFIG_OUTINIT, 1);
    GPIOTE_CONFIG[1] =
        FIELD(GPIOTE_CONFIG_MODE, GPIOTE_MODE_Task)
        | FIELD(GPIOTE_CONFIG_PSEL, PAD4)
        | FIELD(GPIOTE_CONFIG_POLARITY, GPIOTE_POLARITY_Toggle)
        | FIELD(GPIOTE_CONFIG_OUTINIT, 1);

    // Start your engines!
    connect(TIMER2_IRQ);
    TIMER2_START = 1;

    while (1) {
        receive(INTERRUPT, NULL);
        GPIOTE_OUT[1] = 1;
        TIMER2_COMPARE[0] = 0;
        clear_pending(TIMER2_IRQ);
        enable_irq(TIMER2_IRQ);
    }
}

void init(void) {
    DRIVER = start("Driver", timer_task, 0, STACK);
}
        
