/* sio-pin-init.h - header file for RP2040 GPIO
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
#include "rp2040-sio.h"

/* sio_pin_init() - initialise a GPIO pin for input
*/
void sio_pin_init(int pin, boolean_t output)
{
	u32_t pinmask = 0x1 << pin;

	/* Disable the SIO output and turn off.
	*/
	rp2040_sio.gpio_oe.w1c = pinmask;
	rp2040_sio.gpio_out.w1c = pinmask;

	/* Select SIO function for the pin.
	*/
	rp2040_iobank0.gpio[pin].ctrl = FUNCSEL_SIO;

	if ( output )
	{
		rp2040_sio.gpio_oe.w1s = pinmask;
	}
}
