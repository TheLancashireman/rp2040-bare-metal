/* rp2040-dma.h - header file for RP2040 DMA register interface
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
#ifndef RP2040_DMA_H
#define RP2040_DMA_H	1

#include "rp2040-types.h"
#include "rp2040.h"

typedef struct rp2040_dma_s rp2040_dma_t;
typedef struct rp2040_dmac_s rp2040_dmac_t;
typedef struct rp2040_dmairq_s rp2040_dmairq_t;
typedef struct rp2040_dmadbg_s rp2040_dmadbg_t;

/* DMA channel
*/
struct rp2040_dmac_s
{
	reg32_t	read_addr;				/* 0x00		Read address pointer */
	reg32_t	write_addr;				/* 0x04		Write address pointer */
	reg32_t	trans_count;			/* 0x08		Transfer count */
	reg32_t	ctrl_trig;				/* 0x0c		Control and status */
	reg32_t	al1_ctrl;				/* 0x10		Alias for control */
	reg32_t	al1_read_addr;			/* 0x14		Alias for read_addr */
	reg32_t	al1_write_addr;			/* 0x18		Alias for write_addr */
	reg32_t	al1_trans_count_trig;	/* 0x1c		Alias for trans_count; also trigger */
	reg32_t	al2_ctrl;				/* 0x20		Alias for control */
	reg32_t	al2_trans_count;		/* 0x24		Alias for trans_count */
	reg32_t	al2_read_addr;			/* 0x28		Alias for read_addr */
	reg32_t	al2_write_addr_trig;	/* 0x2c		Alias for write_addr; also trigger */
	reg32_t	al3_ctrl;				/* 0x30		Alias for control */
	reg32_t	al3_write_addr;			/* 0x34		Alias for write_addr */
	reg32_t	al3_trans_count;		/* 0x38		Alias for trans_count */
	reg32_t	al3_read_addr_trig;		/* 0x3c		Alias for read_addr; also trigger */
};

/* DMA debug
*/
struct rp2040_dmadbg_s
{
	reg32_t dbg_ctdreq;		/* 0x00		DREQ counter */
	reg32_t dbg_tcr;		/* 0x04		Trans_count reload value */
	reg32_t gap[14];		/* 0x08		*/
};

/* DMA peripheral
*/
struct rp2040_dma_s
{
	rp2040_dmac_t ch[16];		/* 0x000	Channels. Only 12 present (see n_channels) */
	rp2040_intcs_t intcs[2];	/* 0x400	IRQ control and status. intr only present in intcs[0] */
	reg32_t timer[4];			/* 0x420 	Pacing timers */
	reg32_t multi_chan_trig;	/* 0x430	Multi-channel trigger */
	reg32_t	sniff_ctrl;			/* 0x434	Sniffer control */
	reg32_t	sniff_data;			/* 0x438	Sniffer data */
	reg32_t gap1;				/* 0x43c	*/
	reg32_t fifo_levels;		/* 0x440	Debug RAF, WAF, TDF levels */
	reg32_t chan_abort;			/* 0x444	Abort transfer on one or more channels */
	reg32_t n_channels;			/* 0x448	(RO) Number of channels present */
	reg32_t gap2[237];			/* 0x44c	*/
	rp2040_dmadbg_t dbg[16];	/* 0x800	Channel debug counters */
};

#define DMA_BASE				0x50000000
#define rp2040_dma				(((rp2040_dma_t *)(DMA_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_dma_xor			(((rp2040_dma_t *)(DMA_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_dma_w1s			(((rp2040_dma_t *)(DMA_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_dma_w1c			(((rp2040_dma_t *)(DMA_BASE+RP2040_OFFSET_W1C))[0])

#define DMA_AHB_ERROR		0x80000000
#define DMA_READ_ERROR		0x40000000
#define DMA_WRITE_ERROR		0x20000000
#define DMA_BUSY			0x01000000
#define DMA_SNIFF_EN		0x00800000
#define DMA_BSWAP			0x00400000
#define DMA_IRQ_QUIET		0x00200000
#define DMA_TREQ_SEL		0x001f8000
#define DMA_TREQ_VAL(x)		((x)<<15)
#define DMA_CHAIN_TO		0x00007800
#define DMA_CHAIN_VAL(x)	((x)<<11)
#define DMA_RING_SEL		0x00000400	/* 0 = read address is ring, 1 = write address */
#define DMA_RING_SIZE		0x000003c0
#define DMA_RING_VAL(x)		((x)<<6)
#define DMA_RING_NONE		0
#define DMA_INCR_WRITE		0x00000020
#define DMA_INCR_READ		0x00000010
#define DMA_DATA_SIZE		0x0000000c
#define DMA_SIZE_BYTE		0x00000000
#define DMA_SIZE_HALF		0x00000004
#define DMA_SIZE_WORD		0x00000008
#define DMA_HIGH_PRIO		0x00000002
#define DMA_CHANNEL_EN		0x00000001

/* Request IDs for the TREQ_SEL field
*/
#define DREQ_PIO0_TX0		0
#define DREQ_PIO0_TX1		1
#define DREQ_PIO0_TX2		2
#define DREQ_PIO0_TX3		3
#define DREQ_PIO0_RX0		4
#define DREQ_PIO0_RX1		5
#define DREQ_PIO0_RX2		6
#define DREQ_PIO0_RX3		7
#define DREQ_PIO1_TX0		8
#define DREQ_PIO1_TX1		9
#define DREQ_PIO1_TX2		10
#define DREQ_PIO1_TX3		11
#define DREQ_PIO1_RX0		12
#define DREQ_PIO1_RX1		13
#define DREQ_PIO1_RX2		14
#define DREQ_PIO1_RX3		15
#define DREQ_SPI0_TX		16
#define DREQ_SPI0_RX		17
#define DREQ_SPI1_TX		18
#define DREQ_SPI1_RX		19
#define DREQ_UART0_TX		20
#define DREQ_UART0_RX		21
#define DREQ_UART1_TX		22
#define DREQ_UART1_RX		23
#define DREQ_PWM_WRAP0		24
#define DREQ_PWM_WRAP1		25
#define DREQ_PWM_WRAP2		26
#define DREQ_PWM_WRAP3		27
#define DREQ_PWM_WRAP4		28
#define DREQ_PWM_WRAP5		29
#define DREQ_PWM_WRAP6		30
#define DREQ_PWM_WRAP7		31
#define DREQ_I2C0_TX		32
#define DREQ_I2C0_RX		33
#define DREQ_I2C1_TX		34
#define DREQ_I2C1_RX		35
#define DREQ_ADC			36
#define DREQ_XIP_STREAM		37
#define DREQ_XIP_SSITX		38
#define DREQ_XIP_SSIRX		39
#define TREQ_1				0x3b
#define TREQ_2				0x3c
#define TREQ_3				0x3d
#define TREQ_4				0x3e
#define TREQ_PERM			0x3f

#endif
