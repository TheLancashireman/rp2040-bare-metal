/* rp2040-uart.h - header file for RP2040 UART
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
#ifndef RP2040_UART_H
#define RP2040_UART_H		1

#include "rp2040.h"
#include "rp2040-types.h"

typedef struct rp2040_uart_s rp2040_uart_t;

struct rp2040_uart_s
{
	reg32_t	dr;			/* 0x000 - data */
	reg32_t	rsr;		/* 0x004 - receiver status */
	reg32_t	gap1[4];
	reg32_t	fr;			/* 0x018 - flag */
	reg32_t	gap2[1];
	reg32_t	ilpr;		/* 0x020 - IrDA low-power counter */
	reg32_t	ibrd;		/* 0x024 - integer baud rate divisor */
	reg32_t	fbrd;		/* 0x028 - fractional baud rate divider */
	reg32_t	lcr_h;		/* 0x02c - line control*/
	reg32_t	cr;			/* 0x030 - control */
	reg32_t	ifls;		/* 0x034 - interrupt fifo level select */
	reg32_t	imsc;		/* 0x038 - interrupt mask set/clear */
	reg32_t	ris;		/* 0x03c - raw interrupt status */
	reg32_t	mis;		/* 0x040 - masked interrupt status */
	reg32_t	icr;		/* 0x044 - interrupt clear */
	reg32_t	dmacr;		/* 0x048 - DMA control*/
	reg32_t	gap3[997];
	reg32_t	id[4];		/* 0xfe0 - peripheral ID registers 0..3 */
	reg32_t	cellid[4];	/* 0xff0 - peripheral cell ID registers 0..3 */
};

#define UART0_BASE			0x40034000
#define rp2040_uart0		(((rp2040_uart_t *)(UART0_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_uart0_xor	(((rp2040_uart_t *)(UART0_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_uart0_w1s	(((rp2040_uart_t *)(UART0_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_uart0_w1c	(((rp2040_uart_t *)(UART0_BASE+RP2040_OFFSET_W1C))[0])

#define UART1_BASE			0x40038000
#define rp2040_uart1		(((rp2040_uart_t *)(UART1_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_uart1_xor	(((rp2040_uart_t *)(UART1_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_uart1_w1s	(((rp2040_uart_t *)(UART1_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_uart1_w1c	(((rp2040_uart_t *)(UART1_BASE+RP2040_OFFSET_W1C))[0])

/* To mask out the receiver status bits
*/
#define UART_DATA		0xff

/* Receiver status bits. Equivalent bits in DR are shifted left by 8
*/
#define UART_OE			0x08		/* Overflow error */
#define UART_BE			0x04		/* Break */
#define UART_PE			0x02		/* Parity error */
#define UART_FE			0x01		/* Frame error */

#define UART_RI			0x100
#define UART_TXFE		0x080		/* Tx fifo empty */
#define UART_RXFF		0x040		/* Rx fifo full */
#define UART_TXFF		0x020		/* Tx fifo full */
#define UART_RXFE		0x010		/* Rx fifo empty */
#define UART_BUSY		0x008
#define UART_DCD			0x004
#define UART_DSR			0x002
#define UART_CTS			0x001

#define UART_SPS			0x80
#define UART_WLEN		0x60		/* (Nbits-5) << 5 */
#define UART_FEN			0x10		/* FIFO enable */
#define UART_STP2		0x08		/* 2 stop bits (transmit) */
#define UART_EPS			0x04		/* Even parity */
#define UART_PEN			0x02		/* Parity enable */
#define UART_BRK			0x01		/* Break (software-controlled) */

#define UART_CTSEN		0x8000		/* CTS flow control enabled */
#define UART_RTSEN		0x4000		/* RTS flow control enabled */
#define UART_OUT2		0x2000		/* Can be used as RI. Inverted */
#define UART_OUT1		0x1000		/* Can be used as DCD. Inverted */
#define UART_RTS			0x0800		/* RTS. Inverted */
#define UART_DTR			0x0400		/* DTR. Inverted */
#define UART_RXE			0x0200		/* Rx enable */
#define UART_TXE			0x0100		/* Tx enable */
#define UART_LBE			0x0080		/* IrDA */
#define UART_SIRLP		0x0004		/* IrDA */
#define UART_SIREN		0x0002		/* IrDA */
#define UART_UARTEN		0x0001		/* UART enable */

/* Interrupt masks (imsc)
*/
#define UART_OEIM		0x400		/* Overrun */
#define UART_BEIM		0x200		/* Line break */
#define UART_PEIM		0x100		/* Parity error */
#define UART_FEIM		0x080		/* Framing error */
#define UART_RTIM		0x040		/* Rx timeout*/
#define UART_TXIM		0x020		/* Tx */
#define UART_RXIM		0x010		/* Rx */
#define UART_DSRMIM		0x008		/* DSR */
#define UART_DCDMIM		0x004		/* DCD */
#define UART_CTSMIM		0x002		/* CTS */
#define UART_RIMIM		0x001		/* RI */

/* rp2040_uart_isrx() - returns true if there's a character to read.
*/
static inline int rp2040_uart_isrx(rp2040_uart_t *uart)
{
	return ( (uart->fr & UART_RXFE) == 0 );
}

/* rp2040_uart_istx() - returns true if there's room to send a character
*/
static inline int rp2040_uart_istx(rp2040_uart_t *uart)
{
	return ( (uart->fr & UART_TXFF) == 0 );
}

extern int rp2040_uart_getc(rp2040_uart_t *);
extern void rp2040_uart_putc(rp2040_uart_t *, int);
extern int rp2040_uart_init(rp2040_uart_t *, unsigned, char *);

#endif
