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

void demo_square() {
  // Note that X = 0 and Y = 0 correspond to Position DAC in mid-range, i.e. 1.25V
  // DAC value = k*x0*0xfffu + 2048      or, k = 2048/(4095 * abs(max_x_y))
  // The addressable range of Position DAC is therefore
  // -2048/(4095*k) .. (4095-2048)/(4095*k) ... if k = 1,   -0.5 .. +0.5
  // The limit DAC (line endpoint, integrator stop) is intended to be in the same units

  static struct line display_list[15];
  struct line *j = display_list;

  int16_t size = 1700;
  setup_line_int(j++, -size, -size, +size, -size, 0);
  setup_line_int(j++, +size, -size, +size, +size, 0);
  setup_line_int(j++, +size, +size, -size, +size, 0);
  setup_line_int(j++, -size, +size, -size, -size, 0);

  setup_line_int(j++, -(size*4)/5, +size, -(size*4)/5, -size, 1);
  setup_line_int(j++, +(size*4)/5, +size, +(size*4)/5, -size, 0);
  setup_line_int(j++,   0, +size,   0, -size, 2);

  setup_line_int(j++, -size, -(size*4)/5, +size, -(size*4)/5, 1);
  setup_line_int(j++, -size, +(size*4)/5, +size, +(size*4)/5, 0);
  setup_line_int(j++, -size,   0, +size,   0, 2);

  setup_line_int(j++, -size/2, -size/2, +size/2, +size/2, 0);
  setup_line_int(j++, -size/2, +size/2, +size/2, -size/2, 0);

  /*char s[20];
  sprintf(s,"X %d Y %d", adjx, adjy);
  j = setup_text(j, -1500, -500, 30, s);*/

  for(;;) {
    for(struct line *k = display_list; k < j;) {
      execute_line(k++);
    }
  }
}
