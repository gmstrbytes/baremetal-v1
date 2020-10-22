# x1100-instrs/Makefile
# Copyright (c) 2020 J. M. Spivey

all: func.hex

CC = arm-none-eabi-gcc
CPU = -mcpu=cortex-m0 -mthumb
CFLAGS = -O -g -Wall -ffreestanding
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
SIZE = arm-none-eabi-size
OBJCOPY = arm-none-eabi-objcopy

%.o: %.c
	$(CC) $(CPU) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(CPU) $< -o $@

# Use ld for linking to help with the explanation
%.elf: %.o fmain.o lib.o startup.o
	$(LD) -T NRF51822.ld $^ \
		$(LIBGCC) -o $@ -Map $*.map
	$(SIZE) $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

# Nuke the default rules for building executables
SORRY = echo "Please say 'make $@.hex' to compile the '$@' program"
%: %.s; @$(SORRY)
%: %.o; @$(SORRY)

# Find libgcc: find the first part of the gcc search path.
GCCLIB := $(shell $(CC) --print-search-dirs \
		| sed -n '/^libraries: =\([^:]*\)\/:.*/s//\1/p')
# Then try a couple of likely subdirectories for the library.
LIBGCC := $(word 1,$(wildcard \
		$(GCCLIB)/thumb/v6-m/libgcc.a \
		$(GCCLIB)/armv6-m/libgcc.a \
		$(GCCLIB)/thumb/v6-m/nofp/libgcc.a) UNKNOWN)

clean:
	rm -f *.hex *.elf *.map *.o test-*

# Don't delete intermediate files
.SECONDARY:

###

fmain.o: lib.h hardware.h
lib.o: lib.h
startup.o: hardware.h
