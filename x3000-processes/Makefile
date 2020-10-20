# x3000-processes/Makefile
# Copyright (c) 2020 J. M. Spivey

all: valentine.hex

CC = arm-none-eabi-gcc
CPU = -mcpu=cortex-m0 -mthumb
CFLAGS = -O -g -Wall -ffreestanding -I ../microbe
SIZE = arm-none-eabi-size
OBJCOPY = arm-none-eabi-objcopy

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) -c $< -o $@

%.elf: %.o ../microbe/microbe.a
	$(CC) $(CPU) $(CFLAGS) -T ../microbe/NRF51822.ld \
		$^ -nostdlib -lgcc -lc -o $@ -Wl,-Map,$*.map
	$(SIZE) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

../microbe/microbe.a:
	$(MAKE) -C $(@D) $(@F)

# Nuke the default rules for building executables
SORRY = echo "Please say 'make $@.hex' to compile the '$@' program"
%: %.s; @$(SORRY)
%: %.o; @$(SORRY)

clean:
	rm -f *.hex *.elf *.map *.o test-*

# Don't delete intermediate files
.SECONDARY:

###

valentine.o: ../microbe/hardware.h ../microbe/lib.h ../microbe/microbe.h
