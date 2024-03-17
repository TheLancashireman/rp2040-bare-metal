/* header-test.c - compile test for header files
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
#include "rp2040-clocks.h"
#include "rp2040-dma.h"
#include "rp2040-gpio.h"
#include "rp2040-pio.h"
#include "rp2040-resets.h"
#include "rp2040-sio.h"
#include "rp2040-timer.h"

static int test_sysinfo(void);
static int test_adc(void);
static int test_clocks(void);
static int test_dma(void);
static int test_gpio(void);
static int test_pio(void);
static int test_resets(void);
static int test_sio(void);
static int test_timer(void);
static int test_address(volatile void *p, u32_t v, char *name);

int main(int argc, char **argv)
{
	int nfail = 0;

	nfail += test_sysinfo();
	nfail += test_adc();
	nfail += test_clocks();
	nfail += test_dma();
	nfail += test_gpio();
	nfail += test_pio();
	nfail += test_resets();
	nfail += test_sio();
	nfail += test_timer();

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
	nfail += test_address(&rp2040_sio.interp[1].accum0,		0xd00000c0, "rp2040_sio.interp[1].accum0");

	nfail += test_address(&rp2040_sio.spinlock[0],			0xd0000100, "rp2040_sio.spinlock[0]");
	nfail += test_address(&rp2040_sio.spinlock[1],			0xd0000104, "rp2040_sio.spinlock[1]");
	nfail += test_address(&rp2040_sio.spinlock[31],			0xd000017c, "rp2040_sio.spinlock[31]");
	return nfail;
}

static int test_clocks(void)
{
	int nfail = 0;
	nfail += test_address(&rp2040_clocks.gpout[0].ctrl,		0x40008000, "rp2040_clocks.gpout[0].ctrl");
	nfail += test_address(&rp2040_clocks.gpout[0].div,		0x40008004, "rp2040_clocks.gpout[0].div");
	nfail += test_address(&rp2040_clocks.gpout[0].selected,	0x40008008, "rp2040_clocks.gpout[0].selected");
	nfail += test_address(&rp2040_clocks.gpout[1].ctrl,		0x4000800c, "rp2040_clocks.gpout[1].ctrl");
	nfail += test_address(&rp2040_clocks.ref.ctrl,			0x40008030, "rp2040_clocks.ref.ctrl");
	nfail += test_address(&rp2040_clocks.sys.ctrl,			0x4000803c, "rp2040_clocks.sys.ctrl");
	nfail += test_address(&rp2040_clocks.peri.ctrl,			0x40008048, "rp2040_clocks.peri.ctrl");
	nfail += test_address(&rp2040_clocks.usb.ctrl,			0x40008054, "rp2040_clocks.usb.ctrl");
	nfail += test_address(&rp2040_clocks.adc.ctrl,			0x40008060, "rp2040_clocks.adc.ctrl");
	nfail += test_address(&rp2040_clocks.rtc.ctrl,			0x4000806c, "rp2040_clocks.rtc.ctrl");
	nfail += test_address(&rp2040_clocks.sys_resus_ctrl,	0x40008078, "rp2040_clocks.sys_resus_ctrl");
	nfail += test_address(&rp2040_clocks.sys_resus_status,	0x4000807c, "rp2040_clocks.sys_resus_status");
	nfail += test_address(&rp2040_clocks.fc0_ref_khz,		0x40008080, "rp2040_clocks.fc0_ref_khz");
	nfail += test_address(&rp2040_clocks.fc0_min_khz,		0x40008084, "rp2040_clocks.fc0_min_khz");
	nfail += test_address(&rp2040_clocks.fc0_max_khz,		0x40008088, "rp2040_clocks.fc0_max_khz");
	nfail += test_address(&rp2040_clocks.fc0_delay,			0x4000808c, "rp2040_clocks.fc0_delay");
	nfail += test_address(&rp2040_clocks.fc0_interval,		0x40008090, "rp2040_clocks.fc0_interval");
	nfail += test_address(&rp2040_clocks.fc0_src,			0x40008094, "rp2040_clocks.fc0_src");
	nfail += test_address(&rp2040_clocks.fc0_status,		0x40008098, "rp2040_clocks.fc0_status");
	nfail += test_address(&rp2040_clocks.fc0_result,		0x4000809c, "rp2040_clocks.fc0_result");
	nfail += test_address(&rp2040_clocks.wake_en[0],		0x400080a0, "rp2040_clocks.wake_en[0]");
	nfail += test_address(&rp2040_clocks.wake_en[1],		0x400080a4, "rp2040_clocks.wake_en[1]");
	nfail += test_address(&rp2040_clocks.sleep_en[0],		0x400080a8, "rp2040_clocks.sleep_en[0]");
	nfail += test_address(&rp2040_clocks.sleep_en[1],		0x400080ac, "rp2040_clocks.sleep_en[1]");
	nfail += test_address(&rp2040_clocks.enabled[0],		0x400080b0, "rp2040_clocks.enabled[0]");
	nfail += test_address(&rp2040_clocks.enabled[1],		0x400080b4, "rp2040_clocks.enabled[1]");
	nfail += test_address(&rp2040_clocks.intcs.intr,		0x400080b8, "rp2040_clocks.intcs.intr");
	nfail += test_address(&rp2040_clocks.intcs.inte,		0x400080bc, "rp2040_clocks.intcs.inte");
	nfail += test_address(&rp2040_clocks.intcs.intf,		0x400080c0, "rp2040_clocks.intcs.intf");
	nfail += test_address(&rp2040_clocks.intcs.ints,		0x400080c4, "rp2040_clocks.intcs.ints");

	nfail += test_address(&rp2040_xosc.ctrl,				0x40024000, "rp2040_xosc.ctrl");
	nfail += test_address(&rp2040_xosc.status,				0x40024004, "rp2040_xosc.status");
	nfail += test_address(&rp2040_xosc.dormant,				0x40024008, "rp2040_xosc.dormant");
	nfail += test_address(&rp2040_xosc.startup,				0x4002400c, "rp2040_xosc.startup");
	nfail += test_address(&rp2040_xosc.count,				0x4002401c, "rp2040_xosc.count");

	nfail += test_address(&rp2040_pll.cs,					0x40028000, "rp2040_pll.cs");
	nfail += test_address(&rp2040_pll.pwr,					0x40028004, "rp2040_pll.pwr");
	nfail += test_address(&rp2040_pll.fbdiv_int,			0x40028008, "rp2040_pll.fbdiv_int");
	nfail += test_address(&rp2040_pll.prim,					0x4002800c, "rp2040_pll.prim");

	nfail += test_address(&rp2040_usbpll.cs,				0x4002c000, "rp2040_usbpll.cs");
	nfail += test_address(&rp2040_usbpll.pwr,				0x4002c004, "rp2040_usbpll.pwr");
	nfail += test_address(&rp2040_usbpll.fbdiv_int,			0x4002c008, "rp2040_usbpll.fbdiv_int");
	nfail += test_address(&rp2040_usbpll.prim,				0x4002c00c, "rp2040_usbpll.prim");
	return nfail;
}

static int test_gpio(void)
{
	int nfail = 0;
	nfail += test_address(&rp2040_iobank0.gpio[0].status,	0x40014000, "iobank0.gpio[0].status");
	nfail += test_address(&rp2040_iobank0.gpio[0].ctrl,		0x40014004, "iobank0.gpio[0].ctrl");
	nfail += test_address(&rp2040_iobank0.gpio[1].status,	0x40014008, "iobank0.gpio[1].status");
	nfail += test_address(&rp2040_iobank0.gpio[1].ctrl,		0x4001400c, "iobank0.gpio[1].ctrl");
	nfail += test_address(&rp2040_iobank0.gpio[29].status,	0x400140e8, "iobank0.gpio[29].status");
	nfail += test_address(&rp2040_iobank0.gpio[29].ctrl,	0x400140ec, "iobank0.gpio[29].ctrl");
	nfail += test_address(&rp2040_iobank0.intr[0],			0x400140f0, "iobank0.intr[0]");
	nfail += test_address(&rp2040_iobank0.intr[1],			0x400140f4, "iobank0.intr[1]");
	nfail += test_address(&rp2040_iobank0.intr[2],			0x400140f8, "iobank0.intr[2]");
	nfail += test_address(&rp2040_iobank0.intr[3],			0x400140fc, "iobank0.intr[3]");

	nfail += test_address(&rp2040_iobank0.proc_intctl[0].inte[0],	0x40014100, "iobank0.proc_intctl[0].inte[0]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].inte[1],	0x40014104, "iobank0.proc_intctl[0].inte[1]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].inte[2],	0x40014108, "iobank0.proc_intctl[0].inte[2]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].inte[3],	0x4001410c, "iobank0.proc_intctl[0].inte[3]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].intf[0],	0x40014110, "iobank0.proc_intctl[0].intf[0]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].intf[1],	0x40014114, "iobank0.proc_intctl[0].intf[1]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].intf[2],	0x40014118, "iobank0.proc_intctl[0].intf[2]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].intf[3],	0x4001411c, "iobank0.proc_intctl[0].intf[3]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].ints[0],	0x40014120, "iobank0.proc_intctl[0].ints[0]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].ints[1],	0x40014124, "iobank0.proc_intctl[0].ints[1]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].ints[2],	0x40014128, "iobank0.proc_intctl[0].ints[2]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[0].ints[3],	0x4001412c, "iobank0.proc_intctl[0].ints[3]");

	nfail += test_address(&rp2040_iobank0.proc_intctl[1].inte[0],	0x40014130, "iobank0.proc_intctl[1].inte[0]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].inte[1],	0x40014134, "iobank0.proc_intctl[1].inte[1]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].inte[2],	0x40014138, "iobank0.proc_intctl[1].inte[2]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].inte[3],	0x4001413c, "iobank0.proc_intctl[1].inte[3]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].intf[0],	0x40014140, "iobank0.proc_intctl[1].intf[0]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].intf[1],	0x40014144, "iobank0.proc_intctl[1].intf[1]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].intf[2],	0x40014148, "iobank0.proc_intctl[1].intf[2]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].intf[3],	0x4001414c, "iobank0.proc_intctl[1].intf[3]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].ints[0],	0x40014150, "iobank0.proc_intctl[1].ints[0]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].ints[1],	0x40014154, "iobank0.proc_intctl[1].ints[1]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].ints[2],	0x40014158, "iobank0.proc_intctl[1].ints[2]");
	nfail += test_address(&rp2040_iobank0.proc_intctl[1].ints[3],	0x4001415c, "iobank0.proc_intctl[1].ints[3]");

	nfail += test_address(&rp2040_iobank0.dormant_wake.inte[0],		0x40014160, "iobank0.dormant_wake.inte[0]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.inte[1],		0x40014164, "iobank0.dormant_wake.inte[1]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.inte[2],		0x40014168, "iobank0.dormant_wake.inte[2]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.inte[3],		0x4001416c, "iobank0.dormant_wake.inte[3]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.intf[0],		0x40014170, "iobank0.dormant_wake.intf[0]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.intf[1],		0x40014174, "iobank0.dormant_wake.intf[1]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.intf[2],		0x40014178, "iobank0.dormant_wake.intf[2]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.intf[3],		0x4001417c, "iobank0.dormant_wake.intf[3]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.ints[0],		0x40014180, "iobank0.dormant_wake.ints[0]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.ints[1],		0x40014184, "iobank0.dormant_wake.ints[1]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.ints[2],		0x40014188, "iobank0.dormant_wake.ints[2]");
	nfail += test_address(&rp2040_iobank0.dormant_wake.ints[3],		0x4001418c, "iobank0.dormant_wake.ints[3]");
	return nfail;
}

static int test_timer(void)
{
	int nfail = 0;
	nfail += test_address(&rp2040_timer.time_hw,		0x40054000, "rp2040_timer.time_hw");
	nfail += test_address(&rp2040_timer.time_lw,		0x40054004, "rp2040_timer.time_lw");
	nfail += test_address(&rp2040_timer.time_hr,		0x40054008, "rp2040_timer.time_hr");
	nfail += test_address(&rp2040_timer.time_lr,		0x4005400c, "rp2040_timer.time_lr");
	nfail += test_address(&rp2040_timer.alarm[0],		0x40054010, "rp2040_timer.alarm[0]");
	nfail += test_address(&rp2040_timer.alarm[1],		0x40054014, "rp2040_timer.alarm[1]");
	nfail += test_address(&rp2040_timer.alarm[2],		0x40054018, "rp2040_timer.alarm[2]");
	nfail += test_address(&rp2040_timer.alarm[3],		0x4005401c, "rp2040_timer.alarm[3]");
	nfail += test_address(&rp2040_timer.armed,			0x40054020, "rp2040_timer.armed");
	nfail += test_address(&rp2040_timer.time_hraw,		0x40054024, "rp2040_timer.time_hraw");
	nfail += test_address(&rp2040_timer.time_lraw,		0x40054028, "rp2040_timer.time_lraw");
	nfail += test_address(&rp2040_timer.dbgpause,		0x4005402c, "rp2040_timer.dbgpause");
	nfail += test_address(&rp2040_timer.pause,			0x40054030, "rp2040_timer.dbgpause");
	nfail += test_address(&rp2040_timer.intcs.intr,		0x40054034, "rp2040_timer.intcs.intr");
	nfail += test_address(&rp2040_timer.intcs.inte,		0x40054038, "rp2040_timer.intcs.inte");
	nfail += test_address(&rp2040_timer.intcs.intf,		0x4005403c, "rp2040_timer.intcs.intf");
	nfail += test_address(&rp2040_timer.intcs.ints,		0x40054040, "rp2040_timer.intcs.ints");
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

