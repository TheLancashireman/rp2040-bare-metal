/* rp2040-startup1.c - startup functions for RP2040 core 1
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
#include "rp2040.h"
#include "rp2040-types.h"
#include "rp2040-cm0.h"

#define SPSEL		0x02

extern unsigned rp2040_pstacktop1;
extern unsigned rp2040_stacktop1;
extern int main1(void);

/* rp2040_kickstart1() - entry point from the boot code
 *
 * SP has been initialised.
 * Common hardware has been initialised by core 0
*/
void rp2040_kickstart1(void)
{
	/* Set up the vector table
    */
#if 0
    cxm_scr.vtor = (u32_t)&cxm_vectors[0];
#endif

	/* Initialise the exception priorities
	*/
	cxm_scr.shpr[1] = 0x0;			/* SVC and [reserved x 3] all at highest priority */
	cxm_scr.shpr[2] = 0xffff0000;	/* SysTick/PendSV at lowest priority, Debug and [reserved] at highest */

	/* Initialise the interrupt controller
	*/
#if 0
	rp2040_nvic_init();
#endif

	/* Switch to the process stack pointer and simultaneously jump to main()
	*/
	rp2040_switch_to_psp((u32_t)&rp2040_pstacktop1, (u32_t)&rp2040_stacktop1,
						(u32_t)(cxm_get_control() | SPSEL), (u32_t)&main1);
}
