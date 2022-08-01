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


void demo_boxes() {

  static struct line display_list[150];
  struct line *j;
  extern int8_t dac_limit_x_adj, dac_limit_y_adj;
  char c = '0';
  char s[20];

  uint8_t w = 6,h = 6;
  uint16_t spacing = 3000/w;
  uint16_t side = 2000/w;
  for(;;) {
    j = display_list;

    for(uint8_t i = 0; i < w*h; ++i) {
      int x = ((i/w)-w/2)*spacing, y = ((i % h)-h/2)*spacing;
      setup_line_int(j++, x,      y,      x+side, y,      0);
      setup_line_int(j++, x+side, y,      x+side, y+side, 0);
      setup_line_int(j++, x+side, y+side, x,      y+side, 0);
      setup_line_int(j++, x,      y+side, x,      y,      0);
    }

    for(;;) {
      IO_RAISE_TRIGGER();
      for(struct line *k = display_list; k < j;) {
        execute_line(k++);
      }

      // Interactive Limit DAC calibration
      //
      // Use terminal emulator at 115200 baud or adjust rate in hw_impl.c
      // Use W, A, S, D to adjust X and Y limits
      // pay attention to overshoots on the squares at corners
      // when display is optimal, take the adjustment values
      // and enter in display_list.h

      char key = uart_getchar_poll();
      if (key == 'a') {
        -- dac_limit_x_adj;
      } else if (key == 'd') {
        ++ dac_limit_x_adj;
      } else if (key == 'w') {
        ++ dac_limit_y_adj;
      } else if (key == 's') {
        -- dac_limit_y_adj;
      }
      if (key) {
        sprintf(s, "XADJ: %d  YADJ: %d\r\n", dac_limit_x_adj, dac_limit_y_adj);
        uart_print(s);
        break; // recreate display list with new adjustments
      }
    }
  }
}
