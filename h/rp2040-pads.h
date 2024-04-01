/* rp2040-pads.h - header file for RP2040 pads
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
#ifndef RP2040_PADS_H
#define RP2040_PADS_H

#include "rp2040.h"
#include "rp2040-types.h"

/* I/O user bank 0 pads
 *
 * This peripheral controls the input and output drives for each pin
*/
typedef struct rp2040_pads_bank0_s rp2040_pads_bank0_t;

/* IOBANK0 pads
*/
struct rp2040_pads_bank0_s
{
	reg32_t	voltage;	/* 0x00 - 0 = 3.3v, 1 = 1.8v */
	reg32_t	gpio[30];	/* 0x04 - GPIO I/O drive control */
	reg32_t	swclk;		/* 0x7c - SW clk I/O drive control */
	reg32_t	swd;		/* 0x80 - SW data I/O drive control */
};

#define PADBANK0_BASE			0x4001c000
#define rp2040_pads_bank0		(((rp2040_pads_bank0_t *)(PADBANK0_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_pads_bank0_xor	(((rp2040_pads_bank0_t *)(PADBANK0_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_pads_bank0_w1s	(((rp2040_pads_bank0_t *)(PADBANK0_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_pads_bank0_w1c	(((rp2040_pads_bank0_t *)(PADBANK0_BASE+RP2040_OFFSET_W1C))[0])

#define VOLT_33			0x01	/* DVDD >= 2.5 V */
#define VOLT_18			0x01	/* DVDD <= 1.8 V */

/* The following apply to gpio[x], swclk and swd
*/
#define PADS_OD			0x80	/* Output disable (1 to disable) */
#define PADS_IE			0x40	/* Inut enable */
#define PADS_DRIVE		0x30	/* Drive strength */
#define PADS_DRIVE_2mA	0x00
#define PADS_DRIVE_4mA	0x10
#define PADS_DRIVE_8mA	0x20
#define PADS_DRIVE_12mA	0x30
#define PADS_PUE		0x08	/* Pull-up enable */
#define PADS_PDE		0x04	/* Pull-down enable */
#define PADS_SCHMITT	0x02	/* Schmitt trigger enable */
#define PADS_SLEWFAST	0x01	/* Slew rate: 1 = fast, 0 = slow */

#endif
