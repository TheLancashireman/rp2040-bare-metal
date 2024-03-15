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

/* DMA interrupts
*/
struct rp2040_dmairq_s
{
	reg32_t intr;			/* 0x00		Interrupt status (raw) (only in irq0 structure) */
	reg32_t inte;			/* 0x04		Interrupt enables */
	reg32_t intf;			/* 0x08		Interrupt force */
	reg32_t ints;			/* 0x0c		Interrupt status (masked) */
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
	rp2040_dmairq_t irq[2];		/* 0x400	IRQ control and status */
	reg32_t timer[4];			/* 0x420 	Pacing timers */
	reg32_t multi_ch_trig;		/* 0x430	Multi-channel trigger */
	reg32_t	sniff_ctrl;			/* 0x434	Sniffer control */
	reg32_t	sniff_data;			/* 0x438	Sniffer data */
	reg32_t gap1;				/* 0x43c	*/
	reg32_t fifo_levels;		/* 0x440	Debug RAF, WAF, TDF levels */
	reg32_t chan_abort;			/* 0x444	Abort transfer on one or more channels */
	reg32_t n_channels;			/* 0x448	(RO) Number of channels present */
	reg32_t gap2[237];			/* 0x44c	*/
	rp2040_dmadbg_t dbg[16];	/* 0x800	Channel debug counters */
};

static const rp2040_dma = (rp2040_dma *)0x50000000;

#endif
