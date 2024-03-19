/* cxm-vectors.c - vector table for Cortex-M devices
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
extern void rp2040_stacktop(void);	/* This is a blatant lie! rp2040_stacktop is a symbol set in the linker script */
extern void rp2040_kickstart(void);
extern void app_nmi(void);
extern void app_hardfault(void);

/* Cortex M0/M0+ doesn't have memfault, busfault or usage traps. Everything goes to hardfault
*/
#define app_memfault	app_unknowntrap
#define app_busfault	app_unknowntrap
#define app_usagefault	app_unknowntrap

extern void app_svctrap(void);
extern void app_pendsvtrap(void);
extern void app_systickirq(void);
extern void app_irq(void);
extern void app_unknowntrap(void);

typedef void (*rp2040_vector_t)(void);

/* The vector table is an array of addresses.
 * This table has (16+32) vectors:
 *	- 16 are the armvx-m exception/reset vectors (including reset SP)
 *	- 32 are the NVIC vectors
*/
const rp2040_vector_t rp2040_hwvectors[16+32] =
{	&rp2040_stacktop,
	&rp2040_kickstart,
	&app_nmi,
	&app_hardfault,
	&app_memfault,
	&app_busfault,
	&app_usagefault,
	&app_unknowntrap,
	&app_unknowntrap,
	&app_unknowntrap,
	&app_unknowntrap,
	&app_svctrap,
	&app_unknowntrap,
	&app_unknowntrap,
	&app_pendsvtrap,
	&app_systickirq,
	&app_irq,	/* 00 */
	&app_irq,	/* 01 */
	&app_irq,	/* 02 */
	&app_irq,	/* 03 */
	&app_irq,	/* 04 */
	&app_irq,	/* 05 */
	&app_irq,	/* 06 */
	&app_irq,	/* 07 */
	&app_irq,	/* 08 */
	&app_irq,	/* 09 */
	&app_irq,	/* 10 */
	&app_irq,	/* 11 */
	&app_irq,	/* 12 */
	&app_irq,	/* 13 */
	&app_irq,	/* 14 */
	&app_irq,	/* 15 */
	&app_irq,	/* 16 */
	&app_irq,	/* 17 */
	&app_irq,	/* 18 */
	&app_irq,	/* 19 */
	&app_irq,	/* 20 */
	&app_irq,	/* 21 */
	&app_irq,	/* 22 */
	&app_irq,	/* 23 */
	&app_irq,	/* 24 */
	&app_irq,	/* 25 */
	&app_irq,	/* 26 */
	&app_irq,	/* 27 */
	&app_irq,	/* 28 */
	&app_irq,	/* 29 */
	&app_irq,	/* 30 */
	&app_irq,	/* 31 */
};
