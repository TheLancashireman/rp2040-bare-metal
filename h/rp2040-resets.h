/* rp2040-resets.h
 *
 * (c) David Haworth
 *
 *  This file is part of davros.
 *
 *  davros is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  davros is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with davros.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef RP2040_RESETS_H
#define RP2040_RESETS_H		1

#include "rp2040-types.h"
#include "rp2040.h"

/* The resets registers control the power to the peripherals.
 * A peripheral whose reset bit is 1 is held in reset.
*/
typedef struct rp2040_resets_s rp2040_resets_t;

struct rp2040_resets_s
{
	reg32_t	reset;
	reg32_t	wdsel;
	reg32_t	done;
} dv_rp2040_resets_t;

#define RESETS_BASE			0x4000c000
#define rp2040_resets		(((rp2040_resets_t *)(RESETS_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_resets_xor	(((rp2040_resets_t *)(RESETS_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_resets_w1s	(((rp2040_resets_t *)(RESETS_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_resets_w1c	(((rp2040_resets_t *)(RESETS_BASE+RP2040_OFFSET_W1C))[0])

/* Bits in the RESETS registers
*/
#define RESETS_usbctrl		0x01000000
#define RESETS_uart1		0x00800000
#define RESETS_uart0		0x00400000
#define RESETS_timer		0x00200000
#define RESETS_tbman		0x00100000
#define RESETS_sysinfo		0x00080000
#define RESETS_syscfg		0x00040000
#define RESETS_spi1			0x00020000
#define RESETS_spi0			0x00010000
#define RESETS_rtc			0x00008000
#define RESETS_pwm			0x00004000
#define RESETS_pll_usb		0x00002000
#define RESETS_pll_sys		0x00001000
#define RESETS_pio1			0x00000800
#define RESETS_pio0			0x00000400
#define RESETS_pads_qspi	0x00000200
#define RESETS_pads_bank0	0x00000100
#define RESETS_jtag			0x00000080
#define RESETS_io_qspi		0x00000040
#define RESETS_io_bank0		0x00000020
#define RESETS_i2c1			0x00000010
#define RESETS_i2c0			0x00000008
#define RESETS_dma			0x00000004
#define RESETS_busctrl		0x00000002
#define RESETS_adc			0x00000001

/* rp2040_release() - bring a peripheral out of reset
 *
 * The parameter must specify exactly one peripheral
*/
static inline void rp2040_release(u32_t peri)
{
	rp2040_resets_w1c.reset = peri;
   	do {    /* Wait */  } while ( (rp2040_resets.done & peri) == 0 );
}

/* rp2040_reset() - reset a peripheral
 *
 * The parameter must specify exactly one peripheral
*/
static inline void rp2040_reset(u32_t peri)
{
	rp2040_resets_w1s.reset = peri;				/* Put the peripheral into reset */
	rp2040_release(peri);						/* Bring it back out again */
}

#endif
