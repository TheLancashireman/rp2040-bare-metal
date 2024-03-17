#!/bin/sh
#
# (c) David Haworth
#
#  This file is part of rp2040-bare-metal.
#
#  rp2040-bare-metal is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  rp2040-bare-metal is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with rp2040-bare-metal.  If not, see <http://www.gnu.org/licenses/>.

# This script copies a (UF2) file to a Raspberry Pi pico.
# The reason for all the fuss is to try to prevent trashing an arbitrary disk that just happens
# to show up unexpectedly.
#
# WARNING: You may have to enter your password for the sudo parts.
# Read the script to understand what it does before trusting it with supervisor privileges.
#
# Assumptions:
#	* Hot-plugged filesystems don't automatically get mounted
#	* The pico shows up at /dev/sdc --> change PICODEV if not, or use 2 parameters
#	* The mount point is /media/pico --> change PICOMNT if not
#	* You have sudo permissions to mount and umount filesystems  --> hard luck if not ;-)
# You can change PICOMNT and PICODEV by setting environment variables.
#
# Usage:
#	to-pico [UF2-file] [pico-device]

if [ -z "$PICODEV" ]; then
	PICODEV=/dev/sdc1
fi
if [ -z "$PICOMNT" ]; then
PICOMNT=/media/pico
fi

set -e

if [ -z "$1" ] ; then
	UF2FILE="bin/rp2040.uf2"
else
	UF2FILE="$1"
fi

if [ -z "$2" ] ; then
	UF2DEV="$PICODEV"
else
	UF2DEV="$2"
fi

if [ -e "${UF2FILE}" ]; then
	echo "Uploading ${UF2FILE}"
else
	echo "Usage: to-pico.sh [<UF2-file>] [<device>]"
	exit 1
fi

sudo mount -o uid="${USER}" "${UF2DEV}" "${PICOMNT}"
if [ -e "${PICOMNT}/INFO_UF2.TXT" ]; then
	LINE=A
	CHECK=OK
	while read -r l ; do
		case ${LINE} in
		A)	if [ "${l}" != "UF2 Bootloader v2.0" ] ; then
				echo "Unexpected line 1 in INFO_UF2.TXT"
				CHECK=BAD
			fi
			LINE=B
			;;
		B)	if [ "${l}" != "Model: Raspberry Pi RP2" ] ; then
				echo "Unexpected line 2 in INFO_UF2.TXT"
				CHECK=BAD
			fi
			LINE=C
			;;
		C)	if [ "${l}" != "Board-ID: RPI-RP2" ] ; then
				echo "Unexpected line 3 in INFO_UF2.TXT"
				CHECK=BAD
			fi
			LINE=D
			;;
		D)	echo "Unexpected extra line in INFO_UF2.TXT"
			CHECK=BAD
			LINE=E
			;;
		E)
			;;
		esac
	done < ${PICOMNT}/INFO_UF2.TXT

	if [ "${LINE}" = "D" ] && [ "${CHECK}" = "OK" ] ; then
		cp "${UF2FILE}" "${PICOMNT}"
	else
		echo "The mounted UF2 bootloader is not a Raspberry Pi pico"
	fi
else
	echo "The mounted filesystem is not a UF2 bootloader"
fi
sudo umount "${PICOMNT}"
