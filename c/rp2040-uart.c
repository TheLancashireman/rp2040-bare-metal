/* rp2040-uart.c
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
#include "rp2040.h"
#include "rp2040-types.h"
#include "rp2040-uart.h"
#include "rp2040-resets.h"

/* rp2040_uart_getc() - wait until there's a character available then return it.
*/
int rp2040_uart_getc(rp2040_uart_t *uart)
{
	while ( !rp2040_uart_isrx(uart) )
	{
	}

	return (int)(uart->dr & UART_DATA);	/* Discard the error status bits */
}

/* rp2040_uart_putc() - wait until there's room in the tx buffer, then put a character into it.
*/
void rp2040_uart_putc(rp2040_uart_t *uart, int c)
{
	while ( !rp2040_uart_istx(uart) )
	{
	}

	uart->dr = (u32_t)c;
}

/* rp2040_uart_init() - initialise uart for normal async use. Return 0 if OK.
 *
 * Returns nonzero if the parameters aren't supported.
 *
 * Assumes peripheral clock of 12 MHz
 *
 * fmt has 3 characters:  nps (any extra characters are ignored)
 *	n = no of bits (5..8)
 *	p = parity: N (none), E (even), O (odd), M (mark), S (space)
 *	s = no of stop bits (1..2)
 *
 * To calculate the baud-rate dividers we need a __aeabi_uidiv functions from libgcc.a
 * There doesn't appear to be a usable libgcc.a with the devuan compiler.
 * Instead of calculating ibrd and fbrd we use a table lookup
*/
#define NBAUD	14	/* Set to 0 to use the calculation method */

#if NBAUD != 0
typedef struct
{	u32_t baud;
	u16_t ibrd;
	u16_t fbrd;
} br_lookup_t;
static const br_lookup_t br_table[NBAUD] =
{
#if 1	// 12 MHz
	{	256000,	2,		60	},
	{	128000,	5,		55	},
	{	115200,	6,		33	},
	{	57600,	13,		1	},
	{	38400,	19,		34	},
	{	19200,	39,		4	},
	{	14400,	52,		5	},
	{	9600,	78,		8	},
	{	4800,	156,	16	},
	{	2400,	312,	32	},
	{	1200,	625,	0	},
	{	600,	1250,	0	},
	{	300,	2500,	0	},
	{	31250,	24,		0	}
#endif
#if 0	// 133 MHz
	{	256000,	32,		30	},
	{	128000,	64,		60	},
	{	115200,	72,		10	},
	{	57600,	144,	20	},
	{	38400,	216,	30	},
	{	19200,	432,	60	},
	{	14400,	577,	16	},
	{	9600,	865,	57	},
	{	4800,	1731,	49	},
	{	2400,	3463,	35	},
	{	1200,	6927,	5	},
	{	600,	13854,	11	},
	{	300,	27708,	21	},
	{	31250,	266,	0	}
#endif
};
#endif

int rp2040_uart_init(rp2040_uart_t *uart, unsigned baud, char *fmt)
{
	u32_t rst;

	if ( uart == &rp2040_uart0 )
	{
		rst = RESETS_uart0;
	}
	else if ( uart == &rp2040_uart1 )
	{
		rst = RESETS_uart1;
	}
	else
		return 1;
	
#if NBAUD == 0
	/* Calculate the integer and fractional dividers. See rp2040 refman 4.2.7.1
	 * Differences: we treat 65535 as in-range. There's nothing in the refman to suggest otherwise.
	 * If the calculated divisors are out of range, we return an error and don't configure the UART
	*/
	u32_t bdiv = (8 * 12000000)/baud;
	u32_t ibrd = bdiv >> 7;
	if ( ibrd == 0 || ibrd > 65535 )
	{
		return 2;
	}

	u32_t fbrd = ((bdiv & 0x7f) + 1) / 2;
#else
	int bri;
	for ( bri = 0; bri < NBAUD; bri++ )
	{
		if ( br_table[bri].baud == baud )
			break;
	}

	if ( bri >= NBAUD )
		return 2;

	u32_t ibrd = br_table[bri].ibrd;
	u32_t fbrd = br_table[bri].fbrd;
#endif

	if ( fmt[0] < '5' || fmt[0] > '8' )
	{
		return 3;
	}

	u32_t lcr = (u32_t)(fmt[0] - '5') << 5;	/* WLEN */
	lcr |= UART_FEN;								/* Enable FIFO */

	switch ( fmt[1] )
	{
	case 'N':
		break;	/* Nothing to set */

	case 'E':
		lcr |= UART_PEN | UART_EPS;
		break; 

	case 'O':
		lcr |= UART_PEN;
		break; 

	case 'M':
		lcr |= UART_PEN | UART_SPS;
		break; 

	case 'S':
		lcr |= UART_PEN | UART_SPS | UART_EPS;
		break; 

	default:
		return 4;
	}

	switch ( fmt[2] )
	{
	case '1':
		break;

	case '2':
		lcr |= UART_STP2;
		break;

	default:
		return 5;
	}

	/* Parameters are OK.
	*/

	/* If the uart is in reset, release the reset and wait till ready.
	*/
	rp2040_release(rst);

	/* Set up the baud-rate generator. This requires a write to LCR_H to activate; we do that later
	*/
	uart->ibrd = ibrd;
	uart->fbrd = fbrd;

	/* Write the line config to LCR_H
	*/
	uart->lcr_h = lcr;

	/* Disable all the interrupt sources
	*/
	uart->imsc = 0x0;

	/* Enable the UART, enable rx and tx. Turn off all flow control etc.
	*/
	uart->cr = UART_UARTEN | UART_RXE | UART_TXE;

	return 0;
}
