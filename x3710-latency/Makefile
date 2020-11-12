# x3710-latency/Makefile
# Copyright (c) 2020 J. M. Spivey

all: latency.hex

CC = arm-none-eabi-gcc
CPU = -mcpu=cortex-m0 -mthumb
CFLAGS = -O -g -Wall -ffreestanding
INCLUDE = -I ../microbian
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
SIZE = arm-none-eabi-size
OBJCOPY = arm-none-eabi-objcopy

vpath %.h ../microbian

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o: %.s
	$(AS) $(CPU) $< -o $@

%.elf: %.o ../microbian/microbian.a
	$(CC) $(CPU) $(CFLAGS) -T ../microbian/nRF51822.ld \
		$^ -nostdlib -lgcc -lc -o $@ -Wl,-Map,$*.map
	$(SIZE) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

../microbian/microbian.a:
	$(MAKE) -C $(@D) $(@F)

# Nuke the default rules for building executables
SORRY = echo "Please say 'make $@.hex' to compile '$@'"
%: %.s; @$(SORRY)
%: %.o; @$(SORRY)

clean:
	rm -f *.hex *.elf *.map *.o

# Don't delete intermediate files
.SECONDARY:

###

latency.o: hardware.h microbian.h
