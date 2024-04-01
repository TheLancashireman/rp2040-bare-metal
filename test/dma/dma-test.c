/* dma-test.c - testing the dma free-running mode
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
#include "rp2040-dma.h"
#include "rp2040-sio.h"
#include "rp2040-pads.h"
#include "test-io.h"

/* Expected outcome of this test:
 *
 * Async serial output at 115200-8N1 on GPIO 16
 *
 * This test sets up the ADC similar to the ADC test, but
 *	- it uses AIN0 as the analogue input
 *	- it uses DMA to transfer the samples to RAM.
 *	- it prints the average (mean) of the samples and hence checks the divider interface
 *
 * Two DMA channels are used.
 * 	- channel 0 transfers 750 samples to a single variable, then triggers channel 1
 *	- channel 1 transfers 750 samples to a different variable, then triggers channel 0
 * It's necessary to reset the write_addr of each channel after the transfer completes
 *
 * The main program
 *	- waits until the current DMA channel is idle
 *	- resets the write_addr of the current channel
 *	- prints the average value of the current channel variable (i.e. the last set of conversions)
 *	- sets the current channel's buffer to 0
 *	- switches to the other channel
 *
*/

static u32_t ch0[750];
static u32_t ch1[750];

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

	/* We need to disable the digital I/O on the AIN0 pin (GPIO26)
	*/
	rp2040_release(RESETS_pads_bank0);
	rp2040_pads_bank0.gpio[26] = PADS_OD;	/* *everything* disabled */

	/* Set the ADC clock to the USB PLL (48 MHz)
	 * Bring out of reset
	*/
	rp2040_clocks.adc.ctrl = CLK_ENABLE | CLKSRC_ADC_PLL_USB;
	rp2040_release(RESETS_adc);

	rp2040_adc.cs = ADC_SEL_0 | ADC_ERR_STICKY | ADC_EN;
	rp2040_adc.fcs = ADC_OVER | ADC_UNDER | ADC_ERR_EN | ADC_FIFO_EN | ADC_THRESH_VAL(1) | ADC_DREQ_EN;
	rp2040_adc.div = 64000 << 8;	/* 750 samples per second */

	/* Bring the DMA controller out of reset. Clock is assumed to be CPU clock
	*/
	rp2040_release(RESETS_dma);

	rp2040_dma.ch[0].read_addr = (u32_t)&rp2040_adc.fifo;
	rp2040_dma.ch[0].write_addr = (u32_t)&ch0[0];
	rp2040_dma.ch[0].trans_count = 750;
	rp2040_dma.ch[0].ctrl_trig = DMA_READ_ERROR | DMA_WRITE_ERROR | DMA_TREQ_VAL(DREQ_ADC) | DMA_CHAIN_VAL(1) |
								DMA_RING_NONE | DMA_INCR_WRITE | DMA_SIZE_WORD | DMA_CHANNEL_EN;

	rp2040_dma.ch[1].read_addr = (u32_t)&rp2040_adc.fifo;
	rp2040_dma.ch[1].write_addr = (u32_t)&ch1[0];
	rp2040_dma.ch[1].trans_count = 750;
	rp2040_dma.ch[1].al1_ctrl = DMA_READ_ERROR | DMA_WRITE_ERROR | DMA_TREQ_VAL(DREQ_ADC) | DMA_CHAIN_VAL(0) |
								DMA_RING_NONE | DMA_INCR_WRITE | DMA_SIZE_WORD | DMA_CHANNEL_EN;

	for ( int i = 0; i < 750; i++ )
	{
		ch0[i] = 0;
		ch1[i] = 0;
	}

	rp2040_sio.div_udivisor = 750;

	rp2040_adc_w1s.cs = ADC_START_MANY;

	for (;;)
	{
		u32_t sum;

		/* Wait until channel 0 has transferred all 750 values. Not busy --> error message
		*/
		if ( (rp2040_dma.ch[0].ctrl_trig & DMA_BUSY) == 0 )
		{
			dh_puts("ch0 not busy\n");
		}
		else
		{
			while ( (rp2040_dma.ch[0].ctrl_trig & DMA_BUSY) != 0 )
			{
				/* Wait */
			}
		}

		/* Reset the write address for channel 0 for the next trigger.
		*/
		rp2040_dma.ch[0].write_addr = (u32_t)&ch0[0];

		/* Calculate and print the average ADC value from channel 0
		*/
		sum = 0;
		for ( int i = 0; i < 750; i++ )
		{
			sum += ch0[i];
			ch0[i] = 0;
		}
		rp2040_sio.div_udividend = sum;
		dh_puts("ch0: ");		/* This provides ample delay. No interrupts, so no restart */
		dh_putx32(rp2040_sio.div_quotient);

		/* Wait until channel 1 has transferred all 750 values. Not busy --> error message
		*/
		if ( (rp2040_dma.ch[1].ctrl_trig & DMA_BUSY) == 0 )
		{
			dh_puts("ch1 not busy\n");
		}
		else
		{
			while ( (rp2040_dma.ch[1].ctrl_trig & DMA_BUSY) != 0 )
			{
				/* Wait */
			}
		}

		/* Reset the write address for channel 1 for the next trigger.
		*/
		rp2040_dma.ch[1].write_addr = (u32_t)&ch1[0];

		/* Calculate and print the average ADC value from channel 1
		*/
		sum = 0;
		for ( int i = 0; i < 750; i++ )
		{
			sum += ch1[i];
			ch1[i] = 0;
		}
		rp2040_sio.div_udividend = sum;
		dh_puts("ch1: ");		/* This provides ample delay. No interrupts, so no restart */
		dh_putx32(rp2040_sio.div_quotient);
	}

	return 0;
}
