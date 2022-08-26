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

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "display_list.h"
#include "demos.h"

#include "hw_impl.h"


#define INITIAL_SEED 1013

struct line *make_rock(int16_t x, int16_t y, uint16_t r) {
  uint8_t sides = r/40 + (rand() & 7);
  if (sides < 5) sides = 5;
  struct line *obj = malloc(sides*sizeof(struct line));
  float k = 2*M_PI/sides;
  int16_t xpt[sides], ypt[sides];

  for(uint8_t i = 0; i < sides; ++i) {
    float rf = (rand()/(float)RAND_MAX)*0.4 + 0.8;
    xpt[i] = x + (int16_t)(r*rf*cos(i*k)) + rand()/(RAND_MAX/128);
    ypt[i] = y + (int16_t)(r*rf*sin(i*k)) + rand()/(RAND_MAX/128);
  }

  for(uint8_t i = 0; i < sides; ++i) {
    setup_line_int(obj+i, xpt[i], ypt[i], xpt[(i+1)%sides], ypt[(i+1)%sides],
                   i == sides-1 ? END_OBJECT_MASK : 0);
  }
  return obj;
}

void demo_rocks() {
  // Note that X = 0 and Y = 0 correspond to Position DAC in mid-range, i.e. 1.25V
  // DAC value = k*x0*0xfffu + 2048      or, k = 2048/(4095 * abs(max_x_y))
  // The addressable range of Position DAC is therefore
  // -2048/(4095*k) .. (4095-2048)/(4095*k) ... if k = 1,   -0.5 .. +0.5
  // The limit DAC (line endpoint, integrator stop) is intended to be in the same units


  // Possible design for a dynamic environment

  // Each entry in this array points to an array allocated on the heap
  // comprising the actual lines in the object.
  // Or, is NULL, indicating "no object"
  // (by using the heap, we can avoid having to manage a single contiguous
  // display list, as in the first version of this project)

  static struct line xtick, ytick;

  setup_line_int(&xtick, -100, 0, 100, 0, 0);
  setup_line_int(&ytick, 0, -100, 0, 100, 0);

  char str[20];
  for(unsigned int s = INITIAL_SEED; ;) {
    srand(s);
    sprintf(str, "Seed: %u\r\n", s);
    uart_puts(str);

    for(uint8_t i = 0; i < MAX_OBJECTS; ++i) {
      object[i] = make_rock(((rand() & 0xfff) % 3000) - 1500, ((rand() & 0xfff) % 3000) - 1500, (rand() % 300) + 50);
    }

    for(;;) {
      execute_line(&xtick);
      execute_line(&ytick);

      for(uint8_t k = 0; k < MAX_OBJECTS; ++k) {
        struct line *p = object[k];
        if (p) {
          for(;;) {
            execute_line(p);
            if (p->flags & END_OBJECT_MASK) break;
            ++p;
          }
        }
      }

      char key = uart_getchar_poll();
      if (key == ' ') {
        ++s;
        for(uint8_t i = 0; i < MAX_OBJECTS; ++i) {
          free(object[i]);
        }
        break; // recreate display list with new seed
      }
    }
  }
}
