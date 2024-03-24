# Makefile for rp2040-bare-metal w1c-test
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

.PHONY:		default upload

default:	build/w1c-test.uf2

OBJS	+=	build/rp2040-vectors.o
OBJS	+=	build/rp2040-boot.o
OBJS	+=	build/rp2040-ctxsw.o
OBJS	+=	build/rp2040-startup.o
OBJS	+=	build/rp2040-clocks.o
OBJS	+=	build/rp2040-uart.o
OBJS	+=	build/w1c-test.o
OBJS	+=	build/test-io.o

VPATH 	+= 	.
VPATH 	+= 	../../c
VPATH	+=	../../s
VPATH	+=	../common

LDSCRIPT	=	../../ld/rp2040-ram.ldscript

CC_OPT	+=	-mcpu=cortex-m0plus
CC_OPT	+=	-mthumb
CC_OPT	+=	-I ../../h
CC_OPT	+=	-I ../common
CC_OPT	+=	-Wall

build/w1c-test.uf2:	build/w1c-test.elf
	elf2uf2 -v $< $@

build/w1c-test.elf:	build $(OBJS) $(LDSCRIPT)
	/usr/bin/arm-none-eabi-ld -o $@ $(OBJS) -T $(LDSCRIPT) -e 'rp2040_entry'

build/%.o:	%.c
	/usr/bin/arm-none-eabi-gcc $(CC_OPT) -o $@ -c $<
	
build/%.o:	%.S
	/usr/bin/arm-none-eabi-gcc $(CC_OPT) -o $@ -c $<

build:
	mkdir build

upload:		build/w1c-test.uf2
	../../sh/to-pico.sh $<
