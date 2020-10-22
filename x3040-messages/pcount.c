// x3040/pcount.c
// Copyright (c) 2018-20 J. M. Spivey

#include "microbian.h"
#include "lib.h"
#include <string.h>

int PRODUCE, CONSUME;

#define PRIME 42

/* prime -- test for primality */
int prime(int n) {
    for (int k = 2; k * k <= n; k++) {
        if (n % k == 0)
            return 0;
    }

    return 1;
}

void producer(int arg) {
    int n = 2;
    message m;

    while (1) {
        if (prime(n)) {
            m.m_i1 = n;
            send(CONSUME, PRIME, &m);
        }
        n++;
    }
}

void consumer(int arg) {
    int count = 0, limit = 1000;
    message m;

    while (1) {
        receive(PRIME, &m);
        while (m.m_i1 >= limit) {
            printf("There are %d primes less than %d\n", count, limit);
            limit += 1000;
        }
        count++;
    }
}

void init(void) {
    serial_init();
    PRODUCE = start("Produce", producer, 0, STACK);
    CONSUME = start("Consume", consumer, 0, STACK);
}
