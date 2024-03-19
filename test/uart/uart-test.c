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
#include "rp2040-clocks.h"

static void dh_putc(char c);
static void dh_puts(char *str);
static void delay(void);

int main(void)
{
	/* Initialise uart0
	*/
	(void)rp2040_uart_init(&rp2040_uart0, 115200, "8N1");

	/* Set up the I/O function for UART0
	  * GPIO 0 = UART0 tx
	  * GPIO 1 = UART0 rx
	 */
	rp2040_iobank0.gpio[0].ctrl = FUNCSEL_UART;
	rp2040_iobank0.gpio[1].ctrl = FUNCSEL_UART;

	for (;;)
	{
		delay();
		dh_puts("Test passed\n");
	}

	return 0;
}

static void dh_putc(char c)
{
	if ( c == '\n' )
		rp2040_uart_putc(&rp2040_uart0, '\r');
	rp2040_uart_putc(&rp2040_uart0, c);
}

static void dh_puts(char *str)
{
	while ( *str != '\0' )
		dh_putc(*str++);
}

static void delay(void)
{
	rp2040_xosc.count = 12000000;
	while ( rp2040_xosc.count > 0 )
	{
		/* Wait */
	}
}

void app_nmi(void)
{
	dh_puts("NMI\n");
	for (;;) {}
}

void app_hardfault(void)
{
	dh_puts("HardFault\n");
	for (;;) {}
}

void app_svctrap(void)
{
	dh_puts("SVC\n");
	for (;;) {}
}

void app_pendsvtrap(void)
{
	dh_puts("PendSV\n");
	for (;;) {}
}

void app_systickirq(void)
{
	dh_puts("SysTick\n");
	for (;;) {}
}

void app_irq(void)
{
	dh_puts("IRQ\n");
	for (;;) {}
}

void app_unknowntrap(void)
{
	dh_puts("Unknown trap\n");
	for (;;) {}
}
