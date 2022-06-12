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

#include <stdint.h>

#include "display_list.h"

#include "hw_impl.h"

// Release Configuration:

//delay(0); // approx 7-8 ns
//delay(100); // approx 12.56µs
//delay(1000); // approx 125µs

void delay(unsigned j) {
    for (uint32_t i = 0; i < j; ++i) {
        __asm("NOP"); /* delay */
    }
}

void io_line_setup(uint8_t flags) {
  BOARD_INITPINS_INT_RESET_FGPIO->PCOR = BOARD_INITPINS_INT_RESET_GPIO_PIN_MASK; // Open INT RESET

	// UNBLANK

	// Arm comparator on the fastest-changing integrator
	// (greater magnitude coefficient of X and Y)
	if(flags & LINE_LIMIT_X_MASK) {
		BOARD_INITPINS_X_COMP_SEL_FGPIO->PSOR = BOARD_INITPINS_X_COMP_SEL_GPIO_PIN_MASK;
		BOARD_INITPINS_Y_COMP_SEL_FGPIO->PCOR = BOARD_INITPINS_Y_COMP_SEL_GPIO_PIN_MASK;
	} else {
		BOARD_INITPINS_X_COMP_SEL_FGPIO->PCOR = BOARD_INITPINS_X_COMP_SEL_GPIO_PIN_MASK;
		BOARD_INITPINS_Y_COMP_SEL_FGPIO->PSOR = BOARD_INITPINS_Y_COMP_SEL_GPIO_PIN_MASK;
	}

  if(flags & LINE_LIMIT_LOW_MASK) {
    BOARD_INITPINS_LIMIT_LOW_FGPIO->PCOR = BOARD_INITPINS_LIMIT_LOW_GPIO_PIN_MASK;
  } else {
    BOARD_INITPINS_LIMIT_LOW_FGPIO->PSOR = BOARD_INITPINS_LIMIT_LOW_GPIO_PIN_MASK;
  }
}