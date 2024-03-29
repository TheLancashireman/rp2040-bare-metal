/* interp-test.c - testing the interpolator interface
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
#include "rp2040-gpio.h"
#include "rp2040-uart.h"
#include "rp2040-timer.h"
#include "rp2040-sio.h"
#include "test-io.h"

/* Expected outcome of this test:
 *
 * Async serial output at 115200-8N1 on GPIO 16
 *
 * Uses an interpolator lane to generate a bitstream for a 1-bit DAC
 * Each
*/

static u32_t interp_sample(u16_t sample);

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

	/* Initialise the interpolator lane 0.
	 * Initial accumulator is half-range
	 * Base is 0
	 * Control: BLEND, FORCE_MSB, ADD_RAW, crossfeed and signed operation all disabled
	 * Shift is 16, to get the first overflow bit (samples are 16 bits)
	 * Mask is bits 0..0 - only interested in bit 0 of the overflow
	*/
	rp2040_sio.interp[0].accum0 = 0x8000;
	rp2040_sio.interp[0].base0 = 0;
	rp2040_sio.interp[0].ctrl_lane0 = (16 << 0) | (0 << 5) | (0 << 10);

	u32_t val = interp_sample(0x8000);

	dh_putx32(val);

	dh_puts("Test finished\n");

	for (;;) {}

	return 0;
}

static u32_t interp_sample(u16_t sample)
{
	u32_t stream = 0;
	u32_t oflo0 = rp2040_sio.interp[0].peek_lane0;
	u32_t oflo1;
	for ( int i = 0; i < 32; i++ )
	{
		stream <<= 1;									/* Shift the stream */
		rp2040_sio.interp[0].accum0_add = sample;		/* Add the sample */
		oflo1 = rp2040_sio.interp[0].peek_lane0;		/* Get the new result */
		stream |= (oflo0 ^ oflo1);						/* If lane value changes, shift a 1 */
		oflo0 = oflo1;
	}
	return stream;
}
