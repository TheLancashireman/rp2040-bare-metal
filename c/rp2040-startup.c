/* rp2040-startup.c - startup functions for RP2040
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
#include "rp2040-uart.h"
#include "rp2040-gpio.h"
#include "rp2040-timer.h"
#include "rp2040-cm0.h"

#define SPSEL		0x02

extern unsigned start_data, end_data, start_bss, end_bss, idata;
extern unsigned pstacktop;
extern unsigned stacktop;

extern int main(void);
extern void switchToPsp(unsigned *psp, unsigned *msp, unsigned control, int (*fp)(void));

/* init_vars() - initialise variables
 *
 * Initialises all variables from the flash image (.data) or to zero (.bss)
*/
void init_vars(void)
{
#if 0	/* Using direct load-toRAM, .data variables are initialised by the loader */
	unsigned *s = &idata;
	unsigned *d = &start_data;

	while ( d < &end_data )
	{
		*d++ = *s++;
	}
#endif

	unsigned *b = &start_bss;
	while ( b < &end_bss )
	{
		*b++ = 0x00;
	}
}

/* rp2040_kickstart() - entry point from the reset vector
 *
 * SP has been initialised, but that's all
*/
void rp2040_kickstart(void)
{
	/* Set up the vector table
    */
#if 0
    cxm_scr.vtor = (u32_t)&cxm_vectors[0];
#endif

	/* Initialise the the XOSC clock, the PLL (133MHz) and the USB PLL (48MHz)
	*/
	rp2040_clock_init();
	rp2040_pll_init();
	rp2040_usbpll_init();

	/* Initialise variables
	*/
	init_vars();

	/* Initialise the exception priorities
	*/
	cxm_scr.shpr[1] = 0x0;			/* SVC and [reserved x 3] all at highest priority */
	cxm_scr.shpr[2] = 0xffff0000;	/* SysTick/PendSV at lowest priority, Debug and [reserved] at highest */

	/* Release IOBANK0 from reset.
	 * We must do this before any pin functions can be selected.
	*/
	rp2040_release(RESETS_io_bank0);

	/* Initialise uart0
	*/
	(void)rp2040_uart_init(&rp2040_uart0, 115200, "8N1");

	/* Set up the I/O function for UART0
     * GPIO 0 = UART0 tx
     * GPIO 1 = UART0 rx
    */
    rp2040_iobank0.gpio[0].ctrl = FUNCSEL_UART;
    rp2040_iobank0.gpio[1].ctrl = FUNCSEL_UART;

	/* Release the timer from reset
	*/
	rp2040_release(RESETS_timer);
	
	/* Initialise the interrupt controller
	*/
#if 0
	nvic_init();
#endif

	/* Switch to the process stack pointer and simultaneously jump to main()
	*/
	switchToPsp(&pstacktop, &stacktop, (cxm_get_control() | SPSEL), &main);
}
