/* timer-test.c - testing the timer initialisation
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
#include "rp2040-resets.h"
#include "rp2040-uart.h"
#include "rp2040-timer.h"
#include "rp2040-gpio.h"
#include "test-io.h"

/* Expected outcome of this test:
 *
 * Async serial output at 115200-8N1 on GPIO 16
*/

int main(void)
{
	/* Initialise uart0 for result output
	*/
	(void)rp2040_uart_init(&rp2040_uart0, 115200, "8N1");

	/* Set up the I/O function for UART0
	  * GPIO 16 = UART0 tx
	  * GPIO 17 = UART0 rx
	 */
	rp2040_iobank0.gpio[16].ctrl = FUNCSEL_UART;
	rp2040_iobank0.gpio[17].ctrl = FUNCSEL_UART;

	dh_putc('\n');
	dh_puts("Test started ...\n");

	rp2040_release(RESETS_timer);

	u64_t next = rp2040_read_time();

	for ( int i = 0; i < 20; i++ )
	{
		for ( int j = 0; j < 60; j++ )
		{
			next += 1000000;		/* The timer is a microsecond counter. Not clear how... */
			while ( rp2040_read_time() < next )	{ }
			dh_putc('*');
		}
		dh_putc('\n');
	}

	dh_puts("Test finished\n");

	for (;;) {}

	return 0;
}
