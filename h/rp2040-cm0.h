/* rp2040-cm0.h
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
#ifndef RP2040_CM0_H
#define RP2040_CM0_H	1

/* This file covers the Cortex M0+ core used on RP2040.
 * Systick (optional on ARMv6M) is present on RP2040.
*/

/* Aux control registers
*/
typedef struct cxm_acr_s cxm_acr_t;

struct cxm_acr_s
{
	reg32_t unknown;	/* e000 - What's this */
	reg32_t not6m;		/* e004 - Interrupt controller type - not v6m */
	reg32_t actlr;		/* e008 - Aux control register */
};

#define CXM_ACR_BASE		0xe000e000
#define cxm_acr				((cxm_acr_t *)CXM_ACR_BASE)[0]

/* Systick registers
*/
typedef struct cxm_systick_s cxm_systick_t;

struct cxm_systick_s
{
	reg32_t stcsr;		/* e010 - SysTick control/status */
	reg32_t strvr;		/* e014 - SysTick reload */
	reg32_t stcvr;		/* e018 - SysTick current value */
	reg32_t stcr;		/* e01c - SysTick calibration value */
};

#define CXM_SYSTICK_BASE	0xe000e010
#define cxm_systick			((cxm_systick_t *)CXM_SYSTICK_BASE)[0]

#define SYST_COUNTFLAG		0x00010000		/* 1 ==> counter has counted to zero (read-only, clear by reading) */
#define SYST_CLKSRC			0x00000004		/* 1 = CPU, 0 = ext. ref */
#define SYST_TICKINT		0x00000002		/* 1 = trigger exception when timer reaches 0 */
#define SYST_ENABLE			0x00000001		/* 1 = counter is enabled */

#define SYST_MASK			0x00ffffff		/* Max value mask */

/* NVIC has its own header file, but the base address is defined here.
*/
#define NVIC_BASE			0xe000e100

/* System control registers
*/
typedef struct cxm_scr_s cxm_scr_t;

struct cxm_scr_s
{
	reg32_t cpuid;		/* ed00 - RO - CPU identification */
	reg32_t icsr;		/* ed04 - Interrupt control/state */
	reg32_t vtor;		/* ed08 - Vector table offset */
	reg32_t aircr;		/* ed0c - Application interrupt and reset control */
	reg32_t not6m_1;	/* ed10 - System control - not v6m */
	reg32_t ccr;		/* ed14 - Configuration and control */
	reg32_t not6m_2[3];	/* ed18 - System handler priority - element 0 (shpr1) not v6m */
	reg32_t shcsr;		/* ed24 - System handler control and state */
	reg32_t not6m_3;	/* ed28 - Configurable fault status - not v6m */
	reg32_t not6m_4;	/* ed2c - Hardfault status - not v6m */
	reg32_t dfsr;		/* ed30 - Debug fault status */
	/* Remaining "not6m" registers omitted
	*/
};

#define CXM_SCR_BASE		0xe000ed00
#define cxm_scr				((cxm_scr_t *)CXM_SCR_BASE)[0]

/* cxm_get_msp()/cxm_set_msp() - get and set the MSP register (main SP)
 *
 * WARNING: cxm_set_msp() results in undefined behaviour if main SP is the current SP.
*/
static inline u32_t cxm_get_msp(void)
{
	u32_t msp;
	__asm__ volatile("mrs %[reg], MSP" : [reg] "=r" (msp) : : );
	return msp;
}

static inline void cxm_set_msp(u32_t msp)
{
	__asm__ volatile ("msr MSP, %[reg]" : : [reg] "r" (msp) : );
}

/* cxm_get_psp()/cxm_set_psp() - get and set the PSP register (process SP)
 *
 * WARNING: cxm_set_psp() results in undefined behaviour if process SP is the current SP.
*/
static inline u32_t cxm_get_psp(void)
{
	u32_t psp;
	__asm__ volatile("mrs %[reg], PSP" : [reg] "=r" (psp) : : );
	return psp;
}

static inline void cxm_set_psp(u32_t psp)
{
	__asm__ volatile ("msr PSP, %[reg]" : : [reg] "r" (psp) : );
}

/* cxm_get_primask()/cxm_set_primask() - get and set the PRIMASK register
*/
static inline u32_t cxm_get_primask(void)
{
	u32_t primask;
	__asm__ volatile("mrs %[reg], PRIMASK" : [reg] "=r" (primask) : : );
	return primask;
}

static inline void cxm_set_primask(u32_t primask)
{
	__asm__ volatile ("msr PRIMASK, %[reg]" : : [reg] "r" (primask) : );
}

/* cxm_get_faultmask()/cxm_set_faultmask() - get and set the FAULTMASK register
*/
static inline u32_t cxm_get_faultmask(void)
{
	u32_t faultmask;
	__asm__ volatile("mrs %[reg], FAULTMASK" : [reg] "=r" (faultmask) : : );
	return faultmask;
}

static inline void cxm_set_faultmask(u32_t faultmask)
{
	__asm__ volatile ("msr FAULTMASK, %[reg]" : : [reg] "r" (faultmask) : );
}

/* cxm_get_basepri()/cxm_set_basepri() - get and set the BASEPRI register
*/
static inline u32_t cxm_get_basepri(void)
{
	u32_t basepri;
	__asm__ volatile("mrs %[reg], BASEPRI" : [reg] "=r" (basepri) : : );
	return basepri;
}

static inline void cxm_set_basepri(u32_t basepri)
{
	__asm__ volatile ("msr BASEPRI, %[reg]" : : [reg] "r" (basepri) : );
}

/* cxm_get_control()/cxm_set_control() - get and set the CONTROL register
*/
static inline u32_t cxm_get_control(void)
{
	u32_t control;
	__asm__ volatile("mrs %[reg], CONTROL" : [reg] "=r" (control) : : );
	return control;
}

static inline void cxm_set_control(u32_t control)
{
	__asm__ volatile ("msr CONTROL, %[reg]" : : [reg] "r" (control) : );
}

/* cxm_get_xpsr()/cxm_set_xpsr() - get and set the XPSR register
*/
static inline u32_t cxm_get_xpsr(void)
{
	u32_t xpsr;
	__asm__ volatile("mrs %[reg], XPSR" : [reg] "=r" (xpsr) : : );
	return xpsr;
}

static inline void cxm_set_xpsr(u32_t xpsr)
{
	__asm__ volatile ("msr XPSR, %[reg]" : : [reg] "r" (xpsr) : );
}

#define XPSR_IPSR			0x000001ff		/* Mask for IPSR in XPSR */

/* cxm_get_ipsr() - get the IPSR register
 *
 * IPSR is part of XPSR (bits 0..8). During interrupt/exception handling it contains the vector number.
 * IPSR cannot be written.
*/
static inline u32_t cxm_get_ipsr(void)
{
	u32_t ipsr;
	__asm__ volatile("mrs %[reg], IPSR" : [reg] "=r" (ipsr) : : );
	return ipsr;
}

/* cxm_get_sp() - get the current stack pointer
 *
 * There is no "set" function for the sp. Changing the SP from C code would result in undefined behaviour.
*/
static inline u32_t cxm_get_sp(void)
{
	u32_t sp;
	__asm__ volatile("mov %[reg], sp" : [reg] "=r" (sp) : : );
	return sp;
}

/* Interrupt status, locking and unlocking
*/
typedef u8_t intstatus_t;
#define INTENABLED	0x0		/* PRIMASK value to enable interrupts */
#define INTDISABLED	0x1		/* PRIMASK value to disable interrupts */

static inline intstatus_t disable(void)
{
	intstatus_t old = (intstatus_t)cxm_get_primask();
	cxm_set_primask(INTDISABLED);
	return old;
}

static inline intstatus_t restore(intstatus_t x)
{
	intstatus_t old = (intstatus_t)cxm_get_primask();
	cxm_set_primask((u32_t)x);
	return old;
}

#endif
