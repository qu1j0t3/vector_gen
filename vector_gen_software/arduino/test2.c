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

void test2()
{
  // ----- Test 2 - Integrator sawtooth (with reset)

  // Setup positive coefficient

  PORTB |= PORTB_TRIGGER_MASK;
  spi(DAC_COEFF, DAC_A | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | (uint16_t)((0.5+0.4)*4095) );
  spi(DAC_COEFF, DAC_B | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | (uint16_t)((0.5+0.4)*4095) );
  PORTB &= ~PORTB_TRIGGER_MASK;

  _delay_us(20);

  // Run integrator (ramp towards zero)

  PORTB &= ~PORTB_INT_RESET_MASK; // open reset switches
  PORTD |= PORTD_INT_HOLD_MASK; // close hold switches

  _delay_us(100);

  // Stop and reset integrator

  PORTD &= ~PORTD_INT_HOLD_MASK; // open hold switches
  PORTB |= PORTB_INT_RESET_MASK; // close reset switches

  // Setup negative coefficient

  spi(DAC_COEFF, DAC_A | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | (uint16_t)((0.5-0.4)*4095) );
  spi(DAC_COEFF, DAC_B | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | (uint16_t)((0.5-0.4)*4095) );

  _delay_us(20);

  // Run integrator (ramp towards +5V)

  PORTB &= ~PORTB_INT_RESET_MASK; // open reset switches
  PORTD |= PORTD_INT_HOLD_MASK; // close hold switches

  _delay_us(100);

  // Stop and reset integrator

  PORTD &= ~PORTD_INT_HOLD_MASK; // open hold switches
  PORTB |= PORTB_INT_RESET_MASK; // close reset switches
}
