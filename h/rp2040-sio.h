/* rp2040-sio.h - header file for RP2040 SIO block
 *
 * (c) David Haworth
 *
 *  This file is part of rp2040-bare-metal.
 *
 *  rp2040-bare-metal is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  rp2040-bare-metal is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with rp2040-bare-metal.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef RP2040_SIO_H
#define RP2040_SIO_H		1

#include "rp2040-types.h"

/* RP2040 SIO - single-cycle I/O block
 *
 * This is where the software control of the pins states take place.
 * The function selection for the pin has to be set to DV_FUNCSEL_SIO
 *
 * This block contains other features such as the core ID, inter-core FIFOs, spinlocks
 * and some arithmetical units.
*/
#define RP2040_SIO_BASE		0xd0000000

typedef struct rp2040_gpioblk_s
{
	volatile u32_t val;
	volatile u32_t w1s;
	volatile u32_t w1c;
	volatile u32_t xor;
} rp2040_gpioblk_t;

typedef struct rp2040_interp_s
{
	volatile u32_t accum0;				/* 0x00 - */
	volatile u32_t accum1;				/* 0x04 - */
	volatile u32_t base0;				/* 0x08 - */
	volatile u32_t base1;				/* 0x0c - */
	volatile u32_t base2;				/* 0x10 - */
	volatile u32_t pop_lane0;			/* 0x14 - */
	volatile u32_t pop_lane1;			/* 0x18 - */
	volatile u32_t pop_full;			/* 0x1c - */
	volatile u32_t peek_lane0;			/* 0x20 - */
	volatile u32_t peek_lane1;			/* 0x24 - */
	volatile u32_t peek_full;			/* 0x28 - */
	volatile u32_t ctrl_lane0;			/* 0x2c - */
	volatile u32_t ctrl_lane1;			/* 0x30 - */
	volatile u32_t accum0_add;			/* 0x34 - */
	volatile u32_t accum1_add;			/* 0x38 - */
	volatile u32_t base_1and0;			/* 0x3c - */
} rp2040_interp_t;

typedef struct rp2040_sio_s
{
	volatile u32_t		cpuid;			/* 0x000 - Core ID register */
	volatile u32_t		gpio_in;		/* 0x004 - GPIO 0..29 input state */
	volatile u32_t		gpio_hi_in;		/* 0x008 - QSPI GPIO input state */
	volatile u32_t		gap1;
	rp2040_gpioblk_t	gpio_out;		/* 0x010 - GPIO 0..29 output state */
	rp2040_gpioblk_t	gpio_oe;		/* 0x020 - GPIO 0..29 output enable (1 = output, 0 = input) */
	rp2040_gpioblk_t	gpio_hi_out;	/* 0x030 - QSPI GPIO output state */
	rp2040_gpioblk_t	gpio_hi_oe;		/* 0x040 - QSPI GPIO output enable (1 = output, 0 = input) */
	volatile u32_t		fifo_st;		/* 0x050 - FIFO status */
	volatile u32_t		fifo_wr;		/* 0x054 - FIFO write */
	volatile u32_t		fifo_rd;		/* 0x058 - FIFO read */
	volatile u32_t		div_udividend;	/* 0x060 - unsigned dividend */
	volatile u32_t		div_udivisor;	/* 0x064 - unsigned divisor */
	volatile u32_t		div_sdividend;	/* 0x068 - signed dividend */
	volatile u32_t		div_sdivisor;	/* 0x06c - signed divisor */
	volatile u32_t		div_quotient;	/* 0x070 - quotient */
	volatile u32_t		div_remainder;	/* 0x074 - remainder */
	volatile u32_t		div_csr;		/* 0x078 - divider control/status */
	volatile u32_t		gap2;
	rp2040_interp_t		interp[2];		/* 0x080 - interpolators */
	volatile u32_t		spinlock[32];	/* 0x100 - spinlocks */
} rp2040_sio_t;

#endif

