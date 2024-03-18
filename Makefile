# Makefile for rp2040-bare-metal
#
# In order to use this repository, there's nothing to make.
# This Makfile does the compile tests
#
# (c) David Haworth
#
#  This file is part of rp2040-bare-metal.
#
#  rp2040-bare-metal is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  rp2040-bare-metal is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with rp2040-bare-metal.  If not, see <http://www.gnu.org/licenses/>.


# Description of targets:
#	test:			runs header-test and compile-test
#	header-test:	builds and runs a host-based program to check the structure offsets in the header files
#	compile-test:	compiles source files from the c and s directories and creates a library
# Note: none of the above builds anything that runs on an RP2040 target board.

.PHONY:			test header-test compile-test

test:			build header-test compile-test

build:
	mkdir -p build

header-test:	build build/header-test
	build/header-test

compile-test:	build build/rp2040-bare-metal.a

OBJS	+=	build/rp2040-boot.o
OBJS	+=	build/rp2040-startup.o
OBJS	+=	build/rp2040-clocks.o
OBJS	+=	build/rp2040-uart.o

VPATH	+=	s
VPATH	+=	c

CC_OPT	+=	-mcpu=cortex-m0plus
CC_OPT	+=	-mthumb
CC_OPT	+=	-I h

# header-test runs on the host
build/header-test:	test/compile-test/header-test.c
	gcc -I h/ -o build/header-test test/compile-test/header-test.c

# rp2040-bare-metal.a target just compiles all the source files
build/rp2040-bare-metal.a:	$(OBJS)
	if [ -e build/rp2040-bare-metal.a ]; then rm build/rp2040-bare-metal.a; fi
	/usr/bin/arm-none-eabi-ar crs build/rp2040-bare-metal.a $(OBJS)

build/%.o:	%.c
	/usr/bin/arm-none-eabi-gcc $(CC_OPT) -o $@ -c $<
	
build/%.o:	%.S
	/usr/bin/arm-none-eabi-gcc $(CC_OPT) -o $@ -c $<

