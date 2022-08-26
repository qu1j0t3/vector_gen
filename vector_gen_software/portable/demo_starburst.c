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

#include "display_list.h"
#include "demos.h"

#include "hw_impl.h"

#define RAYS 50

void demo_starburst() {
  uint8_t i, n = RAYS;
  float a = 2*M_PI/RAYS/8;
  static struct line display_list[RAYS*3];

  for (uint8_t m = 0; ; ++m) {
    struct line *j = display_list;
    float k = (m % 16)*2*M_PI/16;

    for(i = 0; i < n; ++i) {
      // Drawing the rays from outside to centre
      // exercises accuracy of integrating coefficient
      // linearity, especially near limits
      setup_line_int(j++, (int)1500*cosf(a*i+k), (int)1500*sinf(a*i+k), 0, 0, 0);
    }
    for(i = 0; i < n; ++i) {
      setup_line_int(j++, (int)1500*cosf(a*i+k), (int)1500*sinf(a*i+k), (int)1500*cosf(a*i+k), (int)1500*sinf(a*i+k), 0);
      setup_line_int(j++, (int)750*cosf(a*i+k), (int)750*sinf(a*i+k), (int)750*cosf(a*i+k), (int)750*sinf(a*i+k), 0);
    }

    for(uint16_t fr = 700; fr--;) {
      for(struct line *k = display_list; k < j;) {
        execute_line(k++);
      }
    }
  }
}
