/* rp2040-adc.h - header file for RP2040 ADC
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
#ifndef RP2040_ADC_H
#define RP2040_ADC_H		1

#include "rp2040-types.h"

typedef struct rp2040_adc_s rp2040_adc_t;

struct rp2040_adc_s
{
	reg32_t cs;		/* 0x00	Control and status */
	reg32_t result;	/* 0x04	Result of most recent conversion */
	reg32_t fcs;	/* 0x08	FIFO control and status */
	reg32_t fifo;	/* 0x0c	FIFO */
	reg32_t div;	/* 0x10	Clock divider */
	reg32_t intr;	/* 0x14 Raw interrupt status */
	reg32_t inte;	/* 0x18 Interrupt enable */
	reg32_t intf;	/* 0x1c Interrupt force */
	reg32_t ints;	/* 0x20 Interrupt status after mask and force */
};

static const rp2040_adc = (rp2040_adc_t *)0x4004c000;

/* CS register
*/
#define ADC_RROBIN		0x001f0000	/* Round robin sampling; one bit per channel */
#define ADC_AINSEL		0x00007000	/* Input mux selection; updated in round-robin mode */
#define ADC_ERR_STICKY	0x00000400	/* Sticky error flag */
#define ADC_ERR			0x00000200	/* Error flag */
#define ADC_READY		0x00000100	/* Ready flag */
#define ADC_START_MANY	0x00000008	/* Start continuous conversions */
#define ADC_START_ONCE	0x00000004	/* Start single conversion */
#define ADC_TS_EN		0x00000002	/* Temperature sensor enabled */
#define ADC_EN			0x00000001	/* ADC enabled */

/* RESULT and FIFO registers
*/
#define ADC_FIFO_ERR	0x00008000	/* Conversion error (FIFO only) */
#define ADC_VAL			0x00000fff	/* 12-bit result */

/* FCS register
*/
#define ADC_THRESH		0x0f000000	/* DREQ/IRQ asserted when LEVEL >= THRESH */
#define ADC_LEVEL		0x000f0000	/* FIFO level - no. of conversions to be read */
#define ADC_OVER		0x00000800	/* FIFO overflow; sticky, w1c */
#define ADC_UNDER		0x00000400	/* FIFO underflow; sticky, w1c */
#define ADC_FULL		0x00000200	/* FIFO is full */
#define ADC_EMPTY		0x00000100	/* FIFO is empty */
#define ADC_DREQ_EN		0x00000008	/* Assert DMA request when FIFO contains data */
#define ADC_ERR_EN		0x00000004	/* Enable conversion error bit in FIFO (ADC_FIFO_ERR) */
#define ADC_SHIFT		0x00000002	/* Right-shift results in FIFO by 4 bits */
#define	ADC_FIFO_EN		0x00000001	/* Enable FIFO */

/* DIV register
*/
#define ADC_DIV_INT		0x00ffff00	/* Integer part */
#define ADC_DIV_FRAC	0x000000ff	/* Fractional part */

/* INTx registers
*/
#define ADC_INT_FIFO	0x00000001	/* FIFO level has reached threshold */

#endif
