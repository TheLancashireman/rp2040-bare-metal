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

/* Inhibit inclusion of rp2040-types.h and define our own for the host
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

int test_address(void *p, u32_t v, char *name)
{
	u32_t addr = (u32_t)(u64_t)p;
	if ( addr == v )
		return 0;

	printf("%s has wrong address 0x%08x\n", name, addr);
	return 1;
}

int main(int argc, char **argv)
{
	int nfail = 0;

	nfail += test_address(&rp2040_sysinfo.chip_id, 0x40000000, "rp2040_sysinfo.chip_id");
	nfail += test_address(&rp2040_sysinfo.platform, 0x40000004, "rp2040_sysinfo.platform");
	nfail += test_address(&rp2040_sysinfo.gitref_rp2040, 0x40000040, "rp2040_sysinfo.gitref_rp2040");

	if ( nfail == 0 )
		printf("Pass\n");

	return 0;
}
