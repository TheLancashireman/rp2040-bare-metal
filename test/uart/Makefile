# Makefile for rp2040-bare-metal uart-test
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

.PHONY:		default

default:	build build/uart-test.elf

OBJS	+=	build/rp2040-vectors.o
OBJS	+=	build/rp2040-boot.o
OBJS	+=	build/rp2040-ctxsw.o
OBJS	+=	build/rp2040-startup.o
OBJS	+=	build/rp2040-clocks.o
OBJS	+=	build/rp2040-uart.o
OBJS	+=	build/uart-test.o

VPATH 	+= 	.
VPATH 	+= 	../../c
VPATH	+=	../../s

LDSCRIPT	=	../../ld/rp2040-ram.ldscript

build:
	mkdir build

CC_OPT	+=	-mcpu=cortex-m0plus
CC_OPT	+=	-mthumb
CC_OPT	+=	-I ../../h

# rp2040-bare-metal.a target just compiles all the source files
build/uart-test.elf:	$(OBJS)
	/usr/bin/arm-none-eabi-ld -o $@ $(OBJS) -T $(LDSCRIPT)

build/%.o:	%.c
	/usr/bin/arm-none-eabi-gcc $(CC_OPT) -o $@ -c $<
	
build/%.o:	%.S
	/usr/bin/arm-none-eabi-gcc $(CC_OPT) -o $@ -c $<

