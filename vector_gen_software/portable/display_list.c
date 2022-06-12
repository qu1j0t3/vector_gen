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

#include <stdint.h>
#include <stdlib.h>

#include "display_list.h"
#include "isqrt.h"
#include "hardware.h"

#include "hw_impl.h"

#define NORMALISE_DIRECTIONS 0
#define VARIABLE_SETTLING 0  // need to benchmark this

// If FRAME_SYNC is 1, then the execution of the display list
// will wait until at least one PIT tick has elapsed (see init code),
// producing a frame rate that is somewhat decoupled from
// the size of the display list

#define FRAME_SYNC 0

#define MAX_Z_LEVEL 0xfffu

// FIXME 32 bit dash masks are probably far too heavy for an 8 bit processor
//       since they have to be evaluated in a tight loop during drawing
uint32_t line_dash_style[16] = {
		0,
		0b100100100100100100100100100100,
		0b111110000011111000001111100000,
		~0b110000000011000000001100000000
};

uint16_t pos_dac_x[DISPLAY_LIST_MAX],
         pos_dac_y[DISPLAY_LIST_MAX],
         limit_dac[DISPLAY_LIST_MAX],
         xcoeff[DISPLAY_LIST_MAX],
         ycoeff[DISPLAY_LIST_MAX];

uint8_t line_flags[DISPLAY_LIST_MAX], // low 4 bits are dash style index (or dwell time, for points)
	    	settle_delay[DISPLAY_LIST_MAX]; // parameter for delay()

int8_t limit_adj_units_x = -5;  // FIXME: These must be measured for
int8_t limit_adj_units_y = -12; //        the actual ICs being used

uint8_t ptx[COARSE_POINT_MAX], pty[COARSE_POINT_MAX];

#define LINE_LIMIT_X(i)   (line_flags[i] & LINE_LIMIT_X_MASK)
#define LINE_LIMIT_LOW(i) (line_flags[i] & LINE_LIMIT_LOW_MASK)
#define LINE_ACTIVE(i)    (line_flags[i] & LINE_ACTIVE_MASK)
#define IS_POINT(i)       (line_flags[i] & IS_POINT_MASK)

volatile uint32_t ticks;

int compar_func(const void *pa, const void *pb) {
	uint16_t a = *(const uint16_t*)pa;
	uint16_t b = *(const uint16_t*)pb;

	// sort lines left to right by X starting position
	int sign = (pos_dac_x[a] & 0xfff) - (pos_dac_x[b] & 0xfff);
	return sign ? sign : (
		// Then sort by Y
		(pos_dac_y[a] & 0xfff) - (pos_dac_y[b] & 0xfff)
	);
}

void sort_display_list(uint16_t count, uint16_t perm[]) {
	qsort(perm, count, sizeof(perm[0]), compar_func);
}

void shuffle_display_list(uint16_t count, uint16_t perm[]) {
	for(unsigned n = count; n > 1;) {
		unsigned chosen = (uint16_t)rand() % n;
		--n;
		uint16_t temp = perm[n];
		perm[n] = perm[chosen];
		perm[chosen] = temp;
	}
}

/*
 * Parameters:  x0, y0, x1, y1 : line endpoints; range from -2047 to +2048
 *              dash           : 30 bits of dash pattern; zero for solid line
 *              zlevel         : Z intensity level from 0 (lowest) to 4095 (highest)
                                 but hardware has 8 bit precision
 */

uint16_t setup_line_int_(uint16_t i, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t dash) {

	// VARIABLE_SETTLING is an EXPERIMENTAL optimisation that precomputes
	// a settling delay for DAC setup before each line.
	// CAREFUL: This assumes that the display list is not going to be permuted in any way!
	// Also it doesn't seem to be a very powerful optimisation

	static uint16_t last_pos_x, last_pos_y, last_z, last_xcoeff, last_ycoeff, last_limit_x, last_limit_y;
	if (i == 0) {
		last_pos_x = last_pos_y = last_z = last_xcoeff = last_ycoeff = last_limit_x = last_limit_y = 0;
	}

	int16_t dx = x1-x0, dy = y1-y0, posx, posy;

	// treat very short lines as points (since crt cannot resolve anyway)
	// can be calibrated using short lines test pattern below
	// at some point around length 7 units, lines start to lose brightness

	uint8_t line_limit_x = abs(dx) > abs(dy);

	int16_t larger_delta = line_limit_x ? dx : dy;

	uint8_t line_limit_low = larger_delta > 0;

	uint8_t is_point = larger_delta == 0;


	// x coeff and y coeff and limit DAC use the DAC gain x 2 ... i.e. slew volts = 2 * 2.5 * delta/4096
	// x pos and y pos use the DAC gain x 1                   ... i.e. slew volts =     2.5 * delta/4096

	uint16_t max_slew = 0; // this is in gain x 1 units

	if (is_point) {
		posx = 2048 - (x0+x1)/2;
		posy = 2048 - (y0+y1)/2;
	} else {
		posx = 2048 - x0;
		posy = 2048 - y0;

		// DAC Datasheet output swing = typ 0.01 to Vdd-0.04
		// When powered from USB, Vdd can swing as low as 4.53V-4.80V in my setup (usb hub with other devices connected)
		// i.e. 0.01 to 4.76 which is a scale of 0.952 x full range. (WORST case 0.898)
		// Use 0.9x for now to give a little margin for Vdd
		// N.B. This is best verified by using the Starburst test below. When Vdd is too low,
		//      coefficient error near Vdd causes lines to meet and cross some distance from the centre of the pattern.
		//      When the full coefficient range is available, lines should meet close to the centre.

		// Update: Even when analog section is powered from a regulated 5V supply,
		//         0.9 x full scale is about the maximum we can do before beginning to hit nonlinearity.

		// Integrator Vin = max 2.25V above "zero"
		// R = 15kΩ   C = 10nF
		// dV/dt = 2.25 / (15000*0.00000001) V/second = 0.015 V/µs

    // normalise the signed direction vector
    // multiply by 0.9
    // multiply by 2047
    // add 2048

    // premultiply the length denominator for a little more precision in the square root
    // implies a divide-by-ten in xcf, ycf calculation
	  uint16_t len = isqrt(100*((int32_t)dx*dx + (int32_t)dy*dy));
		uint16_t xcf = 2048 + ((int32_t)dx*2047*9)/len;
		uint16_t ycf = 2048 + ((int32_t)dy*2047*9)/len;

		int16_t dxc = 2*(xcf - last_xcoeff);
		if (!VARIABLE_SETTLING || dxc) {
			xcoeff[i] = DAC_A | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | xcf;
			last_xcoeff = xcf;
			if(abs(dxc) > max_slew) max_slew = abs(dxc);
		} else {
			xcoeff[i] = 0;
		}

		int16_t dyc = 2*(ycf - last_ycoeff);
		if (!VARIABLE_SETTLING || dyc) {
			ycoeff[i] = DAC_B | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | ycf;
			last_ycoeff = ycf;
			if(abs(dyc) > max_slew) max_slew = abs(dyc);
		} else {
			ycoeff[i] = 0;
		}

		uint16_t delta = (uint16_t)abs(larger_delta)/2;
		int16_t clamp;
		if (line_limit_low) {
			// limit = 2048 - delta
			clamp = delta > 2048 ? 0 : 2048 - delta;
		} else {
			// limit = 2048 + delta
			clamp = delta > 2047 ? 4095 : 2048 + delta;
		}

		// While the limit DAC can use almost the whole range between 0 and 5V (with integrator "zero" at 2.5V),
		// the integrators themselves cannot reach these limits. We therefore need to clamp the limit DAC
		// to a reduced, practical range, or the system will stop, waiting forever for a threshold that can't be reached.
		// In tests with LF412CP op amp, the integrators can rise to about 4V (+1.5V), and drop to about 1.3V (-1.2V).
		// With a different rail to rail amp, these limits can be increased.
		// FIXME: This range may not be enough! Because the scope is typically calibrated to 1V full deflection.
		// (As a failsafe, we might need a timeout as well.)
		// TODO: This can be self calibrating

		/* Update: Changed to MCP6292 op amp for integrator and removed the clamping.
		int32_t limit_max = (int32_t)( (4.95/5.0)*0xfffu );
		int32_t limit_min = (int32_t)( (0.05/5.0)*0xfffu );
		uint16_t clamped = limit; //(uint16_t)( limit < limit_min ? limit_min : (limit > limit_max ? limit_max : limit) );*/

		// The limit DAC may have an offset relative to position DAC,
		// which leads lines to overshoot or undershoot their limit.
		// Add a correction which can either be manually measured with DVM,
		// or self calibrated (TODO). (limit_adj_units_x/y)

		if (line_limit_x) {
			clamp += limit_adj_units_x;
			limit_dac[i] = (uint16_t)( DAC_A | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | (uint16_t)clamp );
			int16_t dlimit = 2*(clamp - last_limit_x);
			last_limit_x = clamp;
			if(abs(dlimit) > max_slew) max_slew = abs(dlimit);
		} else {
			clamp += limit_adj_units_y;
			limit_dac[i] = (uint16_t)( DAC_B | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | (uint16_t)clamp );
			int16_t dlimit = 2*(clamp - last_limit_y);
			last_limit_y = clamp;
			if(abs(dlimit) > max_slew) max_slew = abs(dlimit);
		}
	}

	int16_t dxp = posx - last_pos_x;
	if (!VARIABLE_SETTLING || dxp) {
		pos_dac_x[i] = DAC_A | DAC_BUFFERED | DAC_GAINx1 | DAC_ACTIVE | (uint16_t)posx;
		last_pos_x = posx;
		if(abs(dxp) > max_slew) max_slew = abs(dxp);
	} else {
		pos_dac_x[i] = 0; // do not update
	}

	int16_t dyp = posy - last_pos_y;
	if (!VARIABLE_SETTLING || dyp) {
		pos_dac_y[i] = DAC_B | DAC_BUFFERED | DAC_GAINx1 | DAC_ACTIVE | (uint16_t)posy;
		last_pos_y = posy;
		if(abs(dyp) > max_slew) max_slew = abs(dyp);
	} else {
		pos_dac_y[i] = 0; // do not update
	}


	// Compute expected slew time
	// Datasheet has slew rate of 0.55V/µs and typical settling time of 4.5µs
	// slew volts = 2.5 * max_slew / 4096
	// slew microseconds = 2.5 * max_slew / (4096 * 0.55)
	// delay() parameter = 100 * 2.5 * max_slew / (4096 * 0.55 * 12.5)
	//                   ~= max_slew / 112    (how slow is integer divide??)

	settle_delay[i] = max_slew/112;


	// suppress lines that push DACs out of bounds
	// TODO: proper clipping
	uint8_t active = posx >= 0 && posx < 0x1000 && posy >= 0 && posy < 0x1000;

	line_flags[i] = (is_point ? IS_POINT_MASK : 0)
                | (active ? LINE_ACTIVE_MASK : 0)
                | (line_limit_x ? LINE_LIMIT_X_MASK : 0)      // set if X is faster changing integrator
                | (line_limit_low ? LINE_LIMIT_LOW_MASK : 0) // set if the integrator is decreasing (coefficient positive))
                | dash;

	return active;
}

uint16_t setup_line_int(uint16_t i, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t dash) {
	/* This higher level call will try to position line in a normalised direction (X increasing, Y increasing)
	// and if that leads to a starting position outside DAC limits,
	// will then try to place line reversed (which is probably not so good for display list sorting
	// but necessary to have the partial line rendered at all).*/

	if (NORMALISE_DIRECTIONS && x0 > x1) {
		return setup_line_int_(i, x1, y1, x0, y0, dash)
				|| setup_line_int_(i, x0, y0, x1, y1, dash);
	}

	return setup_line_int_(i, x0, y0, x1, y1, dash)
			|| setup_line_int_(i, x1, y1, x0, y0, dash);
}

// Modulo is quite slow on this processor, so use an
// array lookup as a fast but constant time "mod 30"
static uint8_t next[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,0};

// Instead of the vector display list with its precomputed DAC words,
// use the ptx[] and pty[] arrays (compact display list of points)
void execute_pt(uint16_t i) {
	static uint16_t last_pos_x, last_pos_y;

	if (i == 0) { // reset state at start of display list
		last_pos_x = last_pos_y = 0;
	}

	uint16_t dac_x = DAC_A | DAC_BUFFERED | DAC_GAINx1 | DAC_ACTIVE | (uint16_t)(2048 + (((int)ptx[i] - 128)*48));
	uint16_t dac_y = DAC_B | DAC_BUFFERED | DAC_GAINx1 | DAC_ACTIVE | (uint16_t)(2048 - (((int)pty[i] - 128)*48));

	unsigned delta = abs(dac_x - last_pos_x);
	if (abs(dac_y - last_pos_y) > delta) {
		delta = abs(dac_y - last_pos_y);
	}

	if (dac_x != last_pos_x) {
		spi(DAC_POS, dac_x);
		last_pos_x = dac_x;
	}

	if (dac_y != last_pos_y) {
		spi(DAC_POS, dac_y);
		last_pos_y = dac_y;
	}

	// This delay depends greatly on the amount
	// of voltage slew the DAC needs to do
	// Note op amp slew is also involved! (summing amp)
	// dly is the change in DAC value (max 4096)
	delay(delta/112);

	// Unblank Z
  IO_UNBLANK_Z();

	delay(20); // This duration can be adjusted!

	IO_BLANK_Z();
}

volatile uint8_t stop_flag;

void execute_line(uint16_t i) {
	static uint16_t last_pos_x, last_pos_y;

	if (i == 0) { // reset state at start of display list
		last_pos_x = last_pos_y = 0;
	}

	if(FRAME_SYNC && i == 0) {
		// Sync to timer interrupt
		for(uint32_t current_tick = ticks; current_tick == ticks;)
			;
	}

	if(!(line_flags[i] & LINE_ACTIVE_MASK)) return;

	if (line_flags[i] & IS_POINT_MASK) {
		// Assume this initial state
		//BOARD_INITPINS_Z_ENABLE_FGPIO->PCOR = BOARD_INITPINS_Z_ENABLE_GPIO_PIN_MASK;

		IO_POINT_SETUP();

		uint16_t max_slew = 0;
		uint16_t dx = abs(pos_dac_x[i] - last_pos_x);
		if (dx) {
			spi(DAC_POS, pos_dac_x[i]);
			last_pos_x = pos_dac_x[i];
			if (dx > max_slew) {
				max_slew = dx;
			}
		}
		uint16_t dy = abs(pos_dac_y[i] - last_pos_y);
		if (dy) {
			spi(DAC_POS, pos_dac_y[i]);
			last_pos_y = pos_dac_y[i];
			if (dy > max_slew) {
				max_slew = dy;
			}
		}

		delay(max_slew/112);

    if(i == 0) IO_RAISE_TRIGGER();

    IO_ENABLE_AND_UNBLANK_Z();

    uint8_t dwell = (line_flags[i] & 0xf) << 2;
		delay(dwell ? dwell : 3); // This duration can be adjusted!

		IO_DISABLE_AND_BLANK_Z();

    IO_DROP_TRIGGER();

		return;
	}

	// We cannot release HOLD and RESET until coefficient DACs have settled.

	// Set either a high-crossing or low-crossing threshold at the limit DAC.
	// If limitlow[i] is set, it means we must invert the comparator output
	spi(DAC_LIMIT, limit_dac[i]);

	if (xcoeff[i]) {
		spi(DAC_COEFF, xcoeff[i]);
	}
	if (ycoeff[i]) {
		spi(DAC_COEFF, ycoeff[i]);
	}

	if (pos_dac_x[i]) {
		spi(DAC_POS, pos_dac_x[i]);
	}
	if (pos_dac_y[i]) {
		spi(DAC_POS, pos_dac_y[i]);
	}

	if (VARIABLE_SETTLING) {
		delay(settle_delay[i]); // May need fudge factor.. or overheads may compensate
	} else {
	    // Add some settling time for limit DAC. Without this, some lines may be dropped/truncated.
		four_microseconds();
		//four_microseconds();
		//four_microseconds();
	}


	// By testing the display list in random order, we can see that repeatability
	// is not very good even if ample settling time (e.g. 12µs) is allowed here.
	//   Therefore the display list should be ordered for good locality,
	// not just in position, but also coefficients to an extent, which tend to slew far
	// even for close-together lines.
	//   TODO: Try running the coefficient DACs at GAIN X 1
	// (can compensate by halving the integration resistors), which makes the slew faster?
	// TODO: Although it's still unclear whether buffered/unbuffered affects this.

  uint32_t dash = line_dash_style[line_flags[i] & 0xf];


	// If an interrupt occurs in the wait loop, the dash pattern will visibly shimmer.
	// So they must be disabled.
	HW_DISABLE_INTRS();


  if(i == 0) IO_RAISE_TRIGGER();


  // Z = Z_ENABLE & (HOLD ^ Z_BLANK ^ STOP)
  // HOLD is 0 between lines
  // STOP should be a don't-care because Z_ENABLE = 0

  // During line drawing: HOLD = 1, STOP = 1
  // Z = 1 & Z_BLANK

  // Z_ENABLE is master switch on output
  // LIMIT_LOW ^ STOP should be 1 right now
  // Set conditions so that HOLD going high unblanks


	//stop_flag = 0;
	//KBI_EnableInterrupts(KBI0);// This is quite slow

	// All the above takes about 30-32 µs

  IO_OPEN_RESET();

  io_line_start(line_flags[i]);

	// Wait integrating time

	if (dash) {
		for(uint32_t dash_mask = 0; IO_GET_STOP(); dash_mask = next[dash_mask]) {
			if(dash & (1u << dash_mask)) {
				IO_BLANK_Z();
			} else {
				IO_UNBLANK_Z();
			}
		}
		// it may be important to leave this in a known state
		// (debugging weird flashes at the end of the dashed lines)
		IO_UNBLANK_Z();

	  HW_ENABLE_INTRS();
	} else {
		// solid line
		while(IO_GET_STOP())
			;
	}

  IO_END_LINE();

  IO_CLOSE_RESET();

	IO_DROP_TRIGGER();
}
