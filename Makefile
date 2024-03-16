# Makefile for rp2040-bare-metal
#
# In order to use this repository, there's nothing to make.
# This Makfile does the compile tests
#
# (c) David Haworth
#
#  This file is part of pico-bare-metal.
#
#  pico-bare-metal is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  pico-bare-metal is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with pico-bare-metal.  If not, see <http://www.gnu.org/licenses/>.

test:	build/header-test

build/header-test:	test/compile-test/header-test.c
	gcc -I h/ -o build/header-test test/compile-test/header-test.c
	build/header-test
