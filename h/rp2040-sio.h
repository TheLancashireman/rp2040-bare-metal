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
#define RP2040_SIO_H	1

#include "rp2040-types.h"

/* RP2040 SIO - single-cycle I/O block
 *
 * This is where the software control of the pins states take place.
 * The function selection for the pin has to be set to DV_FUNCSEL_SIO
 *
 * This block contains other features such as the core ID, inter-core FIFOs, spinlocks
 * and some arithmetical units.
*/
typedef struct rp2040_gpioblk_s
{
	reg32_t val;
	reg32_t w1s;
	reg32_t w1c;
	reg32_t xor;
} rp2040_gpioblk_t;

typedef struct rp2040_interp_s
{
	reg32_t accum0;			/* 0x00 - */
	reg32_t accum1;			/* 0x04 - */
	reg32_t base0;			/* 0x08 - */
	reg32_t base1;			/* 0x0c - */
	reg32_t base2;			/* 0x10 - */
	reg32_t pop_lane0;		/* 0x14 - */
	reg32_t pop_lane1;		/* 0x18 - */
	reg32_t pop_full;		/* 0x1c - */
	reg32_t peek_lane0;		/* 0x20 - */
	reg32_t peek_lane1;		/* 0x24 - */
	reg32_t peek_full;		/* 0x28 - */
	reg32_t ctrl_lane0;		/* 0x2c - */
	reg32_t ctrl_lane1;		/* 0x30 - */
	reg32_t accum0_add;		/* 0x34 - */
	reg32_t accum1_add;		/* 0x38 - */
	reg32_t base_1and0;		/* 0x3c - */
} rp2040_interp_t;

typedef struct rp2040_sio_s
{
	reg32_t cpuid;					/* 0x000 - Core ID register */
	reg32_t gpio_in;				/* 0x004 - GPIO 0..29 input state */
	reg32_t gpio_hi_in;				/* 0x008 - QSPI GPIO input state */
	reg32_t gap1;
	rp2040_gpioblk_t gpio_out;		/* 0x010 - GPIO 0..29 output state */
	rp2040_gpioblk_t gpio_oe;		/* 0x020 - GPIO 0..29 output enable (1 = output, 0 = input) */
	rp2040_gpioblk_t gpio_hi_out;	/* 0x030 - QSPI GPIO output state */
	rp2040_gpioblk_t gpio_hi_oe;	/* 0x040 - QSPI GPIO output enable (1 = output, 0 = input) */
	reg32_t fifo_st;				/* 0x050 - FIFO status */
	reg32_t fifo_wr;				/* 0x054 - FIFO write */
	reg32_t fifo_rd;				/* 0x058 - FIFO read */
	reg32_t spinlock_st;			/* 0x05c - Spinlock status */
	reg32_t div_udividend;			/* 0x060 - unsigned dividend */
	reg32_t div_udivisor;			/* 0x064 - unsigned divisor */
	reg32_t div_sdividend;			/* 0x068 - signed dividend */
	reg32_t div_sdivisor;			/* 0x06c - signed divisor */
	reg32_t div_quotient;			/* 0x070 - quotient */
	reg32_t div_remainder;			/* 0x074 - remainder */
	reg32_t div_csr;				/* 0x078 - divider control/status */
	reg32_t gap2;
	rp2040_interp_t interp[2];		/* 0x080 - interpolators */
	reg32_t spinlock[32];			/* 0x100 - spinlocks */
} rp2040_sio_t;

/* SIO doesn't have mirror blocks for atomic access
*/
#define SIO_BASE			0xd0000000
#define rp2040_sio			(((rp2040_sio_t *)SIO_BASE)[0])

#endif

