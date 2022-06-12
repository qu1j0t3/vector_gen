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

// Driver software for vector generator board
// Arduino version


#include <avr/io.h>
#include <util/delay.h>

#include "hw_impl.h"
#include "../portable/hardware.h"
#include "../portable/demos.h"

void test1();
void test2();

int main (void)
{
  hw_setup();

  // Measure delays in avr library
  while(0) {
      PORTB |= PORTB_TRIGGER_MASK;
      _delay_loop_1(100); // about 18.87µs measured by scope (can't get freq counter to work right now)
      PORTB &= ~PORTB_TRIGGER_MASK;
      _delay_us(1);
  }

  //while(1) test1();

  demo_square();

  return 1;
}
