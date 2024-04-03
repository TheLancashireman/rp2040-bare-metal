/* rp2040-boot1.S
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
	.extern		rp2040_stacktop1		/* Set in linker script */
	.extern		rp2040_kickstart1
	.globl		rp2040_boot1

/* The RP2040 usually uses some kind of boot loader. Either:
 *	- the application is copied to RAM by a custom boot loader or
 *	- the application is loaded directly to RAM by the UF2 loader or
 *	- the application runs from QSPI flash
 * In all cases, the normal Cortex-M vectors are used by the boot flash, so
 * setting the initial SP from the vectors is not an option.
*/

rp2040_boot1:
    ldr		r0,=rp2040_stacktop1
    mov		sp,r0
    b		rp2040_kickstart1			/* Must not return! */
