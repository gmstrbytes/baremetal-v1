# x2400-neopixels/Makefile
# Copyright (c) 2020 J. M. Spivey

all: rainbow.hex

CC = arm-none-eabi-gcc
CPU = -mcpu=cortex-m0 -mthumb
CFLAGS = -O -g -Wall -ffreestanding
AS = arm-none-eabi-as
SIZE = arm-none-eabi-size
OBJCOPY = arm-none-eabi-objcopy

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(CPU) $< -o $@

%.elf: %.o neopixel.o startup.o
	$(CC) $(CPU) $(CFLAGS) -T NRF51822.ld \
		$^ -nostdlib -lgcc -o $@ -Wl,-Map,$*.map
	$(SIZE) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

# Nuke the default rules for building executables
SORRY = echo "Please say 'make $@.hex' to compile the '$@' program"
%: %.s; @$(SORRY)
%: %.o; @$(SORRY)

clean:
	rm -f *.hex *.elf *.map *.o test-*

# Don't delete intermediate files
.SECONDARY:

###

rainbow.o: hardware.h
