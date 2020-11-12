#include "microbian.h"
#include "hardware.h"
#include "lib.h"

static const unsigned short blank[] = { 0, 0, 0 };

static volatile const unsigned short *image = blank;

void display_task(int dummy) {
    int n = 0;

    GPIO_DIRSET = 0xfff0;
    priority(P_HIGH);

    while (1) {
        GPIO_OUT = image[n];
        timer_delay(5);
        if (++n == 3) n = 0;
    }
}

void display_set(const unsigned short *img) {
    image = img;
}

void display_init(void) {
    start("Display", display_task, 0, STACK);
}

static const unsigned short letter_a[] =
    IMAGE(0,1,1,0,0,
          1,0,0,1,0,
          1,1,1,1,0,
          1,0,0,1,0,
          1,0,0,1,0);

static const unsigned short letter_b[] =
    IMAGE(1,1,1,0,0,
          1,0,0,1,0,
          1,1,1,0,0,
          1,0,0,1,0,
          1,1,1,0,0);

void receiver_task(int dummy) {
    byte buf[32];
    int n;

    printf("Hello\n");

    display_set(letter_a);
    timer_delay(1000);
    display_set(letter_b);
    timer_delay(1000);
    display_set(blank);

    while (1) {
        n = radio_receive(buf);
        if (n == 1 && buf[0] == '1') {
            printf("Button A\n");
            display_set(letter_a);
        } else if (n == 1 && buf[0] == '2') {
            printf("Button B\n");
            display_set(letter_b);
        } else {
            printf("Unknown packet, length %d\n", n);
        }
    }
}

void sender_task(int dummy) {
    GPIO_PINCNF[BUTTON_A] = 0;
    GPIO_PINCNF[BUTTON_B] = 0;

    while (1) {
        if ((GPIO_IN & BIT(BUTTON_A)) == 0) {
            printf("Press A\n");
            radio_send("1", 1);
        } else if ((GPIO_IN & BIT(BUTTON_B)) == 0) {
            printf("Press B\n");
            radio_send("2", 1);
        }

        timer_delay(100);
    }
}

void init(void) {
    serial_init();
    radio_init();
    timer_init();
    display_init();
    start("Receiver", receiver_task, 0, STACK);
    start("Sender", sender_task, 0, STACK);
}
