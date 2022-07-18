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

void demo_maze() {
    uint8_t j = 0;

    setup_line_int(j++,-1710,1900,-1710,-1710,0);
    setup_line_int(j++,1900,1900,-1710,1900,0);
    setup_line_int(j++,1900,-1710,1900,1900,0);
    setup_line_int(j++,-1710,-1710,1900,-1710,0);
    setup_line_int(j++,1520,-190,1520,-380,0);
    setup_line_int(j++,1710,-190,1520,-190,0);
    setup_line_int(j++,1710,0,1710,-190,0);
    setup_line_int(j++,1900,0,1710,0,0);
    setup_line_int(j++,1520,-1330,950,-1330,0);
    setup_line_int(j++,1330,1330,1330,1520,0);
    setup_line_int(j++,1140,1330,1330,1330,0);
    setup_line_int(j++,1140,950,1140,1330,0);
    setup_line_int(j++,1330,950,1140,950,0);
    setup_line_int(j++,1330,760,1330,950,0);
    setup_line_int(j++,1710,760,760,760,0);
    setup_line_int(j++,-190,1140,-190,380,0);
    setup_line_int(j++,0,1140,-190,1140,0);
    setup_line_int(j++,0,1330,0,1140,0);
    setup_line_int(j++,-380,1330,0,1330,0);
    setup_line_int(j++,570,-1140,950,-1140,0);
    setup_line_int(j++,570,-950,570,-1140,0);
    setup_line_int(j++,380,-950,570,-950,0);
    setup_line_int(j++,-380,-380,-380,190,0);
    setup_line_int(j++,-760,-380,-380,-380,0);
    setup_line_int(j++,-760,-1520,-760,-380,0);
    setup_line_int(j++,-1140,-1520,-760,-1520,0);
    setup_line_int(j++,-1140,-1710,-1140,-1520,0);
    setup_line_int(j++,-570,190,-570,570,0);
    setup_line_int(j++,-190,190,-570,190,0);
    setup_line_int(j++,-190,-190,-190,190,0);
    setup_line_int(j++,760,-190,-190,-190,0);
    setup_line_int(j++,760,380,760,-190,0);
    setup_line_int(j++,1330,380,190,380,0);
    setup_line_int(j++,1330,570,1330,380,0);
    setup_line_int(j++,1520,570,1330,570,0);
    setup_line_int(j++,1520,380,1520,570,0);
    setup_line_int(j++,1710,380,1520,380,0);
    setup_line_int(j++,1710,190,1710,380,0);
    setup_line_int(j++,1520,190,1710,190,0);
    setup_line_int(j++,1520,0,1520,190,0);
    setup_line_int(j++,1330,0,1520,0,0);
    setup_line_int(j++,-570,-1140,-760,-1140,0);
    setup_line_int(j++,-570,-1330,-570,-1140,0);
    setup_line_int(j++,190,-1330,-570,-1330,0);
    setup_line_int(j++,570,-190,570,-570,0);
    setup_line_int(j++,-1330,-1520,-1710,-1520,0);
    setup_line_int(j++,-1330,-1330,-1330,-1520,0);
    setup_line_int(j++,-1330,950,-760,950,0);
    setup_line_int(j++,-380,-760,-190,-760,0);
    setup_line_int(j++,-1520,-190,-1710,-190,0);
    setup_line_int(j++,-1330,-760,-1710,-760,0);
    setup_line_int(j++,570,-1330,760,-1330,0);
    setup_line_int(j++,570,-1520,570,-1330,0);
    setup_line_int(j++,190,-1520,570,-1520,0);
    setup_line_int(j++,190,-1140,190,-1520,0);
    setup_line_int(j++,-380,570,-1520,570,0);
    setup_line_int(j++,-190,-950,-190,-380,0);
    setup_line_int(j++,190,-950,-190,-950,0);
    setup_line_int(j++,190,-380,190,-950,0);
    setup_line_int(j++,380,-380,190,-380,0);
    setup_line_int(j++,760,-570,760,-950,0);
    setup_line_int(j++,380,-570,760,-570,0);
    setup_line_int(j++,1520,-760,1520,-1520,0);
    setup_line_int(j++,1710,-760,1520,-760,0);
    setup_line_int(j++,-760,760,-570,760,0);
    setup_line_int(j++,-760,1140,-760,760,0);
    setup_line_int(j++,950,950,760,950,0);
    setup_line_int(j++,-950,0,-1140,0,0);
    setup_line_int(j++,-950,190,-950,0,0);
    setup_line_int(j++,760,570,760,1900,0);
    setup_line_int(j++,950,1330,760,1330,0);
    setup_line_int(j++,950,190,950,0,0);
    setup_line_int(j++,570,1520,760,1520,0);
    setup_line_int(j++,1140,0,760,0,0);
    setup_line_int(j++,1710,570,1710,1140,0);
    setup_line_int(j++,570,-760,760,-760,0);
    setup_line_int(j++,-190,1520,190,1520,0);
    setup_line_int(j++,-190,1710,-190,1520,0);
    setup_line_int(j++,0,-1520,0,-1710,0);
    setup_line_int(j++,0,1710,0,1900,0);
    setup_line_int(j++,950,-1520,950,-380,0);
    setup_line_int(j++,760,-1520,950,-1520,0);
    setup_line_int(j++,-380,-1520,-380,-1710,0);
    setup_line_int(j++,-570,-1520,-380,-1520,0);
    setup_line_int(j++,1710,-950,1710,-1710,0);
    setup_line_int(j++,-950,760,-950,570,0);
    setup_line_int(j++,-1140,760,-950,760,0);
    setup_line_int(j++,-190,-1520,-190,-1140,0);
    setup_line_int(j++,190,1710,190,190,0);
    setup_line_int(j++,570,1710,190,1710,0);
    setup_line_int(j++,1330,1710,1330,1900,0);
    setup_line_int(j++,950,1710,950,1900,0);
    setup_line_int(j++,-570,1330,-950,1330,0);
    setup_line_int(j++,-570,950,-570,1330,0);
    setup_line_int(j++,-380,950,-570,950,0);
    setup_line_int(j++,-380,760,-380,950,0);
    setup_line_int(j++,0,760,-380,760,0);
    setup_line_int(j++,-1520,-570,-1140,-570,0);
    setup_line_int(j++,1710,1520,950,1520,0);
    setup_line_int(j++,0,380,-380,380,0);
    setup_line_int(j++,0,0,0,380,0);
    setup_line_int(j++,570,0,0,0,0);
    setup_line_int(j++,570,190,570,0,0);
    setup_line_int(j++,380,190,570,190,0);
    setup_line_int(j++,-570,-570,-190,-570,0);
    setup_line_int(j++,-570,-950,-570,-570,0);
    setup_line_int(j++,-380,-950,-570,-950,0);
    setup_line_int(j++,-380,-1140,-380,-950,0);
    setup_line_int(j++,1140,-1520,1140,-1710,0);
    setup_line_int(j++,1330,-1520,1140,-1520,0);
    setup_line_int(j++,1330,1140,1520,1140,0);
    setup_line_int(j++,1330,-190,950,-190,0);
    setup_line_int(j++,1330,190,1330,-190,0);
    setup_line_int(j++,1140,190,1330,190,0);
    setup_line_int(j++,1520,1330,1900,1330,0);
    setup_line_int(j++,1520,950,1520,1330,0);
    setup_line_int(j++,380,-760,190,-760,0);
    setup_line_int(j++,380,-1330,380,-760,0);
    setup_line_int(j++,950,570,950,380,0);
    setup_line_int(j++,1140,570,1140,760,0);
    setup_line_int(j++,1330,-570,1900,-570,0);
    setup_line_int(j++,1330,-1140,1330,-570,0);
    setup_line_int(j++,1140,-1140,1330,-1140,0);
    setup_line_int(j++,1140,-760,1140,-1140,0);
    setup_line_int(j++,-1520,-1140,-1520,-760,0);
    setup_line_int(j++,950,1140,1140,1140,0);
    setup_line_int(j++,1710,1710,1900,1710,0);
    setup_line_int(j++,0,950,190,950,0);
    setup_line_int(j++,380,760,190,760,0);
    setup_line_int(j++,-1520,950,-1710,950,0);
    setup_line_int(j++,-1520,1710,-1520,950,0);
    setup_line_int(j++,-950,1710,-1520,1710,0);
    setup_line_int(j++,-950,1140,-950,1710,0);
    setup_line_int(j++,-1140,1140,-950,1140,0);
    setup_line_int(j++,760,-380,1710,-380,0);
    setup_line_int(j++,-570,1520,-570,1900,0);
    setup_line_int(j++,-380,1140,-380,1710,0);
    setup_line_int(j++,-1140,-1140,-1140,-1330,0);
    setup_line_int(j++,-1140,1520,-1140,1330,0);
    setup_line_int(j++,-1330,1520,-1140,1520,0);
    setup_line_int(j++,-1330,760,-1330,1520,0);
    setup_line_int(j++,-1520,760,-1330,760,0);
    setup_line_int(j++,-1520,0,-1520,760,0);
    setup_line_int(j++,-1330,0,-1520,0,0);
    setup_line_int(j++,-1330,-380,-1330,0,0);
    setup_line_int(j++,-1520,-380,-1330,-380,0);
    setup_line_int(j++,-950,-1330,-1520,-1330,0);
    setup_line_int(j++,-950,-190,-950,-1330,0);
    setup_line_int(j++,-570,-190,-950,-190,0);
    setup_line_int(j++,-570,0,-570,-190,0);
    setup_line_int(j++,-760,0,-570,0,0);
    setup_line_int(j++,-760,380,-760,0,0);
    setup_line_int(j++,-1330,380,-760,380,0);
    setup_line_int(j++,-1330,190,-1330,380,0);
    setup_line_int(j++,-1140,190,-1330,190,0);
    setup_line_int(j++,-1140,-950,-1140,190,0);
    setup_line_int(j++,-1330,-950,-1140,-950,0);
    setup_line_int(j++,-1330,-1140,-1330,-950,0);
    setup_line_int(j++,-760,1710,-760,1330,0);
    setup_line_int(j++,380,950,570,950,0);
    setup_line_int(j++,380,1140,380,950,0);
    setup_line_int(j++,1140,1710,1140,1520,0);
    setup_line_int(j++,1140,-570,950,-570,0);
    setup_line_int(j++,0,-1140,0,-950,0);
    setup_line_int(j++,1520,1710,1520,1520,0);
    setup_line_int(j++,380,1330,380,1710,0);
    setup_line_int(j++,570,1330,380,1330,0);
    setup_line_int(j++,570,570,570,1330,0);
    setup_line_int(j++,380,570,570,570,0);
    setup_line_int(j++,0,570,190,570,0);
    setup_line_int(j++,0,-760,0,-190,0);
    // 171 vectors

		for(;;) {
			for(uint8_t i = 0; i < j; ++i) {
				execute_line(i);
			}
		}
}
