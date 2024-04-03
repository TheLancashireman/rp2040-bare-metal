/* multicore-test.c - testing the multicore start functions
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
#include "rp2040-sio.h"
#include "test-io.h"

/* Expected outcome of this test:
 *
 * Async serial output at 115200-8N1 on GPIO 16
 *
 * Alternating outputs from core 0 then core 1 - 10 from each core
 * The core 1 lines alternate between 0x22222222 and 0x33333333
 *
*/

int main(void)
{
	u32_t resp;

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

	rp2040_start_core1();

	for ( unsigned i = 0; i < 10; i++ )		/* unsigned because there's a bitwise operation on it */
	{
		while ( (rp2040_sio.fifo_st & SIO_FIFO_VLD) == 0 )
		{	/* Wait */
		}

		resp = rp2040_sio.fifo_rd;
		if ( resp != 0x11111111 )
		{
			dh_puts("Unexpected response from core 1: ");
			dh_putx32(resp);
			for (;;) {}
		}

		dh_puts("Core 0\n");
		soft_delay_1s();

		rp2040_sio.fifo_wr = ( i & 1 ) == 0 ? 0x22222222 : 0x33333333;
	}

	for (;;) {}

	return 0;
}

int main1(void)
{
	u32_t resp;

	dh_puts("Core 1 started ...\n");

	rp2040_sio.fifo_wr = 0x11111111;

	for ( ;; )
	{
		while ( (rp2040_sio.fifo_st & SIO_FIFO_VLD) == 0 )
		{	/* Wait */
		}

		resp = rp2040_sio.fifo_rd;
		if ( resp == 0x22222222 || resp == 0x33333333 )
		{
			dh_puts("Core 1: ");
			dh_putx32(resp);
		}
		else
		{
			dh_puts("Unexpected command from core 0: ");
			dh_putx32(resp);
			for (;;) {}
		}

		rp2040_sio.fifo_wr = 0x11111111;
	}
}
