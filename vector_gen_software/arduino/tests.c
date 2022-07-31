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

// TODO:
// Can these be made hardware portable??

#include <avr/io.h>
#include <util/delay.h>

#include "hw_impl.h"
#include "../portable/hardware.h"
#include "../portable/tests.h"
#include "../portable/display_list.h"

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

void test1b()
{
  // ----- Test 1b - Check coefficient zero

  spi(DAC_COEFF, DAC_A | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 2048);
  spi(DAC_COEFF, DAC_B | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 2048);
  for(;;) ; // stop after setting voltages
}


void test1c()
{
  // Measure delays in avr library

  PORTB |= PORTB_TRIGGER_MASK;
  _delay_loop_1(100); // about 18.87µs measured by scope (can't get freq counter to work right now)
  PORTB &= ~PORTB_TRIGGER_MASK;
  _delay_us(1);
}

void test1d()
{
  // Exercise all PORTD and PORTC outputs to verify they work

  PORTD = 0;
  PORTC = 0;
    _delay_us(1);
  PORTD = PORTD_OUTPUT_MASK;
  PORTC = PORTC_OUTPUT_MASK;
    _delay_us(1);
}

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

void test3()
{
  // ----- Test 3 - Z output

  IO_POINT_SETUP(); // De-selects both limit comparators, setting limit signal to 0
  IO_ENABLE_AND_UNBLANK_Z();

  for(;;) ; // stop after setting voltages
}

// Test 4 - Limit DAC offsets

// This is an important calibration step.
// We determine the code unit offset between the integrator output
// when in reset state, versus the limit DAC.
// This is done by a manual DVM measurement of the respective voltages.
// Without this calibration, the line may be drawn visibly too long
// or too short.

void test4() {
  IO_END_LINE(); // put integrators in hold
  IO_CLOSE_RESET(); // put integrators in reset

  // We want to find difference between DAC midpoint voltage
  // and the integrator zero (where lines start from).

  spi(DAC_LIMIT, DAC_A | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | 2048);
  spi(DAC_LIMIT, DAC_B | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | 2048);

  for(;;) ;
}

void test4b() {
  IO_END_LINE(); // put integrators in hold
  IO_CLOSE_RESET(); // put integrators in reset

  // Apply adjustments

  spi(DAC_LIMIT, DAC_A | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | (2048+DAC_LIMIT_X_ADJ));
  spi(DAC_LIMIT, DAC_B | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | (2048+DAC_LIMIT_Y_ADJ));

  for(;;) ;
}

/**
 * This test is to generate a step signal to check frequency response
 * of the output/summing amplifier.
 *
 * Note: this is a slow transition obviously at DAC slew rate
 */
void test5() {
  IO_END_LINE(); // put integrators in hold
  IO_CLOSE_RESET(); // put integrators in reset

  for(;;) {
    IO_RAISE_TRIGGER();
    spi(DAC_POS, DAC_A | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | 1024);
    spi(DAC_POS, DAC_B | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | 1024);
  _delay_us(10);
    IO_DROP_TRIGGER();
    spi(DAC_POS, DAC_A | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | 3072);
    spi(DAC_POS, DAC_B | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | 3072);
  _delay_us(10);
  }
}
