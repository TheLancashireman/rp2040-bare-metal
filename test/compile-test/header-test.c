/* header-test.c - compile test for header files
 *
 * (c) David Haworth
 *
 *  This file is part of pico-bare-metal.
 *
 *  pico-bare-metal is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  pico-bare-metal is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with pico-bare-metal.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Intended to be compiled on the host system (gcc).
 * Tests sizes of variables and addresses of registers
*/
#include <stdio.h>

/* Inhibit inclusion of rp2040-types.h and define our own for the host. Assumes a 64-bit host.
*/
#define RP2040_TYPES_H		1

typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long u64_t;

typedef signed char s8_t;
typedef signed short s16_t;
typedef signed int s32_t;
typedef signed long s64_t;

typedef volatile u8_t reg8_t;
typedef volatile u16_t reg16_t;
typedef volatile u32_t reg32_t;
typedef volatile u64_t reg64_t;

#include "rp2040.h"
#include "rp2040-adc.h"
#include "rp2040-dma.h"
#include "rp2040-pio.h"
#include "rp2040-resets.h"
#include "rp2040-sio.h"

static int test_sysinfo(void);
static int test_adc(void);
static int test_dma(void);
static int test_pio(void);
static int test_resets(void);
static int test_sio(void);
static int test_address(volatile void *p, u32_t v, char *name);

int main(int argc, char **argv)
{
	int nfail = 0;

	nfail += test_sysinfo();
	nfail += test_adc();
	nfail += test_dma();
	nfail += test_pio();
	nfail += test_resets();
	nfail += test_sio();

	if ( nfail == 0 )
		printf("Pass\n");

	return 0;
}

static int test_sysinfo(void)
{
	int nfail = 0;
	nfail += test_address(&rp2040_sysinfo.chip_id,			0x40000000, "rp2040_sysinfo.chip_id");
	nfail += test_address(&rp2040_sysinfo.platform,			0x40000004, "rp2040_sysinfo.platform");
	nfail += test_address(&rp2040_sysinfo.gitref_rp2040,	0x40000040, "rp2040_sysinfo.gitref_rp2040");
	return nfail;
}

static int test_adc(void)
{
	int nfail = 0;
	nfail += test_address(&rp2040_adc.cs,			0x4004c000, "rp2040_adc.cs");
	nfail += test_address(&rp2040_adc.result,		0x4004c004, "rp2040_adc.result");
	nfail += test_address(&rp2040_adc.fcs,			0x4004c008, "rp2040_adc.fcs");
	nfail += test_address(&rp2040_adc.fifo,			0x4004c00c, "rp2040_adc.fifo");
	nfail += test_address(&rp2040_adc.div,			0x4004c010, "rp2040_adc.div");
	nfail += test_address(&rp2040_adc.intcs.intr,	0x4004c014, "rp2040_adc.intr");
	nfail += test_address(&rp2040_adc.intcs.inte,	0x4004c018, "rp2040_adc.inte");
	nfail += test_address(&rp2040_adc.intcs.intf,	0x4004c01c, "rp2040_adc.intf");
	nfail += test_address(&rp2040_adc.intcs.ints,	0x4004c020, "rp2040_adc.ints");
	return nfail;
}

static int test_dma(void)
{
	int nfail = 0;
	nfail += test_address(&rp2040_dma.ch[0].read_addr,				0x50000000, "rp2040_dma.ch0_read_addr");
	nfail += test_address(&rp2040_dma.ch[0].write_addr,				0x50000004, "rp2040_dma.ch0_write_addr");
	nfail += test_address(&rp2040_dma.ch[0].trans_count,			0x50000008, "rp2040_dma.ch0_trans_count");
	nfail += test_address(&rp2040_dma.ch[0].ctrl_trig,				0x5000000c, "rp2040_dma.ch0_ctrl_trig");

	nfail += test_address(&rp2040_dma.ch[0].al1_ctrl,				0x50000010, "rp2040_dma.ch0_read_addr");
	nfail += test_address(&rp2040_dma.ch[0].al1_read_addr,			0x50000014, "rp2040_dma.ch0_read_addr");
	nfail += test_address(&rp2040_dma.ch[0].al1_write_addr,			0x50000018, "rp2040_dma.ch0_write_addr");
	nfail += test_address(&rp2040_dma.ch[0].al1_trans_count_trig,	0x5000001c, "rp2040_dma.ch0_trans_count");

	nfail += test_address(&rp2040_dma.ch[0].al2_ctrl,				0x50000020, "rp2040_dma.ch0_read_addr");
	nfail += test_address(&rp2040_dma.ch[0].al2_trans_count,		0x50000024, "rp2040_dma.ch0_trans_count");
	nfail += test_address(&rp2040_dma.ch[0].al2_read_addr,			0x50000028, "rp2040_dma.ch0_read_addr");
	nfail += test_address(&rp2040_dma.ch[0].al2_write_addr_trig,	0x5000002c, "rp2040_dma.ch0_write_addr");

	nfail += test_address(&rp2040_dma.ch[0].al3_ctrl,				0x50000030, "rp2040_dma.ch0_read_addr");
	nfail += test_address(&rp2040_dma.ch[0].al3_write_addr,			0x50000034, "rp2040_dma.ch0_write_addr");
	nfail += test_address(&rp2040_dma.ch[0].al3_trans_count,		0x50000038, "rp2040_dma.ch0_trans_count");
	nfail += test_address(&rp2040_dma.ch[0].al3_read_addr_trig,		0x5000003c, "rp2040_dma.ch0_read_addr");

	/* It's sufficient to test the first register of the channel 1 to make sure the structure
	 * is the right size. All the rest of the registers in channel 1 and all the rest of the channels
	 * can be inferred.
	*/
	nfail += test_address(&rp2040_dma.ch[1].read_addr,				0x50000040, "rp2040_dma.ch1_read_addr");

	nfail += test_address(&rp2040_dma.intcs[0].intr,				0x50000400, "rp2040_dma.intr");
	nfail += test_address(&rp2040_dma.intcs[0].inte,				0x50000404, "rp2040_dma.inte0");
	nfail += test_address(&rp2040_dma.intcs[0].intf,				0x50000408, "rp2040_dma.intf0");
	nfail += test_address(&rp2040_dma.intcs[0].ints,				0x5000040c, "rp2040_dma.ints0");
	nfail += test_address(&rp2040_dma.intcs[1].inte,				0x50000414, "rp2040_dma.inte1");
	nfail += test_address(&rp2040_dma.intcs[1].intf,				0x50000418, "rp2040_dma.intf1");
	nfail += test_address(&rp2040_dma.intcs[1].ints,				0x5000041c, "rp2040_dma.ints1");

	nfail += test_address(&rp2040_dma.timer[0],						0x50000420, "rp2040_dma.timer0");
	nfail += test_address(&rp2040_dma.timer[1],						0x50000424, "rp2040_dma.timer1");
	nfail += test_address(&rp2040_dma.timer[2],						0x50000428, "rp2040_dma.timer2");
	nfail += test_address(&rp2040_dma.timer[3],						0x5000042c, "rp2040_dma.timer3");

	nfail += test_address(&rp2040_dma.multi_chan_trig,				0x50000430, "rp2040_dma.multi_chan_trigger");
	nfail += test_address(&rp2040_dma.sniff_ctrl,					0x50000434, "rp2040_dma.sniff_ctrl");
	nfail += test_address(&rp2040_dma.sniff_data,					0x50000438, "rp2040_dma.sniff_data");
	nfail += test_address(&rp2040_dma.fifo_levels,					0x50000440, "rp2040_dma.fifo_levels");
	nfail += test_address(&rp2040_dma.chan_abort,					0x50000444, "rp2040_dma.chan_abort");
	nfail += test_address(&rp2040_dma.n_channels,					0x50000448, "rp2040_dma.n_channels");

	nfail += test_address(&rp2040_dma.dbg[0].dbg_ctdreq,			0x50000800, "rp2040_dma.ch0_dbg_ctdreq");
	nfail += test_address(&rp2040_dma.dbg[0].dbg_tcr,				0x50000804, "rp2040_dma.ch0_dbg_tcr");

	/* It's sufficient to test the first register of ch1_dbg to make sure the structure
	 * is the right size. All the rest of the registers in ch1_dbg and all the rest of the channels
	 * can be inferred.
	*/
	nfail += test_address(&rp2040_dma.dbg[1].dbg_ctdreq,			0x50000840, "rp2040_dma.ch1_dbg_ctdreq");
	return nfail;
}

static int test_pio(void)
{
	int nfail = 0;
	nfail += test_address(&rp2040_pio0.ctrl,			0x50200000, "rp2040_pio0.ctrl");
	nfail += test_address(&rp2040_pio0.fstat,			0x50200004, "rp2040_pio0.fstat");
	nfail += test_address(&rp2040_pio0.fdebug,			0x50200008, "rp2040_pio0.fdebug");
	nfail += test_address(&rp2040_pio0.flevel,			0x5020000c, "rp2040_pio0.flevel");

	/* For array members it's sufficient to test the first element and the (first register of the)
	 * second element. The rest can be inferred
	*/
	nfail += test_address(&rp2040_pio0.txf[0],			0x50200010, "rp2040_pio0.txf[0]");
	nfail += test_address(&rp2040_pio0.txf[1],			0x50200014, "rp2040_pio0.txf[1]");
	nfail += test_address(&rp2040_pio0.rxf[0],			0x50200020, "rp2040_pio0.rxf[0]");
	nfail += test_address(&rp2040_pio0.rxf[1],			0x50200024, "rp2040_pio0.rxf[1]");

	nfail += test_address(&rp2040_pio0.irqf,			0x50200030, "rp2040_pio0.irqf");
	nfail += test_address(&rp2040_pio0.irq_force,		0x50200034, "rp2040_pio0.irq_force");
	nfail += test_address(&rp2040_pio0.ip_syn_byp,		0x50200038, "rp2040_pio0.ip_syn_byp");
	nfail += test_address(&rp2040_pio0.dbg_padout,		0x5020003c, "rp2040_pio0.dbg_padout");
	nfail += test_address(&rp2040_pio0.dbg_padoe,		0x50200040, "rp2040_pio0.dbg_padoe");
	nfail += test_address(&rp2040_pio0.dbg_cfginfo,		0x50200044, "rp2040_pio0.dbg_cfginfo");

	nfail += test_address(&rp2040_pio0.instr_mem[0],	0x50200048, "rp2040_pio0.instr_mem[0]");
	nfail += test_address(&rp2040_pio0.instr_mem[1],	0x5020004c, "rp2040_pio0.instr_mem[1]");

	nfail += test_address(&rp2040_pio0.sm[0].clkdiv,	0x502000c8, "rp2040_pio0.sm0_clkdiv");
	nfail += test_address(&rp2040_pio0.sm[0].execctrl,	0x502000cc, "rp2040_pio0.sm0_execctrl");
	nfail += test_address(&rp2040_pio0.sm[0].shiftctrl,	0x502000d0, "rp2040_pio0.sm0_shiftctrl");
	nfail += test_address(&rp2040_pio0.sm[0].addr,		0x502000d4, "rp2040_pio0.sm0_addr");
	nfail += test_address(&rp2040_pio0.sm[0].instr,		0x502000d8, "rp2040_pio0.sm0_instr");
	nfail += test_address(&rp2040_pio0.sm[0].pinctrl,	0x502000dc, "rp2040_pio0.sm0_pinctrl");

	nfail += test_address(&rp2040_pio0.sm[1].clkdiv,	0x502000e0, "rp2040_pio0.sm1_clkdiv");

	nfail += test_address(&rp2040_pio0.intr,			0x50200128, "rp2040_pio0.intr");

	nfail += test_address(&rp2040_pio0.irq[0].inte,		0x5020012c, "rp2040_pio0.irq0_inte");
	nfail += test_address(&rp2040_pio0.irq[0].intf,		0x50200130, "rp2040_pio0.irq0_intf");
	nfail += test_address(&rp2040_pio0.irq[0].ints,		0x50200134, "rp2040_pio0.irq0_ints");
	nfail += test_address(&rp2040_pio0.irq[1].inte,		0x50200138, "rp2040_pio0.irq1_inte");

	/* It's sufficient to test the first register of pio1 to make sure the base address is correct.
	 * All the rest of the registers in pio1 can be inferred.
	*/
	nfail += test_address(&rp2040_pio1.ctrl,			0x50300000, "rp2040_pio1.ctrl");
	return nfail;
}

static int test_resets(void)
{
	int nfail = 0;
	nfail += test_address(&rp2040_resets.reset,			0x4000c000, "rp2040_resets.reset");
	nfail += test_address(&rp2040_resets.wdsel,			0x4000c004, "rp2040_resets.wdsel");
	nfail += test_address(&rp2040_resets.done,			0x4000c008, "rp2040_resets.done");
	return nfail;
}

static int test_sio(void)
{
	int nfail = 0;
	nfail += test_address(&rp2040_sio.cpuid,				0xd0000000, "rp2040_sio.cpuid");
	nfail += test_address(&rp2040_sio.gpio_in,				0xd0000004, "rp2040_sio.gpio_in");
	nfail += test_address(&rp2040_sio.gpio_hi_in,			0xd0000008, "rp2040_sio.gpio_hi_in");

	nfail += test_address(&rp2040_sio.gpio_out.val,			0xd0000010, "rp2040_sio.gpio_out");
	nfail += test_address(&rp2040_sio.gpio_out.w1s,			0xd0000014, "rp2040_sio.gpio_out_set");
	nfail += test_address(&rp2040_sio.gpio_out.w1c,			0xd0000018, "rp2040_sio.gpio_out_clr");
	nfail += test_address(&rp2040_sio.gpio_out.xor,			0xd000001c, "rp2040_sio.gpio_out_xor");

	/* No need to test the atomic access ports for gpio_oe, gpio_hi_out and gpio_hi_oe.
	*/
	nfail += test_address(&rp2040_sio.gpio_oe.val,			0xd0000020, "rp2040_sio.gpio_oe");
	nfail += test_address(&rp2040_sio.gpio_hi_out.val,		0xd0000030, "rp2040_sio.gpio_hi_out");
	nfail += test_address(&rp2040_sio.gpio_hi_oe.val,		0xd0000040, "rp2040_sio.gpio_hi_oe");

	nfail += test_address(&rp2040_sio.fifo_st,				0xd0000050, "rp2040_sio.fifo_st");
	nfail += test_address(&rp2040_sio.fifo_wr,				0xd0000054, "rp2040_sio.fifo_wr");
	nfail += test_address(&rp2040_sio.fifo_rd,				0xd0000058, "rp2040_sio.fifo_rd");
	nfail += test_address(&rp2040_sio.spinlock_st,			0xd000005c, "rp2040_sio.spinlock_st");
	nfail += test_address(&rp2040_sio.div_udividend,		0xd0000060, "rp2040_sio.div_udividend");
	nfail += test_address(&rp2040_sio.div_udivisor,			0xd0000064, "rp2040_sio.div_udivisor");
	nfail += test_address(&rp2040_sio.div_sdividend,		0xd0000068, "rp2040_sio.div_sdividend");
	nfail += test_address(&rp2040_sio.div_sdivisor,			0xd000006c, "rp2040_sio.div_sdivisor");
	nfail += test_address(&rp2040_sio.div_quotient,			0xd0000070, "rp2040_sio.div_quotient");
	nfail += test_address(&rp2040_sio.div_remainder,		0xd0000074, "rp2040_sio.div_remainder");
	nfail += test_address(&rp2040_sio.div_csr,				0xd0000078, "rp2040_sio.div_csr");
	nfail += test_address(&rp2040_sio.interp[0].accum0,		0xd0000080, "rp2040_sio.interp[0].accum0");
	nfail += test_address(&rp2040_sio.interp[0].accum1,		0xd0000084, "rp2040_sio.interp[0].accum1");
	nfail += test_address(&rp2040_sio.interp[0].base0,		0xd0000088, "rp2040_sio.interp[0].base0");
	nfail += test_address(&rp2040_sio.interp[0].base1,		0xd000008c, "rp2040_sio.interp[0].base1");
	nfail += test_address(&rp2040_sio.interp[0].base2,		0xd0000090, "rp2040_sio.interp[0].base2");
	nfail += test_address(&rp2040_sio.interp[0].pop_lane0,	0xd0000094, "rp2040_sio.interp[0].pop_lane0");
	nfail += test_address(&rp2040_sio.interp[0].pop_lane1,	0xd0000098, "rp2040_sio.interp[0].pop_lane1");
	nfail += test_address(&rp2040_sio.interp[0].pop_full,	0xd000009c, "rp2040_sio.interp[0].pop_full");
	nfail += test_address(&rp2040_sio.interp[0].peek_lane0,	0xd00000a0, "rp2040_sio.interp[0].peek_lane0");
	nfail += test_address(&rp2040_sio.interp[0].peek_lane1,	0xd00000a4, "rp2040_sio.interp[0].peek_lane1");
	nfail += test_address(&rp2040_sio.interp[0].peek_full,	0xd00000a8, "rp2040_sio.interp[0].peek_full");
	nfail += test_address(&rp2040_sio.interp[0].ctrl_lane0,	0xd00000ac, "rp2040_sio.interp[0].ctrl_lane0");
	nfail += test_address(&rp2040_sio.interp[0].ctrl_lane1,	0xd00000b0, "rp2040_sio.interp[0].ctrl_lane1");
	nfail += test_address(&rp2040_sio.interp[0].accum0_add,	0xd00000b4, "rp2040_sio.interp[0].accum0_add");
	nfail += test_address(&rp2040_sio.interp[0].accum1_add,	0xd00000b8, "rp2040_sio.interp[0].accum1_add");
	nfail += test_address(&rp2040_sio.interp[0].base_1and0,	0xd00000bc, "rp2040_sio.interp[0].base_1and0");

	/* No need to test the all the registers in interp1.
	*/
	nfail += test_address(&rp2040_sio.interp[1].accum0,	0xd00000c0, "rp2040_sio.interp[1].accum0");

	nfail += test_address(&rp2040_sio.spinlock[0],		0xd0000100, "rp2040_sio.spinlock[0]");
	nfail += test_address(&rp2040_sio.spinlock[1],		0xd0000104, "rp2040_sio.spinlock[1]");
	nfail += test_address(&rp2040_sio.spinlock[31],		0xd000017c, "rp2040_sio.spinlock[31]");
	return nfail;
}

#if 0
/* Template peripheral test
*/
static int test_per(void)
{
	int nfail = 0;
	nfail += test_address(&rp2040_xxx.rr,			0xdeadbeef, "rp2040_xxx.rr");
	return nfail;
}
#endif


static int test_address(volatile void *p, u32_t v, char *name)
{
	u32_t addr = (u32_t)(u64_t)p;
	if ( addr == v )
		return 0;

	printf("%s has wrong address 0x%08x\n", name, addr);
	return 1;
}

