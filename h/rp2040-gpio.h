/* rp2040-gpio.h - header file for RP2040 GPIO
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
#ifndef RP2040_GPIO_H
#define RP2040_GPIO_H

#include "rp2040.h"
#include "rp2040-types.h"

/* I/O user bank 0
 *
 * This peripheral controls the function selection for each GPIO pin.
*/
typedef struct rp2040_gpio_sc_s rp2040_gpio_sc_t;
typedef struct rp2040_io_intctl_s rp2040_io_intctl_t;
typedef struct rp2040_iobank0_s rp2040_iobank0_t;

/* Pin status and control registers
*/
struct rp2040_gpio_sc_s
{
	reg32_t	status;
	reg32_t	ctrl;
};

/* Interrupt control and status
*/
struct rp2040_io_intctl_s
{
	reg32_t	inte[4];		/* 0x000 - interrupt enable */
	reg32_t	intf[4];		/* 0x010 - interrupt force */
	reg32_t	ints[4];		/* 0x020 - interrupt cooked status */
};

/* IOBANK0
*/
struct rp2040_iobank0_s
{
	rp2040_gpio_sc_t	gpio[30];		/* 0x000 - GPIO 0..29 control and status */
	reg32_t				intr[4];		/* 0x0f0 - raw interrupt status */
	rp2040_io_intctl_t	proc_intctl[2];	/* 0x100 - interrupt control for each core */
	rp2040_io_intctl_t	dormant_wake;	/* 0x160 - interrupt control for dormant wake */
};

#define IOBANK0_BASE		0x40014000
#define rp2040_iobank0		(((rp2040_iobank0_t *)(IOBANK0_BASE+RP2040_OFFSET_REG))[0])
#define rp2040_iobank0_xor	(((rp2040_iobank0_t *)(IOBANK0_BASE+RP2040_OFFSET_XOR))[0])
#define rp2040_iobank0_w1s	(((rp2040_iobank0_t *)(IOBANK0_BASE+RP2040_OFFSET_W1S))[0])
#define rp2040_iobank0_w1c	(((rp2040_iobank0_t *)(IOBANK0_BASE+RP2040_OFFSET_W1C))[0])

/* Bits 0..4 of gpio[n].ctrl select the function.
 *
 * The exact meaning (which peripheral, which signal) depends on the pin.
 * See section 2.19.2 in the RP2040 ref. manual for full details.
*/
#define FUNCSEL_SPI		1
#define FUNCSEL_UART	2
#define FUNCSEL_I2C		3
#define FUNCSEL_PWM		4
#define FUNCSEL_SIO		5
#define FUNCSEL_PIO0	6
#define FUNCSEL_PIO1	7
#define FUNCSEL_CLOCK	8
#define FUNCSEL_USB		9
#define FUNCSEL_NULL	31

/* Override fields in gpio[n].ctrl
*/
#define IRQOVER_INT		0x00000000		/* Normal interrupt */
#define IRQOVER_INVINT	0x10000000		/* Inverted interrupt */
#define IRQOVER_LOW		0x20000000		/* Drive interrupt low */
#define IRQOVER_HIGH	0x30000000		/* Drive interrupt high */

#define INOVER_PERI		0x00000000		/* Normal peripheral input */
#define INOVER_INVPERI	0x00010000		/* Invert the peripheral input */
#define INOVER_LOW		0x00020000		/* Drive peripheral input low */
#define INOVER_HIGH		0x00030000		/* Drive peripheral input high */

#define OEOVER_PERI		0x00000000		/* Drive output enable from peripheral signal */
#define OEOVER_INVPERI	0x00001000		/* Drive output enable from inverted peripheral signal */
#define OEOVER_DISABLE	0x00002000		/* Disable output */
#define OEOVER_ENABLE	0x00003000		/* Enable output */

#define OUTOVER_PERI	0x00000000		/* Drive output from peripheral signal */
#define OUTOVER_INVPERI	0x00000100		/* Drive output from inverse of peripheral signal */
#define OUTOVER_LOW		0x00000200		/* Drive output low */
#define OUTOVER_HIGH	0x00000300		/* Drive output high */

#endif
