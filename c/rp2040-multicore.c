/* rp2040-multicore.c - multicore functions for RP2040
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
#include "rp2040-sio.h"

/* Debugging. Remove later */
#ifdef DEBUG
#include "test-io.h"

#define DBG_PUTC(x)			dh_putc(x)
#define DBG_PUTS(x)			dh_puts(x)
#define DBG_PUTX(x)			dh_putx32(x)
#define DBG_MSGVAL(m, x)	do { dh_puts(m); dh_putx32(x);	} while (0)

#else

#define DBG_PUTC(x)			do {} while (0)
#define DBG_PUTS(x)			do {} while (0)
#define DBG_PUTX(x)			do {} while (0)
#define DBG_MSGVAL(m, x)	do {} while (0)

#endif

#define START_SEQ_LEN	6

extern u32_t rp2040_stacktop1, rp2040_entry1;	/* Dummy types - these are linker symbols */

static const u32_t start_seq[START_SEQ_LEN] =
{	0, 0, 1,						/* Synchronisation sequence */
	0x20000000,						/* Vector table (dummy) */
	(u32_t)&rp2040_stacktop1,		/* Initial stack pointer (defined by linker script) */
	(u32_t)&rp2040_entry1			/* Entry point (defined by linker script) */
};

/* rp2040_start_core1() - wake up core 1
 *
 * This code is based on the example from the RP2040 data sheet
 * Section 2.8.2. "Launching Code On Processor Core 1"
*/
void rp2040_start_core1(void)
{
	u32_t cmd, resp;

	do {
		for ( int i = 0; i < START_SEQ_LEN; i++ )
		{
			cmd = start_seq[i];

			DBG_MSGVAL("cmd = ", cmd);

			if ( cmd == 0 )
			{
				/* Empty the rx fifo. Always do this when the command is 0
				*/
				while ( (rp2040_sio.fifo_st & SIO_FIFO_VLD) != 0 )
				{
					DBG_PUTC('a');
					rp2040_sio.fifo_rd;		/* Volatile qualifier means read is mandatory */
				}

				DBG_PUTC('\n');
				__asm__ volatile("sev" : : : "memory");
			}

			while ( (rp2040_sio.fifo_st & SIO_FIFO_RDY) == 0 )
			{
				DBG_PUTC('b');
				/* Wait until the tx fifo has space */
			}

			DBG_PUTC('\n');
			rp2040_sio.fifo_wr = cmd;	/* Send the command */

			__asm__ volatile("sev" : : : "memory");		/* Not shown in data sheet but appears to be needed */

			while ( (rp2040_sio.fifo_st & SIO_FIFO_VLD) == 0 )
			{
				DBG_PUTC('c');
				/* Wait for the response */
			}

			DBG_PUTC('\n');
			resp = rp2040_sio.fifo_rd;	/* Read the response */

			if ( resp != cmd )
			{
				DBG_MSGVAL("Unexpected response: ", resp);
				break;
			}
		}
	} while ( resp != cmd );
}
