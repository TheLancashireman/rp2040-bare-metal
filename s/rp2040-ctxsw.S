/* rp2040-ctxsw.S - context switch function for Cortex-M CPUs
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
 *
 *
 *
*/
	.text
	.globl	rp2040_switch_to_psp

/* void rp2040_switchToPsp(u32_t psp, u32_t msp, u32_t control, u32_t fp);
 *
 * Switches to the PSP and calls a function.
*/
rp2040_switch_to_psp:
	msr		PSP, r0
	msr		CONTROL, r2
	msr		MSP, r1
	bx		r3
