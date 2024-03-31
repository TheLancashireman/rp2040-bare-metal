/* pio-test.c - testing the pio interface
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
#include "rp2040-pio.h"
#include "rp2040-sio.h"
#include "test-io.h"

#include "pio-test.h"

/* Expected outcome of this test:
 *
 * Async serial output at 115200-8N1 on GPIO 16
 *	- One asterisk for each waveform cycle (every 12.8 ms)
 *
 * Waveform appears on GPIO15
 *	- low  6.4 ms
 *	- high 0.4 ms
 *	- low  2.0 ms
 *	- high 4.0 ms
 *	- repeats endlessly
 *
*/
#define SM	3		/* Use state machine 3 (of PIO0) */

#define	FSTAT_TXEMPTY	(1 << (24+SM))
#define	FSTAT_TXFULL	(1 << (16+SM))

static void pio_fifo_write(u32_t v);

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

	/* The PIO clock appears to be fixed at the M0 core frequency
	 * Bring out of reset
	*/
	rp2040_release(RESETS_pio0);

	/* Set up the GPIO as a PIO0 output, permanently enabled
	*/
	rp2040_iobank0.gpio[15].ctrl = FUNCSEL_PIO0 | OEOVER_ENABLE;
	rp2040_sio.gpio_oe.w1s = 1 << 15;

	/* Set up the PIO
	 * There's only one PIO program in this example. It gets loaded at 0 so we can
	 * hard-code the start address as 0.
	*/
	u16_t start_addr = 0;
	for ( int i = 0; i < sizeof(shifter_program_instructions)/sizeof(shifter_program_instructions[0]); i++)
	{
		rp2040_pio0.instr_mem[start_addr + i] = shifter_program_instructions[i];
	}

	/* Set up the PIO state machine
	*/
	rp2040_pio0.sm[SM].clkdiv = 13300 << 16;		/* 10 kHz clock */
	rp2040_pio0.sm[SM].pinctrl = (1 << 20) | 15;	/* PINS is 1 pin starting at pin 15 */
	rp2040_pio0.sm[SM].shiftctrl = (1 << 17);		/* PULL_THRESH=32  OUT_SHIFTDIR=L  AUTOPULL */

	/* 
	 * WRAP_BOTTOM = (start_addr + shifter_wrap_target)
	 * WRAP_TOP = (start_addr + shifter_wrap)
	*/
	rp2040_pio0.sm[SM].execctrl = ((start_addr + shifter_wrap) << 12) | ((start_addr + shifter_wrap_target) << 7);
	rp2040_pio0.sm[SM].instr = (PIO_JMP | PIO_JMP_ALWAYS | start_addr);

	rp2040_pio0_w1s.ctrl = (1 << SM);


	for (;;)
	{
		for ( int i = 0; i < 60; i ++ )
		{
			pio_fifo_write(0x00000000);
			pio_fifo_write(0x00000000);
			pio_fifo_write(0xf00000ff);
			pio_fifo_write(0xffffffff);
			dh_putc('*');
		}
		dh_putc('\n');
	}

	return 0;
}

static void pio_fifo_write(u32_t v)
{
	while ( (rp2040_pio0.fstat & FSTAT_TXFULL) != 0 )
	{
		/* Wait */
	}
	rp2040_pio0.txf[SM] = v;
}
