/* test-io.c - common I/O-related functions for tests
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
#include "test-io.h"

void dh_putc(char c)
{
	if ( c == '\n' )
		rp2040_uart_putc(&rp2040_uart0, '\r');
	rp2040_uart_putc(&rp2040_uart0, c);
}

void dh_puts(const char *str)
{
	while ( *str != '\0' )
		dh_putc(*str++);
}

void dh_putx32(u32_t v)
{
	char str[12];
	str[0] = '0';
	str[1] = 'x';
	str[10] = '\n';
	str[11] = '\0';
	for ( int i = 9; i > 1; i-- )
	{
		u8_t b = v & 0x0f;
		v >>= 4;
		if ( b < 10 )
			b += '0';
		else
			b += ('a' - 10);
		str[i] = b;
	}
	dh_puts(str);
}

#define NLOOPS	(133000000/12)	/* Assumes no compiler optimisation */
void soft_delay_1s(void)
{
	/* An earler version of this function used xosc.count
	 * However, the counter is not reliable - see RP2040 datasheet erratum RP2040-E7
	 * So we're stuck with a loop until a timer gets initialised.
	*/
	for ( int i = 0; i < NLOOPS; i++ )
	{
		__asm__ volatile ("nop");
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
