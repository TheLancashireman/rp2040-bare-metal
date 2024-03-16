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

static int test_sysinfo(void);
static int test_adc(void);
static int test_dma(void);
static int test_address(volatile void *p, u32_t v, char *name);

int main(int argc, char **argv)
{
	int nfail = 0;

	nfail += test_sysinfo();

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

