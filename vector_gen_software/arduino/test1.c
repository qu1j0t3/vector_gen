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

#include <avr/io.h>
#include <util/delay.h>

#include "hw_impl.h"
#include "../portable/hardware.h"

void test1()
{
  // ----- Test 1 - coefficient DAC step

  PORTB |= PORTB_TRIGGER_MASK;
  spi(DAC_COEFF, DAC_A | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 0);
  spi(DAC_COEFF, DAC_B | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 0);
  PORTB &= ~PORTB_TRIGGER_MASK;
  _delay_us(20);
  spi(DAC_COEFF, DAC_A | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 2048);
  spi(DAC_COEFF, DAC_B | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 2048);
  _delay_us(20);
  spi(DAC_COEFF, DAC_A | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 4095);
  spi(DAC_COEFF, DAC_B | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 4095);
  _delay_us(20);
}
