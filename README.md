# Bare metal programming on the RP2040

This is a collection of bits for constructing RP2040 applications without the pico SDK environment.

The aim is for the memory footprint to be as small as possible, by letting the compiler
and linker do a lot of the work.

## Tests

The "tests" in the test/ directory are not proper tests of the software. They serve as a check
that I have understood the RP2040 datasheet properly for a subset of the hardware functionality.

You might find the tests useful as examples when building your own bare-metal RP2040 application.

The header test uses the host C compiler and merely checks that the addresses of the structure
elements in the .h files correspond with the register addresses given in the datasheet.

The compile test checks that there are no syntax errors in the files under c/ and s/

## Caveat

The testing uses the on-board UF2 loader to load directly into RAM and run from there.
Because of that, the test programs might not initialise everything that should be initialised
for the code to run correctly. One example is the watchdog - it seems that the tick generator
is initialised to divide the reference clock (XOSC) by 12. From a cold boot, it is likely 
that the startup code will need to do that. See rp2040-watchdog.h

It should be possible to link the programs so that they run from flash, but the timing might be off.


## License, disclaimer etc.

Copyright David Haworth

This file is part of rp2040-bare-metal.

rp2040-bare-metal is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

rp2040-bare-metal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with rp2040-bare-metal.  If not, see <http://www.gnu.org/licenses/>.
