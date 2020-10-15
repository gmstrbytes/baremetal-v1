// common/startup.c
// Copyright (c) 2018 J. M. Spivey

#include "hardware.h"

/* init -- main program, creates application processes */
void init(void);

#ifdef MICROBE
/* os_init -- hook to set up scheduler data structures */
void os_init(void);

/* os_start -- hook to start process execution */
void os_start(void);

/* os_interrupt -- general interrupt handler */
void os_interrupt(int irq);
#endif

/* Addresses set by the linker */
extern unsigned char __data_start[], __data_end[],
     __bss_start[], __bss_end[], __etext[], __stack[];

/* The next four routines can be used in C compiler output, even
   if not mentioned in the source. */

/* memcpy -- copy n bytes from src to dest (non-overlapping) */
void *memcpy(void *dest, const void *src, unsigned n) {
    unsigned char *p = dest;
    const unsigned char *q = src;
    while (n-- > 0) *p++ = *q++;
    return dest;
}

/* memmove -- copy n bytes from src to dest, allowing overlaps */
void *memmove(void *dest, const void *src, unsigned n) {
    unsigned char *p = dest;
    const unsigned char *q = src;
    if (dest <= src)
        while (n-- > 0) *p++ = *q++;
    else {
        p += n; q += n;
        while (n-- > 0) *--p = *--q;
    }
    return dest;
}
    
/* memset -- set n bytes of dest to byte x */
void *memset(void *dest, unsigned x, unsigned n) {
    unsigned char *p = dest;
    while (n-- > 0) *p++ = x;
    return dest;
}

/* memcmp -- compare n bytes */
int memcmp(const void *pp, const void *qq, int n) {
    const unsigned char *p = pp, *q = qq;
    while (n-- > 0) {
        if (*p++ != *q++)
            return (p[-1] < q[-1] ? -1 : 1);
    }
    return 0;
}

/* __reset -- the system starts here */
void __reset(void) {
     // Make sure all RAM banks are powered on.
     POWER_RAMON |= BIT(0) | BIT(1);

     // Activate the crystal clock
     CLOCK_XTALFREQ = CLOCK_XTALFREQ_16MHz;
     CLOCK_HFCLKSTARTED = 0;
     CLOCK_HFCLKSTART = 1;
     while (! CLOCK_HFCLKSTARTED) { }

     // Copy data segment and zero out bss.
     memcpy(__data_start, __etext, __data_end - __data_start);
     memset(__bss_start, 0, __bss_end - __bss_start);

#ifdef MICROBE
     os_init();                 // Initialise the scheduler.
     init();                    // Let the program initialise itself.
     os_start();                // Start the scheduler -- never returns.
#else
     init();                    // Call the main program.
     while (1) pause();         // Halt if it returns.
#endif
}


/* NVIC SETUP FUNCTIONS */

/* On Cortex-M0, only the top two bits of each interrupt priority are
   implemented, but for portability priorities should be specified
   with integers in the range [0..255]. */

/* irq_priority -- set priority for an IRQ to a value [0..255] */
void irq_priority(int irq, unsigned prio) {
     if (irq < 0)
         SET_BYTE(SCB_SHPR[(irq+8) >> 2], irq & 0x3, prio);
     else
         SET_BYTE(NVIC_IPR[irq >> 2], irq & 0x3, prio);
}
     
// See hardware.h for macros enable_irq, disable_irq, clear_pending, reschedule


/*  INTERRUPT VECTORS */

/* We use GCC features to define each handler name as an alias for the
   spin() function if it is not defined elsewhere, or for the general
   interrupt handler of Phos.  Applications can subsitute their own
   definitions for individual handler names like uart_handler(). */

/* spin -- show Seven Stars of Death */
void spin(void) {
     int n;

     intr_disable();

     GPIO_DIR = 0xfff0;
     while (1) {
          GPIO_OUT = 0x4000;
          for (n = 1000000; n > 0; n--) {
               nop(); nop(); nop();
          }
          GPIO_OUT = 0;
          for (n = 200000; n > 0; n--) {
               nop(); nop(); nop();
          }
     }          
}

void default_handler(void);

#ifndef MICROBE
void default_handler(void) {
    spin();
}
#endif

// The linker script makes all these handlers into weak aliases for
// default_handler.

#define HANDLER(name)  void name(void)

HANDLER(nmi_handler);
HANDLER(hardfault_handler);
HANDLER(svc_handler);
HANDLER(pendsv_handler);
HANDLER(systick_handler);
HANDLER(uart_handler);
HANDLER(timer0_handler);
HANDLER(timer1_handler);
HANDLER(timer2_handler);
HANDLER(power_clock_handler);
HANDLER(radio_handler);
HANDLER(spi0_twi0_handler);
HANDLER(spi1_twi1_handler);
HANDLER(gpiote_handler);
HANDLER(adc_handler);
HANDLER(rtc0_handler);
HANDLER(temp_handler);
HANDLER(rng_handler);
HANDLER(ecb_handler);
HANDLER(ccm_aar_handler);
HANDLER(wdt_handler);
HANDLER(rtc1_handler);
HANDLER(qdec_handler);
HANDLER(lpcomp_handler);
HANDLER(swi0_handler);
HANDLER(swi1_handler);
HANDLER(swi2_handler);
HANDLER(swi3_handler);
HANDLER(swi4_handler);
HANDLER(swi5_handler);

// This vector table is placed at address 0 in the flash by directives
// in the linker script.  Entries directly filled with default_handler
// are unused by the hardware.  Getting such an interrupt would be a
// real surprise!

void *__vectors[] __attribute((section(".vectors"))) = {
    __stack,                    // -16
    __reset,
    nmi_handler,
    hardfault_handler,
    default_handler,            // -12
    default_handler,
    default_handler,
    default_handler,
    default_handler,            //  -8
    default_handler,
    default_handler,
    svc_handler,
    default_handler,            // -4
    default_handler,
    pendsv_handler,
    systick_handler,
    
    /* external interrupts */
    power_clock_handler,        //  0
    radio_handler,
    uart_handler,
    spi0_twi0_handler,
    spi1_twi1_handler,          //  4
    default_handler,
    gpiote_handler,
    adc_handler,
    timer0_handler,             //  8
    timer1_handler,
    timer2_handler,
    rtc0_handler,
    temp_handler,               // 12
    rng_handler,
    ecb_handler,
    ccm_aar_handler,
    wdt_handler,                // 16
    rtc1_handler,
    qdec_handler,
    lpcomp_handler,
    swi0_handler,               // 20
    swi1_handler,
    swi2_handler,
    swi3_handler,
    swi4_handler,               // 24
    swi5_handler,
    default_handler,
    default_handler,
    default_handler,            // 28
    default_handler,
    default_handler,
    default_handler
};
