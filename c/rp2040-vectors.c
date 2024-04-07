/* rp2040-vectors.c - vector table for RP2040
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

extern void rp2040_stacktop(void);	/* This is a blatant lie! rp2040_stacktop is a symbol set in the linker script */
extern void rp2040_kickstart(void);

extern void app_unknowntrap(void);
extern void app_unknown_irq(void);

typedef void (*rp2040_vector_t)(void);

/* Cortex-M0 doesn't provide memfault, busfault and usagefault traps
*/
#ifndef APP_NMI
#define APP_NMI				app_unknowntrap
#endif
#ifndef APP_HARDFAULT
#define APP_HARDFAULT		app_unknowntrap
#endif
#ifndef APP_MEMFAULT
#define APP_MEMFAULT		app_unknowntrap
#endif
#ifndef APP_BUSFAULT
#define APP_BUSFAULT		app_unknowntrap
#endif
#ifndef APP_USAGEFAULT
#define APP_USAGEFAULT		app_unknowntrap
#endif
#ifndef APP_SVCTRAP
#define APP_SVCTRAP			app_unknowntrap
#endif
#ifndef APP_PENDSVTRAP
#define APP_PENDSVTRAP		app_unknowntrap
#endif
#ifndef APP_SYSTICKIRQ
#define APP_SYSTICKIRQ		app_unknowntrap
#endif
#ifndef APP_UNKNOWNTRAP
#define APP_UNKNOWNTRAP		app_unknowntrap
#endif
#ifndef APP_TIMER_IRQ_0
#define APP_TIMER_IRQ_0		app_unknown_irq
#endif
#ifndef APP_TIMER_IRQ_1
#define APP_TIMER_IRQ_1		app_unknown_irq
#endif
#ifndef APP_TIMER_IRQ_2
#define APP_TIMER_IRQ_2		app_unknown_irq
#endif
#ifndef APP_TIMER_IRQ_3
#define APP_TIMER_IRQ_3		app_unknown_irq
#endif
#ifndef APP_PWM_IRQ_WRAP
#define APP_PWM_IRQ_WRAP	app_unknown_irq
#endif
#ifndef APP_USBCTRL_IRQ
#define APP_USBCTRL_IRQ		app_unknown_irq
#endif
#ifndef APP_XIP_IRQ
#define APP_XIP_IRQ			app_unknown_irq
#endif
#ifndef APP_PIO0_IRQ_0
#define APP_PIO0_IRQ_0		app_unknown_irq
#endif
#ifndef APP_PIO0_IRQ_1
#define APP_PIO0_IRQ_1		app_unknown_irq
#endif
#ifndef APP_PIO1_IRQ_0
#define APP_PIO1_IRQ_0		app_unknown_irq
#endif
#ifndef APP_PIO1_IRQ_1
#define APP_PIO1_IRQ_1		app_unknown_irq
#endif
#ifndef APP_DMA_IRQ_0
#define APP_DMA_IRQ_0		app_unknown_irq
#endif
#ifndef APP_DMA_IRQ_1
#define APP_DMA_IRQ_1		app_unknown_irq
#endif
#ifndef APP_IO_IRQ_BANK0
#define APP_IO_IRQ_BANK0	app_unknown_irq
#endif
#ifndef APP_IO_IRQ_QSPI
#define APP_IO_IRQ_QSPI		app_unknown_irq
#endif
#ifndef APP_SIO_IRQ_PROC0
#define APP_SIO_IRQ_PROC0	app_unknown_irq
#endif
#ifndef APP_SIO_IRQ_PROC1
#define APP_SIO_IRQ_PROC1	app_unknown_irq
#endif
#ifndef APP_CLOCKS_IRQ
#define APP_CLOCKS_IRQ		app_unknown_irq
#endif
#ifndef APP_SPI0_IRQ
#define APP_SPI0_IRQ		app_unknown_irq
#endif
#ifndef APP_SPI1_IRQ
#define APP_SPI1_IRQ		app_unknown_irq
#endif
#ifndef APP_UART0_IRQ
#define APP_UART0_IRQ		app_unknown_irq
#endif
#ifndef APP_UART1_IRQ
#define APP_UART1_IRQ		app_unknown_irq
#endif
#ifndef APP_ADC_IRQ_FIFO
#define APP_ADC_IRQ_FIFO	app_unknown_irq
#endif
#ifndef APP_I2C0_IRQ
#define APP_I2C0_IRQ		app_unknown_irq
#endif
#ifndef APP_I2C1_IRQ
#define APP_I2C1_IRQ		app_unknown_irq
#endif
#ifndef APP_RTC_IRQ
#define APP_RTC_IRQ			app_unknown_irq
#endif
#ifndef APP_IRQ_26
#define APP_IRQ_26			app_unknown_irq
#endif
#ifndef APP_IRQ_27
#define APP_IRQ_27			app_unknown_irq
#endif
#ifndef APP_IRQ_28
#define APP_IRQ_28			app_unknown_irq
#endif
#ifndef APP_IRQ_29
#define APP_IRQ_29			app_unknown_irq
#endif
#ifndef APP_IRQ_30
#define APP_IRQ_30			app_unknown_irq
#endif
#ifndef APP_IRQ_31
#define APP_IRQ_31			app_unknown_irq
#endif

/* The vector table is an array of addresses.
 * This table has (16+32) vectors:
 *	- 16 are the armvx-m exception/reset vectors (including reset SP)
 *	- 32 are the NVIC vectors. NVC interrupts 26..31 have no periperal connected but can be triggered by software.
*/
const rp2040_vector_t rp2040_hwvectors[16+32] =
{	&rp2040_stacktop,
	&rp2040_kickstart,
	&APP_NMI,
	&APP_HARDFAULT,
	&APP_MEMFAULT,
	&APP_BUSFAULT,
	&APP_USAGEFAULT,
	&APP_UNKNOWNTRAP,
	&APP_UNKNOWNTRAP,
	&APP_UNKNOWNTRAP,
	&APP_UNKNOWNTRAP,
	&APP_SVCTRAP,
	&APP_UNKNOWNTRAP,
	&APP_UNKNOWNTRAP,
	&APP_PENDSVTRAP,
	&APP_SYSTICKIRQ,
	&APP_TIMER_IRQ_0,		/* 0 */
	&APP_TIMER_IRQ_1,		/* 1 */
	&APP_TIMER_IRQ_2,		/* 2 */
	&APP_TIMER_IRQ_3,		/* 3 */
	&APP_PWM_IRQ_WRAP,		/* 4 */
	&APP_USBCTRL_IRQ,		/* 5 */
	&APP_XIP_IRQ,			/* 6 */
	&APP_PIO0_IRQ_0,		/* 7 */
	&APP_PIO0_IRQ_1,		/* 8 */
	&APP_PIO1_IRQ_0,		/* 9 */
	&APP_PIO1_IRQ_1,		/* 10 */
	&APP_DMA_IRQ_0,			/* 11 */
	&APP_DMA_IRQ_1,			/* 12 */
	&APP_IO_IRQ_BANK0,		/* 13 */
	&APP_IO_IRQ_QSPI,		/* 14 */
	&APP_SIO_IRQ_PROC0,		/* 15 */
	&APP_SIO_IRQ_PROC1,		/* 16 */
	&APP_CLOCKS_IRQ,		/* 17 */
	&APP_SPI0_IRQ,			/* 18 */
	&APP_SPI1_IRQ,			/* 19 */
	&APP_UART0_IRQ,			/* 20 */
	&APP_UART1_IRQ,			/* 21 */
	&APP_ADC_IRQ_FIFO,		/* 22 */
	&APP_I2C0_IRQ,			/* 23 */
	&APP_I2C1_IRQ,			/* 24 */
	&APP_RTC_IRQ,			/* 25 */
	&APP_IRQ_26,			/* 26 */
	&APP_IRQ_27,			/* 27 */
	&APP_IRQ_28,			/* 28 */
	&APP_IRQ_29,			/* 29 */
	&APP_IRQ_30,			/* 30 */
	&APP_IRQ_31,			/* 31 */
};

void app_unknowntrap(void)
{
	for (;;)
	{
	}
}

void app_unknown_irq(void)
{
	for (;;)
	{
	}
}
