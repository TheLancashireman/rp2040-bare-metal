/* rp2040.h - header file common to rp2040 peripherals
 *
 * (c) David Haworth
 *
 *  This file is part of pico-bare-metal.
 *
 *  pico-bare-metal is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  pico-bare-metal is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with pico-bare-metal.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef RP2040_H
#define RP2040_H		1

#include "rp2040-types.h"
#include "rp2040.h"

/* Most peripherals have "mirror" addresses that allow atomic access.
 * Exceptions:
 *	I2C, UART, SPI and SSI use the same mirror address scheme but have a "bus interposer" that adds two cycles.
 *	SIO doesn't have mirror addresses. It has separate atomic access for some registers
*/
#define	RP2040_OFFSET_REG	0x0000		/* Plain register (i.e. read/write all bits) */
#define	RP2040_OFFSET_XOR	0x1000		/* XOR (i.e. toggle) */
#define	RP2040_OFFSET_W1S	0x2000		/* Write 1 to set */
#define	RP2040_OFFSET_W1C	0x3000		/* Write 1 to clear */

/* SYSINFO is a read-only "peripheral" that reports information about the RP2040 device
*/
typedef struct rp2040_sysinfo_s rp2040_sysinfo_t;

struct rp2040_sysinfo_s
{
	u32_t	chip_id;		/* 0x00 - JEDEC jep-106 compliant chip identifier */
	u32_t	platform;		/* 0x04 - platform (ASIC or FPGA) */
	u32_t	gap[14];
	u32_t	gitref_rp2040;	/* 0x40 - git hash of chip source */
};

#define RP2040_SYSINFO_BASE	0x40000000
#define rp2040_sysinfo		(((rp2040_sysinfo_t *)RP2040_SYSINFO_BASE)[0])

#define RP2040_MANUFACTURER	0x00000fff
#define RP2040_PART			0x0ffff000
#define RP2040_REVISION		0xf0000000

/* Some peripherals have a set of four registers to control their interrupts.
 * If they are in the same order we can use this struct.
*/
typedef struct rp2040_intcs_s rp2040_intcs_t;

struct rp2040_intcs_s
{
	reg32_t intr;	/* 0x00 Raw interrupt status */
	reg32_t inte;	/* 0x04 Interrupt enable */
	reg32_t intf;	/* 0x08 Interrupt force */
	reg32_t ints;	/* 0x0c Interrupt status after mask and force */
};


#endif
