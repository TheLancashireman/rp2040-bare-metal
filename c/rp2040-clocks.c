/* rp2040-clocks.c - header file for RP2040 clocks, oscillators and PLLs
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
#include "rp2040.h"
#include "rp2040-types.h"
#include "rp2040-clocks.h"
#include "rp2040-resets.h"

/* ToDo: allow these to be configured externally
*/
#define APP_REFDIV		1		/* VCO input at 12 MHz */
#define APP_FBDIV		133		/* VCO at 1596 MHz */
#define APP_POSTDIV1	6		/* Div1 output at 266 MHz */
#define APP_POSTDIV2	2		/* Div2 output at 133 MHz */
#define APP_POSTDIVS	((APP_POSTDIV1 << 16) | (APP_POSTDIV2 << 12))

#define APP_USB_REFDIV		APP_REFDIV			
#define APP_USB_FBDIV		120		/* VCO at 1440 MHz */
#define APP_USB_POSTDIV1	6		/* Div1 output at 240 MHz */
#define APP_USB_POSTDIV2	5		/* Div2 output at 48 MHz */
#define APP_USB_POSTDIVS	((APP_USB_POSTDIV1 << 16) | (APP_USB_POSTDIV2 << 12))


void rp2040_clock_init(void)
{
	rp2040_clocks.sys_resus_ctrl = 0x00;				/* Disable resuscitation for now */

	rp2040_xosc.ctrl = RP2040_XOSC_1_15_MHZ;			/* Should be fixed according to datasheet, but is R/W */
	rp2040_xosc.startup = ((12000000/1000)+255)/256;	/* 1 ms at 12 MHz, rounded up */
	rp2040_xosc_w1s.ctrl = RP2040_XOSC_ENABLE;
	do {	/* Wait */	} while ( (rp2040_xosc.status & RP2040_XOSC_STABLE) == 0 );

	rp2040_clocks.ref.ctrl = RP2040_CLKSRC_REF_XOSC;	/* Select XOSC as the reference clock source */
	rp2040_clocks.sys.ctrl = RP2040_CLKSRC_SYS_REF;		/* Select REF as the system clock source */

	rp2040_clocks.peri.ctrl = RP2040_CLK_ENABLE | RP2040_CLKSRC_PERI_XOSC;		/* Select xosc as peripheral clock */
}

static void pll_helper(rp2040_pll_t *pll, rp2040_pll_t *pll_w1c, u32_t fbdiv, u32_t postdivs);

void rp2040_pll_init(void)
{
	/* Check if PLL is already correctly configured and running
	*/
	if ( (rp2040_pll.cs & RP2040_PLL_LOCK) != 0 &&
		 (rp2040_pll.cs & RP2040_PLL_REFDIV) == APP_REFDIV &&
		 (rp2040_pll.fbdiv_int & RP2040_PLL_FBDIV) == APP_FBDIV &&
		 (rp2040_pll.prim & (RP2040_PLL_POSTDIV1 | RP2040_PLL_POSTDIV2)) == APP_POSTDIVS )
		return;

	/* Reset and re-enable the PLL.
	*/
	rp2040_reset(RP2040_RESETS_pll_sys);

	/* Initialise the main PLL
	*/
	pll_helper(&rp2040_pll, &rp2040_pll_w1c, APP_FBDIV, APP_POSTDIVS);

	/* Switch the sys clock to the PLL
	*/
	rp2040_clocks.sys.ctrl = RP2040_CLKSRC_SYS_REF | RP2040_CLKSRC_SYS_AUX_PLL;	/* Should be this already */
	rp2040_clocks.sys.ctrl = RP2040_CLKSRC_SYS_AUX | RP2040_CLKSRC_SYS_AUX_PLL;	/* Switch to the aux clock */
}

void rp2040_usbpll_init(void)
{
	/* Check if USB PLL is already correctly configured and running
	*/
	if ( (rp2040_usbpll.cs & RP2040_PLL_LOCK) != 0 &&
		 (rp2040_usbpll.cs & RP2040_PLL_REFDIV) == APP_REFDIV &&
		 (rp2040_usbpll.fbdiv_int & RP2040_PLL_FBDIV) == APP_USB_FBDIV &&
		 (rp2040_usbpll.prim & (RP2040_PLL_POSTDIV1 | RP2040_PLL_POSTDIV2)) == APP_USB_POSTDIVS )
		return;

	/* Reset and re-enable the PLL.
	*/
	rp2040_reset(RP2040_RESETS_pll_usb);

	/* Initialise the USB PLL
	*/
	pll_helper(&rp2040_usbpll, &rp2040_usbpll_w1c, APP_USB_FBDIV, APP_USB_POSTDIVS);

	/* Switch the USB clock to the PLL
	*/
	rp2040_clocks.usb.div = RP2040_CLK_DIVBY1;
	rp2040_clocks.usb.ctrl = RP2040_CLK_ENABLE | RP2040_CLKSRC_USB_PLL_USB;
}

static void pll_helper(rp2040_pll_t *pll, rp2040_pll_t *pll_w1c, u32_t fbdiv, u32_t postdivs)
{
	/* Load the VCO-related dividers
	*/
	pll->cs = APP_REFDIV;
	pll->fbdiv_int = fbdiv;

	/* Power on the PLL
	*/
	pll_w1c->pwr = RP2040_PLL_VCOPD | RP2040_PLL_PD;

	/* Wait for the PLL to lock
	*/
	do {	/* Wait */	} while ( (pll->cs & RP2040_PLL_LOCK) == 0 );

	/* Configure the post-dividers
	*/
	pll->prim = postdivs;

	/* Power on the post-dividers
	*/
	pll_w1c->pwr = RP2040_PLL_POSTDIVPD;
}
