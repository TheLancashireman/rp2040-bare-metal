/* rp2040-pio.h - RP2040 PIO register interface
 *
 * (c) David Haworth
*/
#ifndef RP2040_PIO_H
#define RP2040_PIO_H	1

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
	reg32_t execctrl		/* 0x0cc	Execution/behavioural settings */
	reg32_t shiftctrl;		/* 0x0d0	Shift register control settings */
	reg32_t addr;			/* 0x0d4	Current instruction address */
	reg32_t instr;			/* 0x0d8	Current instruction */
	reg32_t	pinctrl;		/* 0x0dc	State machine pin control */
};

/* Interrupt control/status registers
 * Each PIO has two interrupt requests. Each interrupt request has three IRQ registers.
 * The register sets for the IRQs are at offsets 0x12c and 0x138.
 * The offsets given here are for IRQ0. For IRQ1 add 0x00c
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

static const rp2040_pio0 = (rp2040_pio_t *)0x50200000;
static const rp2040_pio1 = (rp2040_pio_t *)0x50300000;

#endif
