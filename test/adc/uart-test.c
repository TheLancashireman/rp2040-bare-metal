/* adc-test.c - testing the adc free-running mode
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
#include "rp2040-types.h"
#include "rp2040.h"
#include "rp2040-uart.h"
#include "rp2040-gpio.h"
#include "rp2040-clocks.h"
#include "rp2040-resets.h"
#include "rp2040-adc.h"
#include "test-io.h"

/* Expected outcome of this test:
 *
 * Async serial output at 115200-8N1 on GPIO 16
 *
*/

int main(void)
{
	/* Initialise uart0
	*/
	(void)rp2040_uart_init(&rp2040_uart0, 115200, "8N1");

	/* Set up the I/O function for UART0
	  * GPIO 16 = UART0 tx
	  * GPIO 17 = UART0 rx
	 */
	rp2040_iobank0.gpio[16].ctrl = FUNCSEL_UART;
	rp2040_iobank0.gpio[17].ctrl = FUNCSEL_UART;

	dh_puts("Test started ...\n");

	/* Set the ADC clock to the USB PLL (48 MHz)
	 * Bring out of reset
	*/
	rp2040_clocks.adc.ctrl = CLK_ENABLE | CLKSRC_ADC_PLL_USB;
	rp2040_release(RESETS_adc);

	rp2040_adc.cs = ADC_SEL_TEMP | ADC_ERR_STICKY | ADC_TS_EN | ADC_EN;
	rp2040_adc.fcs = ADC_OVER | ADC_UNDER | ADC_ERR_EN | ADC_FIFO_EN;
	rp2040_adc.div = 64000 << 8;	/* 750 samples per second */


	rp2040_adc_w1s.cs = ADC_START_MANY;

	for (;;)
	{
		u32_t val;

		for ( int i = 0; i < 750; i++ )
		{
			while ( (rp2040_adc.fcs & ADC_EMPTY) != 0 )
			{
				/* Wait */
			}
			val = rp2040_adc.fifo;
		}

		dh_putx32(val);

		if ( (rp2040_adc.fcs & ADC_OVER) != 0 )
		{
			dh_putc('*');
			rp2040_adc_w1s.fcs = ADC_OVER;
		}
	}

	return 0;
}
