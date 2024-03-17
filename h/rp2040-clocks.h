/* rp2040-clocks.h - header file for RP2040 clocks and PLL peripherals
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
#ifndef RP2040_CLOCKS_H
#define RP2040_CLOCKS_H		1

#include "rp2040.h"
#include "rp2040-types.h"

typedef struct rp2040_clkblk_s rp2040_clkblk_t;
typedef struct rp2040_clocks_s rp2040_clocks_t;
typedef struct rp2040_xosc_s rp2040_xosc_t;
typedef struct rp2040_pll_s rp2040_pll_t;

/* Clocks
*/
struct rp2040_clkblk_s
{
	reg32_t ctrl;
	reg32_t div;
	reg32_t selected;
};

struct rp2040_clocks_s
{
	rp2040_clkblk_t	gpout[4];	/* 0x00/0x0c/0x18/0x24 */
	rp2040_clkblk_t	ref;		/* 0x30 - */
	rp2040_clkblk_t	sys;		/* 0x3c - */
	rp2040_clkblk_t	peri;		/* 0x48 - no DIV in this block? */
	rp2040_clkblk_t	usb;		/* 0x54 - */
	rp2040_clkblk_t	adc;		/* 0x60 - */
	rp2040_clkblk_t	rtc;		/* 0x6c - */
	reg32_t	sys_resus_ctrl;		/* 0x74 - */
	reg32_t	sys_resus_status;	/* 0x7c - */
	reg32_t	fc0_ref_khz;		/* 0x80 - */
	reg32_t	fc0_min_khz;		/* 0x84 - */
	reg32_t	fc0_max_khz;		/* 0x88 - */
	reg32_t	fc0_delay;			/* 0x8c - */
	reg32_t	fc0_interval;		/* 0x90 - */
	reg32_t	fc0_src;			/* 0x94 - */
	reg32_t	fc0_status;			/* 0x98 - */
	reg32_t	fc0_result;			/* 0x9c - */
	reg32_t	wake_en[2];			/* 0xa0 - */
	reg32_t	sleep_en[2];		/* 0xa8 - */
	reg32_t	enabled[2];			/* 0xb0 - */
	rp2040_intcs_t intcs;		/* 0xb8 - Interrupt control/status (4 registers) */
};

#define CLOCKS_BASE			0x40008000
#define rp2040_clocks		(((rp2040_clocks_t *)(CLOCKS_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_clocks_xor	(((rp2040_clocks_t *)(CLOCKS_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_clocks_w1s	(((rp2040_clocks_t *)(CLOCKS_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_clocks_w1c	(((rp2040_clocks_t *)(CLOCKS_BASE+RP2040_OFFSET_W1C))[0])

#define CLK_ENABLE			0x00000800
#define CLK_KILL			0x00000400
#define CLK_DIVBY1			0x00000100

#define CLKSRC_REF_ROSC		0x00
#define CLKSRC_REF_AUX		0x01
#define CLKSRC_REF_XOSC		0x02
#define CLKSRC_SYS_REF		0x00
#define CLKSRC_SYS_AUX		0x01
#define CLKSRC_SYS_AUX_PLL	0x00
#define CLKSRC_SYS_AUX_UPLL	0x20
#define CLKSRC_SYS_AUX_ROSC	0x40
#define CLKSRC_SYS_AUX_XOSC	0x60
#define CLKSRC_PERI_SYS		0x00
#define CLKSRC_PERI_PLL		0x20
#define CLKSRC_PERI_UPLL	0x40
#define CLKSRC_PERI_ROSC	0x60
#define CLKSRC_PERI_XOSC	0x80
#define CLKSRC_USB_SYS		0x00
#define CLKSRC_USB_PLL_SYS	0x20
#define CLKSRC_USB_PLL_USB	0x40
#define CLKSRC_USB_ROSC_PH	0x60
#define CLKSRC_USB_XOSC		0x80
#define CLKSRC_USB_GPIN0	0xa0
#define CLKSRC_USB_GPIN1	0xc0

/* Xosc
*/
struct rp2040_xosc_s
{
	reg32_t	ctrl;
	reg32_t	status;
	reg32_t	dormant;
	reg32_t	startup;
	reg32_t gap[3];
	reg32_t	count;
};

#define XOSC_BASE			0x40024000
#define rp2040_xosc			(((rp2040_xosc_t *)(XOSC_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_xosc_xor		(((rp2040_xosc_t *)(XOSC_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_xosc_w1s		(((rp2040_xosc_t *)(XOSC_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_xosc_w1c		(((rp2040_xosc_t *)(XOSC_BASE+RP2040_OFFSET_W1C))[0])

#define XOSC_1_15_MHZ		0x00000aa0
#define XOSC_ENABLE			0x00fab000
#define XOSC_DISABLE		0x00d1e000
#define XOSC_STARTUP_X4		0x00100000
#define XOSC_STABLE			0x80000000
#define XOSC_BADWRITE		0x01000000
#define XOSC_ENABLED		0x00001000

/* PLL
 * The sys PLL can be used to drive the cores. For full speed we need to wind it up to 133 MHz
 *
 * From the block diagram and description in refman 2.18:
 *	REFDIV = 1 because FREF is 12 MHz.
 *	VCO output must be in range 400..1600 MHz. The higher the frequency, the less jitter there is.
 *	FBDIV must be in the range 16..320. This is actually the multiplication factor.
 *
 * So if we choose FBDIV = 133, we have a VCO output frequeny of 1596, which is in range (just!)
 * Then we need to divide by 12 by setting POSTDIV1 to 6 and POSTDIV2 to 2
*/
struct rp2040_pll_s
{
	reg32_t	cs;			/* 0x00 - control and status */
	reg32_t	pwr;		/* 0x04 - controls power modes */
	reg32_t	fbdiv_int;	/* 0x08 - feedback divisor */
	reg32_t	prim;		/* 0x0c - controls PLL post dividers */
};

#define PLL_SYS_BASE		0x40028000
#define rp2040_pll			(((rp2040_pll_t *)(PLL_SYS_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_pll_xor		(((rp2040_pll_t *)(PLL_SYS_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_pll_w1s		(((rp2040_pll_t *)(PLL_SYS_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_pll_w1c		(((rp2040_pll_t *)(PLL_SYS_BASE+RP2040_OFFSET_W1C))[0])

#define PLL_USB_BASE		0x4002c000
#define rp2040_usbpll		(((rp2040_pll_t *)(PLL_USB_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_usbpll_xor	(((rp2040_pll_t *)(PLL_USB_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_usbpll_w1s	(((rp2040_pll_t *)(PLL_USB_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_usbpll_w1c	(((rp2040_pll_t *)(PLL_USB_BASE+RP2040_OFFSET_W1C))[0])

#define PLL_LOCK			0x80000000	/* CS */
#define PLL_BYPASS			0x00000100	/* CS */
#define PLL_REFDIV			0x0000003f	/* CS */
#define PLL_VCOPD			0x00000020	/* PWR */
#define PLL_POSTDIVPD		0x00000008	/* PWR */
#define PLL_DSMPD			0x00000004	/* PWR */
#define PLL_PD				0x00000001	/* PWR */
#define PLL_FBDIV			0x00000fff	/* FBDIV_INT */
#define PLL_POSTDIV1		0x00070000	/* PRIM */
#define PLL_POSTDIV2		0x00007000	/* PRIM */

extern void rp2040_clock_init(void);
extern void rp2040_pll_init(void);
extern void rp2040_usbpll_init(void);

#endif
