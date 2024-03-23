/* uart-test.c - testing the uart initialisation and (polled) output driver
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
#include "test-io.h"

/* Expected outcome of this test:
 *
 * Async serial output at 115200-8N1 on GPIO 16
 * Message "n Test passed" (n = 0..9) repeats approx once per second.
 *
 * Also verifies that the peripheral clock is correctly set at 12 MHz
 * and that the CPU clock is approximately correct at 133 MHz.
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
	int cc = '0';
	for (;;)
	{
		soft_delay_1s();
		dh_putc(cc);
		dh_puts(" Test passed\n");
		cc++;
		if ( cc > '9' )
		{
			cc = '0';
		}
	}

	return 0;
}
