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

#ifndef __VG_DISPLAY_LIST__

#define __VG_DISPLAY_LIST__

#include <stdint.h>

// Arduino 328 seems to be limited to below 200 vectors
// Display list needs 7 16-bit words per vector (14 bytes)
#define DISPLAY_LIST_MAX 175

#define COARSE_POINT_MAX 1

#define LINE_LIMIT_X_MASK   0b00010000
#define LINE_LIMIT_LOW_MASK 0b00100000
#define LINE_ACTIVE_MASK    0b01000000
#define IS_POINT_MASK       0b10000000

extern uint8_t ptx[], pty[];

extern uint8_t line_dash_style[];

#define DAC_COEFF_X_ADJ -4
#define DAC_COEFF_Y_ADJ +5

#define DAC_LIMIT_X_ADJ -5
#define DAC_LIMIT_Y_ADJ -13

uint16_t setup_line_int(uint16_t i, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t dash);

void execute_pt(uint16_t i);
void execute_line(uint16_t i);

#endif
