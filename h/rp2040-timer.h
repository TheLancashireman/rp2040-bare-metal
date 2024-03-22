/* rp2040-timer.h - header file for RP2040 timer
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
#ifndef RP2040_TIMER_H
#define RP2040_TIMER_H		1

#include "rp2040.h"
#include "rp2040-types.h"

typedef struct rp2040_timer_s rp2040_timer_t;

/* The armed and intx registers have bits for each alarm ( 1<<alarm_index )
 * The alarm registers are one-shot - they arm on write and automatically disarm in match with
 * time_l.
*/
struct rp2040_timer_s
{
	reg32_t	time_hw;		/* 0x00 - write high word and transfer low word buffer */
	reg32_t	time_lw;		/* 0x04 - write low word buffer */
	reg32_t	time_hr;		/* 0x08 - read latched high word */
	reg32_t	time_lr;		/* 0x0c - read low word and latch high word */
	reg32_t	alarm[4];		/* 0x10 - alarm registers */
	reg32_t	armed;			/* 0x20 - armed state of alarms */
	reg32_t	time_hraw;		/* 0x24 - read high word */
	reg32_t	time_lraw;		/* 0x28 - read low word */
	reg32_t	dbgpause;		/* 0x2c - pause timer while debugging */
	reg32_t	pause;			/* 0x30 - pause timer in software */
	rp2040_intcs_t intcs;	/* 0x34 - interrupt control/status  */
};

#define TIMER_BASE			0x40054000
#define rp2040_timer		(((rp2040_timer_t *)(TIMER_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_timer_xor	(((rp2040_timer_t *)(TIMER_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_timer_w1s	(((rp2040_timer_t *)(TIMER_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_timer_w1c	(((rp2040_timer_t *)(TIMER_BASE+RP2040_OFFSET_W1C))[0])

/* rp2040_read_time() - read and return the timer value
 *
 * The time_hr/_lr registers suffer from the usual concurrency issues, so this function
 * uses a standard high-low-high algorithm reading the raw registers.
*/
u64_t rp2040_read_time(void)
{
	u32_t h1, h2, l;

	do {
		h1 = rp2040_timer.time_hraw;
		l  = rp2040_timer.time_lraw;
		h2 = rp2040_timer.time_hraw;
	} while ( h1 != h2 );

	return (((u64_t)h1) << 32) + ((u64_t)l);
}

#endif
