/*
    This file is part of the Vector Generator project for drawing analogue
    graphics on X/Y oscilloscopes.

    Copyright (C) 2018-2022 Toby Thain, toby@telegraphics.com.au

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GrenNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __VG_HARDWARE__

#define __VG_HARDWARE__

#include <stdint.h>

// units
#define DAC_A      0u
#define DAC_B      (1u << 15)

// control
#define DAC_BUFFERED (1u << 14) // Since we do not buffer the DAC Vrefs off chip, should always be configured buffered
#define DAC_UNBUFFERED 0u
#define DAC_GAINx2   0u
#define DAC_GAINx1   (1u << 13)
#define DAC_ACTIVE   (1u << 12)

void spi(uint8_t cs, uint16_t word);

#endif
