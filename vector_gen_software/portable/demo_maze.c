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
    setup_line_int(j++,300,-1200,-1800,-1200,0);
    setup_line_int(j++,300,-1500,300,-1200,0);
    setup_line_int(j++,600,-1500,300,-1500,0);
    setup_line_int(j++,600,-300,600,-1500,0);
    setup_line_int(j++,300,-300,600,-300,0);
    setup_line_int(j++,900,-1200,1200,-1200,0);
    setup_line_int(j++,900,-1800,900,-1200,0);
    setup_line_int(j++,300,1500,300,600,0);
    setup_line_int(j++,-300,1500,300,1500,0);
    setup_line_int(j++,-300,1200,-300,1500,0);
    setup_line_int(j++,-600,1200,-300,1200,0);
    setup_line_int(j++,-900,300,-1200,300,0);
    setup_line_int(j++,-900,-600,-900,300,0);
    setup_line_int(j++,-1200,-300,-1500,-300,0);
    setup_line_int(j++,600,900,600,1800,0);
    setup_line_int(j++,900,900,600,900,0);
    setup_line_int(j++,900,600,900,900,0);
    setup_line_int(j++,1500,600,900,600,0);
    setup_line_int(j++,1500,1200,1500,600,0);
    setup_line_int(j++,900,1200,1500,1200,0);
    setup_line_int(j++,1200,-300,1800,-300,0);
    setup_line_int(j++,-900,1200,-900,1800,0);
    setup_line_int(j++,-300,-300,-600,-300,0);
    setup_line_int(j++,-300,900,-300,-300,0);
    setup_line_int(j++,0,900,-300,900,0);
    setup_line_int(j++,0,1200,0,900,0);
    setup_line_int(j++,1500,-1500,1500,-300,0);
    setup_line_int(j++,-300,-1500,-300,-1200,0);
    setup_line_int(j++,-600,300,-600,600,0);
    setup_line_int(j++,0,0,-300,0,0);
    setup_line_int(j++,-900,-1500,-900,-900,0);
    setup_line_int(j++,0,-1500,0,-1800,0);
    setup_line_int(j++,-600,-1500,-600,-1800,0);
    setup_line_int(j++,0,-600,0,-300,0);
    setup_line_int(j++,-1200,-600,0,-600,0);
    setup_line_int(j++,-1200,-900,-1200,-600,0);
    setup_line_int(j++,-1500,-900,-1200,-900,0);
    setup_line_int(j++,-1500,0,-1500,-900,0);
    setup_line_int(j++,-1200,1500,-1200,0,0);
    setup_line_int(j++,-1500,1500,-1200,1500,0);
    setup_line_int(j++,600,600,0,600,0);
    setup_line_int(j++,600,0,600,600,0);
    setup_line_int(j++,900,0,600,0,0);
    setup_line_int(j++,900,-600,900,0,0);
    setup_line_int(j++,1200,-600,900,-600,0);
    setup_line_int(j++,1200,-1500,1200,-600,0);
    setup_line_int(j++,300,300,-300,300,0);
    setup_line_int(j++,300,-900,300,300,0);
    setup_line_int(j++,0,-900,300,-900,0);
    setup_line_int(j++,-1500,1200,-1800,1200,0);
    setup_line_int(j++,-600,0,-900,0,0);
    setup_line_int(j++,-600,900,-1200,900,0);
    setup_line_int(j++,-600,1500,-600,900,0);
    setup_line_int(j++,-1500,600,-1200,600,0);
    setup_line_int(j++,-1500,900,-1500,600,0);
    setup_line_int(j++,1500,300,600,300,0);
    setup_line_int(j++,1500,0,1500,300,0);
    setup_line_int(j++,1200,0,1500,0,0);
    setup_line_int(j++,1500,1500,1800,1500,0);
    setup_line_int(j++,1200,1500,1200,900,0);
    setup_line_int(j++,-1500,300,-1800,300,0);
    setup_line_int(j++,-1500,-1500,-1500,-1800,0);
    setup_line_int(j++,-1200,-1500,-1500,-1500,0);
    setup_line_int(j++,-300,-900,-300,-600,0);
    setup_line_int(j++,-600,-900,-300,-900,0);
    setup_line_int(j++,900,-900,600,-900,0);
    setup_line_int(j++,-900,600,-300,600,0);
    setup_line_int(j++,900,1500,900,1800,0);

		for(;;) {
			for(uint8_t i = 0; i < j; ++i) {
				execute_line(i);
			}
		}
}
