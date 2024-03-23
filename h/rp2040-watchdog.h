/* rp2040-watchdog.h - header file for RP2040 watchdog
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
#ifndef RP2040_WATCHDOG_H
#define RP2040_WATCHDOG_H		1

#include "rp2040.h"
#include "rp2040-types.h"

typedef struct rp2040_watchdog_s rp2040_watchdog_t;

/* The armed and intx registers have bits for each alarm ( 1<<alarm_index )
 * The alarm registers are one-shot - they arm on write and automatically disarm in match with
 * time_l.
*/
struct rp2040_watchdog_s
{
	reg32_t	ctrl;			/* 0x00 - watchdog control */
	reg32_t	load;			/* 0x04 - load the watchdog timer */
	reg32_t	reason;			/* 0x08 - reason for the last reset */
	reg32_t	scratch[8];		/* 0x0c - scratch registers that survive a soft reset */
	reg32_t	tick;			/* 0x2c - controls the tick generator */
};

#define WATCHDOG_BASE		0x40058000
#define rp2040_watchdog		(((rp2040_watchdog_t *)(WATCHDOG_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_watchdog_xor	(((rp2040_watchdog_t *)(WATCHDOG_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_watchdog_w1s	(((rp2040_watchdog_t *)(WATCHDOG_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_watchdog_w1c	(((rp2040_watchdog_t *)(WATCHDOG_BASE+RP2040_OFFSET_W1C))[0])

#define TICK_COUNT			0x000ff800
#define TICK_RUNNING		0x00000400
#define TICK_ENABLED		0x00000200
#define TICK_DIV			0x000001ff

#define WATCHDOG_TRIGGER	0x80000000
#define WATCHDOG_ENABLED	0x40000000

static inline void rp2040_watchdog_disable(void)
{
	/* Clear the ENABLED bit
	*/
	rp2040_watchdog_w1c.ctrl = WATCHDOG_ENABLED;
}

static inline void rp2040_tick_init(void)
{
	rp2040_watchdog.tick = 0;					/* Clear out the old stuff */
	rp2040_watchdog.tick;						/* Read back to force the write */
	rp2040_watchdog.tick = TICK_ENABLED | 12;	/* Divide by 12 make the timer a "standard" microsecond timer */

	while ( (rp2040_watchdog.tick & TICK_RUNNING) == 0 ) 
	{
		/* Wait */
	}
}

#endif
