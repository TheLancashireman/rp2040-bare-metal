/* rp2040-nvic.h - header file for the rp2040 NVIC
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
#ifndef RP2040_NVIC_H
#define RP2040_NVIC_H		1

#include "rp2040.h"
#include "rp2040-types.h"

/* Interrupt priorities
 * These values must be shifted left by 0, 8, 16 or 24 bits depending on irqid % 4
*/
#define	NVIC_PRIO_0			0x00	/* Highest */
#define NVIC_PRIO_1			0x40
#define NVIC_PRIO_2			0x80
#define NVIC_PRIO_3			0xc0	/* Lowest */

/* Interrupt indexes
*/
typedef enum irqid_e
{
	irq_timer0,			/* 00 - TIMER_IRQ_0 */
	irq_timer1,			/* 01 - TIMER_IRQ_1 */
	irq_timer2,			/* 02 - TIMER_IRQ_2 */
	irq_timer3,			/* 03 - TIMER_IRQ_3 */
	irq_pwm_wrap,		/* 04 - PWM_IRQ_WRAP */
	irq_usbctrl,		/* 05 - USBCTRL_IRQ */
	irq_xip,			/* 06 - XIP_IRQ */
	irq_pio0_0,			/* 07 - PIO0_IRQ_0 */
	irq_pio0_1,			/* 08 - PIO0_IRQ_1 */
	irq_pio1_0,			/* 09 - PIO1_IRQ_0 */
	irq_pio1_1,			/* 10 - PIO1_IRQ_1 */
	irq_dma_0,			/* 11 - DMA_IRQ_0 */
	irq_dma_1,			/* 12 - DMA_IRQ_1 */
	irq_io_bank0,		/* 13 - IO_IRQ_BANK0 */
	irq_io_qspi,		/* 14 - IO_IRQ_QSPI */
	irq_sio_proc0,		/* 15 - SIO_IRQ_PROC0 */
	irq_sio_proc1,		/* 16 - SIO_IRQ_PROC1 */
	irq_clocks,			/* 17 - CLOCKS_IRQ */
	irq_spi0,			/* 18 - SPI0_IRQ */
	irq_spi1,			/* 19 - SPI1_IRQ */
	irq_uart0,			/* 20 - UART0_IRQ */
	irq_uart1,			/* 21 - UART1_IRQ */
	irq_adc_fifo,		/* 22 - ADC_IRQ_FIFO */
	irq_i2c0,			/* 23 - I2C0_IRQ */
	irq_i2c1,			/* 24 - I2C1_IRQ */
	irq_rtc,			/* 25 - RTC_IRQ */
	irq_26,				/* 26 - No hardware; can be software-triggered */
	irq_27,				/* 27 - No hardware; can be software-triggered */
	irq_28,				/* 28 - No hardware; can be software-triggered */
	irq_29,				/* 29 - No hardware; can be software-triggered */
	irq_30,				/* 30 - No hardware; can be software-triggered */
	irq_31,				/* 31 - No hardware; can be software-triggered */
	nvic_nirq			/* 32 - no. of interrupts (must be last) */
} irqid_t;

/* Register structure for NVIC.
 * This structure describes a generalised NVIC. On the RP2040 (Cortex-M0+) only the first register
 * of each set of 32 and the first 8 ipr[] registers are implemented.
*/
typedef struct nvic_s nvic_t;

struct nvic_s
{
	reg32_t iser[32];		/* Write 1 to a bit to enable an IRQ; Write 0 has no effect */
	reg32_t icer[32];		/* Write 1 to a bit to disable an IRQ; Write 0 has no effect */
	reg32_t ispr[32];		/* Write 1 to a bit to set an IRQ's pending flag; Write 0 has no effect */
	reg32_t icpr[32];		/* Write 1 to a bit to clear an IRQ's pending flag; Write 0 has no effect */
	reg32_t iabr[32];		/* Read-only. a bit at 1 indicates interrupt is active */
							/* Note: in the above register arrays, registers 1..31 are reserved */
	u32_t reserved[32];
	reg32_t ipr[256];		/* Interrupt priorities; 8 bits per IRQ; registers 16..127 are reserved */
};

#define NVIC_BASE			0xe000e100
#define rp2040_nvic			((nvic_t *)NVIC_BASE)[0]

#endif
