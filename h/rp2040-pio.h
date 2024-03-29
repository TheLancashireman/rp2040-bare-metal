/* rp2040-pio.h - header file for RP2040 PIO register interface
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
#ifndef RP2040_PIO_H
#define RP2040_PIO_H	1

#include "rp2040-types.h"
#include "rp2040.h"

typedef struct rp2040_pio_s rp2040_pio_t;
typedef struct rp2040_piosm_s rp2040_piosm_t;
typedef struct rp2040_pioirq_s rp2040_pioirq_t;

/* State machine registers
 * Each PIO has four state machines. Each state machine has six registers.
 * The register sets for the state machines are at offsets 0x0c8, 0x0e0, 0x0f8 and 0x110.
 * The offsets given here are for SM0. For SM1,2,3 add 0x18, 0x30, 0x48 respectively.
*/
struct rp2040_piosm_s
{
	reg32_t clkdiv;			/* 0x0c8	Clock divider */
	reg32_t execctrl;		/* 0x0cc	Execution/behavioural settings */
	reg32_t shiftctrl;		/* 0x0d0	Shift register control settings */
	reg32_t addr;			/* 0x0d4	Current instruction address */
	reg32_t instr;			/* 0x0d8	Current instruction */
	reg32_t	pinctrl;		/* 0x0dc	State machine pin control */
};

/* Interrupt control/status registers
 * Each PIO has two interrupt requests. Each interrupt request has three IRQ registers.
 * The register sets for the IRQs are at offsets 0x12c and 0x138.
 * The offsets given here are for IRQ0. For IRQ1 add 0x00c
 *
 * NOTE: There is no gap where irqf1 should be, so the rp2040_intcs_t structure can't be used here.
*/
struct rp2040_pioirq_s
{
	reg32_t inte;			/* 0x12c	Interrupt enable */
	reg32_t intf;			/* 0x130	Interrupt force */
	reg32_t ints;			/* 0x134	Interrupt status after masking and forcing */
};

struct rp2040_pio_s
{
	reg32_t	ctrl;			/* 0x000	PIO control register */
	reg32_t fstat;			/* 0x004	FIFO status register */
	reg32_t fdebug;			/* 0x008	FIFO debug register */
	reg32_t	flevel;			/* 0x00c	FIFO levels */
	reg32_t txf[4];			/* 0x010	Tx FIFO write registers */
	reg32_t rxf[4];			/* 0x020	Rx FIFO read registers */
	reg32_t irqf;			/* 0x030	IRQ flags */
	reg32_t irq_force;		/* 0x034	IRQ trigger */
	reg32_t	ip_syn_byp;		/* 0x038	Input sync bypass */
	reg32_t dbg_padout;		/* 0x03c	PIO output values */
	reg32_t dbg_padoe;		/* 0x040	PIO output enable values */
	reg32_t dbg_cfginfo;	/* 0x044	Hardware information */
	reg32_t instr_mem[32];	/* 0x048	Instruction memory */
	rp2040_piosm_t sm[4];	/* 0x0c8	Registers for the individual state machines */
	reg32_t intr;			/* 0x128	Raw interrupts */
	rp2040_pioirq_t irq[4];	/* 0x12c	Interrupt staus and control */
};

#define PIO0_BASE			0x50200000
#define rp2040_pio0			(((rp2040_pio_t *)(PIO0_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_pio0_xor		(((rp2040_pio_t *)(PIO0_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_pio0_w1s		(((rp2040_pio_t *)(PIO0_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_pio0_w1c		(((rp2040_pio_t *)(PIO0_BASE+RP2040_OFFSET_W1C))[0])

#define PIO1_BASE	0x50300000
#define rp2040_pio1			(((rp2040_pio_t *)(PIO1_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_pio1_xor		(((rp2040_pio_t *)(PIO1_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_pio1_w1s		(((rp2040_pio_t *)(PIO1_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_pio1_w1c		(((rp2040_pio_t *)(PIO1_BASE+RP2040_OFFSET_W1C))[0])

/* PIO opcodes encoded in bits 15..13. PUSH and PULL use bit 7 as well.
 * All instructions have SIDESET/DELAY in bits 12..8.
 * For the jump instruction, the target address is in the lower 5 bits.
 * Bits 7..5 contain the condition. Unconditional is encoded as 0.
 * That's all we'll need for now.
 * So an unconditional jump instruction is (PIO_JMP | PIO_JMP_ALWAYS | addr)
 *
 * To initialise the SM start address, write (PIO_JMP | PIO_JMP_ALWAYS | startaddr) to the instr register.
*/
#define PIO_JMP			0x0000
#define PIO_JMP_ALWAYS	0x0000

#define PIO_WAIT		0x2000
#define PIO_IN			0x4000
#define PIO_OUT			0x6000
#define PIO_PUSH		0x8000
#define PIO_PULL		0x8080
#define PIO_MOV			0xa000
#define PIO_IRQ			0xc000
#define PIO_SET			0xe000


#endif
