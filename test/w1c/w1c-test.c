/* w1c-test.c - investigating the relationship between atomic register banks and 'W1C' (or 'WC') bits
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
 * The test uses the two "sticky" bits (UNDER and OVER) of the ADC FCS register, which are described
 * as "write 1 to clear".
 * Normally, with W1C bits, the bit becomes 1 when active. To clear it, you read the register and write
 * back the same value. This clears all the W1C bits of the register and leaves other bits untouched.
 * The operation is, however, not atomic.
 * The RP2040 provides three 'mirror' locations for most peripherals. These locations provide atomic
 * access in the form of w1c, w1s and xor (toggle) with the selected bits. The assumption is that,
 * from the software viewpoint, the peripheral reads its own register, performs a bitwise AND NOT (w1c),
 * OR (w1s) or XOR (xor) with the value written, then writes the new value back to the register.
 * If this is the case, then the use of the mirror locations should exhibit interference with the
 * inherent W1C characteristic of the individual bit:
 *	- writing UNDER to the w1s mirror should clear both UNDER and OVER
 *	- writing UNDER to the w1c mirror should clear OVER
 *	- writing UNDER to the xor mirror should clear OVER
 * This test case investigates what really happens.
 *
 * Result:
 *	rp2040_adc.fcs = (before & ~ADC_OVER):
 *	  Before: 0x00000d05
 *	  After:  0x00000905
 *	--> UNDER cleared, OVER unchanged, as expected
 *	rp2040_adc_w1s.fcs = UNDER:
 *	  Before: 0x00000d05
 *	  After:  0x00000d05
 *	--> No effect
 *	rp2040_adc_w1c.fcs = UNDER:
 *	  Before: 0x00000d05
 *	  After:  0x00000905
 *	--> UNDER cleared, OVER unchanged
 *	rp2040_adc_xor.fcs = UNDER:
 *	  Before: 0x00000d05
 *	  After:  0x00000905
 *	--> UNDER cleared, OVER unchanged
 *
 * Conclusion:
 *	It seems that the 'W1C' characteristic of the individual bits is implemented in the
 *	normal read/write interface. The atomic mirrors operate on the actual value of the bit.
 *	So we can use the w1c interface to clear a W1C bit and the xor interface to clear it.
 *
 * Next question: can we use the w1s interface to set the raw value of a W1C bit?
 *
 * Result:
 *	rp2040_adc_w1s.fcs = UNDER:
 *	  Before: 0x00000105
 *	  After:  0x00000105
 *	--> Unchanged
 *
 * Conclusion:s
 *	W1C bits cannot be set using the w1s atomic interface
*/

static void set_sticky(void);
static void print_vals(u32_t before, u32_t after);

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

	u32_t before, after;

	set_sticky();
	before = rp2040_adc.fcs;
	rp2040_adc.fcs = (before & ~ADC_OVER);		/* Should clear UNDER and leave rest intact */
	after = rp2040_adc.fcs;
	dh_puts("rp2040_adc.fcs = (before & ~ADC_OVER):\n");
	print_vals(before, after);

	set_sticky();
	before = rp2040_adc.fcs;
	rp2040_adc_w1s.fcs = ADC_UNDER;
	after = rp2040_adc.fcs;
	dh_puts("rp2040_adc_w1s.fcs = UNDER:\n");
	print_vals(before, after);

	set_sticky();
	before = rp2040_adc.fcs;
	rp2040_adc_w1c.fcs = ADC_UNDER;
	after = rp2040_adc.fcs;
	dh_puts("rp2040_adc_w1c.fcs = UNDER:\n");
	print_vals(before, after);

	set_sticky();
	before = rp2040_adc.fcs;
	rp2040_adc_xor.fcs = ADC_UNDER;
	after = rp2040_adc.fcs;
	dh_puts("rp2040_adc_xor.fcs = UNDER:\n");
	print_vals(before, after);

	/* Next question ...
	*/
	set_sticky();
	rp2040_adc_w1c.fcs = (ADC_UNDER | ADC_OVER);	/* Clear both sticky bits */
	before = rp2040_adc.fcs;
	rp2040_adc_w1s.fcs = ADC_UNDER;
	after = rp2040_adc.fcs;
	dh_puts("rp2040_adc_w1s.fcs = UNDER:\n");
	print_vals(before, after);

	for (;;) {}

	return 0;
}

/* Sets both of the sticky bits UNDER and OVER.
 * Returns with the FIFO empty
*/
static void set_sticky(void)
{
	while ( (rp2040_adc.fcs & ADC_OVER) == 0 )
	{
		/* Wait */
	}

	while ( (rp2040_adc.fcs & ADC_UNDER) == 0 )
	{
		rp2040_adc.fifo;		/* Read should be forced because the register is qualified as volatile */
	}

	while ( (rp2040_adc.fcs & ADC_LEVEL) != 0 )
	{
		rp2040_adc.fifo;		/* Read should be forced because the register is qualified as volatile */
	}
}

static void print_vals(u32_t before, u32_t after)
{
	dh_puts("  Before: ");
	dh_putx32(before);
	dh_puts("  After:  ");
	dh_putx32(after);
}
