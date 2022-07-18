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
		// 12 x 12 maze
		// Benchmark: 225.5 Hz, 74 vectors ~ 16,687 vectors/sec

    uint8_t j = 0;

    setup_line_int(j++,-1800,1800,-1800,-1800,0);
    setup_line_int(j++,1800,1800,-1800,1800,0);
    setup_line_int(j++,1800,-1800,1800,1800,0);
    setup_line_int(j++,-1800,-1800,1800,-1800,0);
    setup_line_int(j++,-600,200,-600,-200,0);
    setup_line_int(j++,-400,200,-600,200,0);
    setup_line_int(j++,-400,400,-400,200,0);
    setup_line_int(j++,0,400,-400,400,0);
    setup_line_int(j++,0,200,0,400,0);
    setup_line_int(j++,800,1000,800,1400,0);
    setup_line_int(j++,1200,1000,800,1000,0);
    setup_line_int(j++,1200,800,1200,1000,0);
    setup_line_int(j++,1800,800,1200,800,0);
    setup_line_int(j++,-1400,-800,-1800,-800,0);
    setup_line_int(j++,-1400,-600,-1400,-800,0);
    setup_line_int(j++,-1600,-600,-1400,-600,0);
    setup_line_int(j++,-1600,-200,-1600,-600,0);
    setup_line_int(j++,-1000,-200,-1600,-200,0);
    setup_line_int(j++,-1000,-400,-1000,-200,0);
    setup_line_int(j++,-800,-400,-1000,-400,0);
    setup_line_int(j++,-800,-200,-800,-400,0);
    setup_line_int(j++,-400,-200,-800,-200,0);
    setup_line_int(j++,-400,-1000,-400,-200,0);
    setup_line_int(j++,400,800,400,1400,0);
    setup_line_int(j++,-800,800,400,800,0);
    setup_line_int(j++,-600,-600,-1200,-600,0);
    setup_line_int(j++,-1600,400,-1800,400,0);
    setup_line_int(j++,-1600,600,-1600,400,0);
    setup_line_int(j++,-1200,600,-1600,600,0);
    setup_line_int(j++,-1200,800,-1200,600,0);
    setup_line_int(j++,1400,-1200,1800,-1200,0);
    setup_line_int(j++,1400,-800,1400,-1200,0);
    setup_line_int(j++,1000,-800,1400,-800,0);
    setup_line_int(j++,1000,-600,1000,-800,0);
    setup_line_int(j++,1200,-600,1000,-600,0);
    setup_line_int(j++,1200,-200,1200,-600,0);
    setup_line_int(j++,400,400,1600,400,0);
    setup_line_int(j++,400,-200,400,400,0);
    setup_line_int(j++,600,-200,400,-200,0);
    setup_line_int(j++,600,-400,600,-200,0);
    setup_line_int(j++,200,-600,200,0,0);
    setup_line_int(j++,-1600,-1400,-1600,-1800,0);
    setup_line_int(j++,-1400,-1400,-1600,-1400,0);
    setup_line_int(j++,-1400,-1000,-1400,-1400,0);
    setup_line_int(j++,-400,-1600,-400,-1200,0);
    setup_line_int(j++,-400,0,400,0,0);
    setup_line_int(j++,-1200,1600,-1200,1800,0);
    setup_line_int(j++,-1000,1600,-1200,1600,0);
    setup_line_int(j++,-1000,1200,-1000,1600,0);
    setup_line_int(j++,200,1200,-1000,1200,0);
    setup_line_int(j++,-200,200,200,200,0);
    setup_line_int(j++,400,-400,1000,-400,0);
    setup_line_int(j++,-1400,1600,-1400,1400,0);
    setup_line_int(j++,-600,-400,-600,-800,0);
    setup_line_int(j++,0,1400,0,1200,0);
    setup_line_int(j++,1400,1000,1400,800,0);
    setup_line_int(j++,-600,1000,-600,1200,0);
    setup_line_int(j++,-1000,200,-800,200,0);
    setup_line_int(j++,800,600,800,400,0);
    setup_line_int(j++,1000,200,1000,400,0);
    setup_line_int(j++,800,1600,800,1800,0);
    setup_line_int(j++,600,1600,600,1400,0);
    setup_line_int(j++,1400,-600,1600,-600,0);
    setup_line_int(j++,1600,1600,1600,1000,0);
    setup_line_int(j++,1200,-1000,800,-1000,0);
    setup_line_int(j++,1200,-1600,1200,-1000,0);
    setup_line_int(j++,600,600,600,1200,0);
    setup_line_int(j++,-600,600,600,600,0);
    setup_line_int(j++,-600,400,-600,600,0);
    setup_line_int(j++,-800,400,-600,400,0);
    setup_line_int(j++,-800,0,-800,400,0);
    setup_line_int(j++,-600,-1400,-600,-1800,0);
    setup_line_int(j++,800,-200,800,0,0);
    setup_line_int(j++,1600,-200,800,-200,0);
    setup_line_int(j++,1600,-800,1600,-200,0);
    setup_line_int(j++,-1400,800,-1000,800,0);
    setup_line_int(j++,-800,1400,-400,1400,0);
    setup_line_int(j++,-800,1600,-800,1400,0);
    setup_line_int(j++,1000,800,600,800,0);
    setup_line_int(j++,1000,600,1000,800,0);
    setup_line_int(j++,1600,600,1000,600,0);
    setup_line_int(j++,1600,0,1600,600,0);
    setup_line_int(j++,1400,0,1600,0,0);
    setup_line_int(j++,800,-1600,800,-1800,0);
    setup_line_int(j++,1000,-1600,800,-1600,0);
    setup_line_int(j++,1000,-1200,1000,-1600,0);
    setup_line_int(j++,-1600,200,-1200,200,0);
    setup_line_int(j++,600,200,800,200,0);
    setup_line_int(j++,600,0,600,200,0);
    setup_line_int(j++,1200,0,600,0,0);
    setup_line_int(j++,1200,200,1200,0,0);
    setup_line_int(j++,1400,200,1200,200,0);
    setup_line_int(j++,1400,-1600,1400,-1800,0);
    setup_line_int(j++,0,-400,-400,-400,0);
    setup_line_int(j++,0,-200,0,-400,0);
    setup_line_int(j++,-1400,0,-1400,-200,0);
    setup_line_int(j++,-1600,0,-1400,0,0);
    setup_line_int(j++,-1400,1200,-1400,1000,0);
    setup_line_int(j++,-800,-1000,200,-1000,0);
    setup_line_int(j++,-800,-800,-800,-1000,0);
    setup_line_int(j++,-1000,-800,-800,-800,0);
    setup_line_int(j++,-1000,-1000,-1000,-1400,0);
    setup_line_int(j++,-1600,-1000,-1000,-1000,0);
    setup_line_int(j++,1600,-1000,1800,-1000,0);
    setup_line_int(j++,800,-1400,800,-400,0);
    setup_line_int(j++,600,-1400,800,-1400,0);
    setup_line_int(j++,600,-1600,600,-1400,0);
    setup_line_int(j++,-200,-1600,600,-1600,0);
    setup_line_int(j++,-200,-1400,-200,-1600,0);
    setup_line_int(j++,200,-1400,-200,-1400,0);
    setup_line_int(j++,200,-800,200,-1400,0);
    setup_line_int(j++,400,-800,200,-800,0);
    setup_line_int(j++,400,-1400,400,-800,0);
    setup_line_int(j++,-200,-200,-200,0,0);
    setup_line_int(j++,-800,1000,-1600,1000,0);
    setup_line_int(j++,-800,600,-800,1000,0);
    setup_line_int(j++,-1000,600,-800,600,0);
    setup_line_int(j++,-1000,400,-1000,600,0);
    setup_line_int(j++,-1200,400,-1000,400,0);
    setup_line_int(j++,-1200,0,-1200,400,0);
    setup_line_int(j++,-1000,0,-1200,0,0);
    setup_line_int(j++,600,-1200,400,-1200,0);
    setup_line_int(j++,1600,-1400,1200,-1400,0);
    setup_line_int(j++,1600,-1600,1600,-1400,0);
    setup_line_int(j++,400,1600,400,1800,0);
    setup_line_int(j++,1200,1400,1200,1200,0);
    setup_line_int(j++,200,1400,1200,1400,0);
    setup_line_int(j++,200,1600,200,1400,0);
    setup_line_int(j++,-200,1600,200,1600,0);
    setup_line_int(j++,-200,1400,-200,1600,0);
    setup_line_int(j++,-400,1000,-400,800,0);
    setup_line_int(j++,200,1000,-400,1000,0);
    setup_line_int(j++,-400,1600,-400,1200,0);
    setup_line_int(j++,-1600,-1200,-1800,-1200,0);
    setup_line_int(j++,200,400,200,600,0);
    setup_line_int(j++,1200,1600,1200,1800,0);
    setup_line_int(j++,1400,1600,1200,1600,0);
    setup_line_int(j++,1400,1400,1400,1600,0);
    setup_line_int(j++,-1200,-400,-1200,-1000,0);
    setup_line_int(j++,-1400,-400,-1200,-400,0);
    setup_line_int(j++,1000,1200,1600,1200,0);
    setup_line_int(j++,-1600,1400,-1600,800,0);
    setup_line_int(j++,-1200,1400,-1600,1400,0);
    setup_line_int(j++,-1200,1200,-1200,1400,0);
    setup_line_int(j++,-600,1600,-600,1800,0);
    setup_line_int(j++,-200,-800,0,-800,0);
    setup_line_int(j++,-200,-600,-200,-800,0);
    setup_line_int(j++,600,-600,-200,-600,0);
    setup_line_int(j++,600,-1000,600,-600,0);
    setup_line_int(j++,-1600,1600,-1800,1600,0);
    setup_line_int(j++,-1400,400,-1400,200,0);
    setup_line_int(j++,-800,-1600,-800,-1200,0);
    setup_line_int(j++,-1200,-1600,-800,-1600,0);
    setup_line_int(j++,-1200,-1200,-1200,-1600,0);
    setup_line_int(j++,-1400,-1600,-1400,-1800,0);
    setup_line_int(j++,1000,1600,1000,1400,0);
    setup_line_int(j++,1400,-400,1200,-400,0);
    setup_line_int(j++,0,-1200,-1000,-1200,0);
    // 158 vectors

		for(;;) {
			for(uint8_t i = 0; i < j; ++i) {
				execute_line(i);
			}
		}
}
