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

// Dash styles are defined as a vector of six bits
// that repeat along the line
uint8_t line_dash_style[] = {
		0,
		0b001001,
		0b000111,
		0b011111
};

int8_t dac_limit_x_adj = DAC_LIMIT_X_ADJ;
int8_t dac_limit_y_adj = DAC_LIMIT_Y_ADJ;

uint8_t ptx[COARSE_POINT_MAX], pty[COARSE_POINT_MAX];

#define END_OBJECT(line)     (line->flags & END_OBJECT_MASK)
#define LINE_LIMIT_X(line)   (line->flags & LINE_LIMIT_X_MASK)
#define LINE_LIMIT_LOW(line) (line->flags & LINE_LIMIT_LOW_MASK)
#define LINE_ACTIVE(line)    (line->flags & LINE_ACTIVE_MASK)
#define IS_POINT(line)       (line->flags & IS_POINT_MASK)

volatile uint32_t ticks;

/*
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
*/

/*
 * Parameters:  x0, y0, x1, y1 : line endpoints; range from -2047 to +2048
 *              dash           : 30 bits of dash pattern; zero for solid line
 */

uint8_t setup_line_int_(struct line *line, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t dash) {

	int16_t dx = x1-x0, dy = y1-y0, posx, posy;

	// treat very short lines as points (since crt cannot resolve anyway)
	// can be calibrated using short lines test pattern below
	// at some point around length 7 units, lines start to lose brightness

	uint8_t line_limit_x = abs(dx) > abs(dy);

	int16_t larger_delta = line_limit_x ? dx : dy;

	uint8_t line_limit_low = larger_delta > 0;

	uint8_t is_point = larger_delta == 0;

	if (is_point) {
		posx = 2048 - (x0+x1)/2;
		posy = 2048 - (y0+y1)/2;
	} else {
		posx = 2048 - x0;
		posy = 2048 - y0;

		// DAC Datasheet output swing = typ 0.01 to Vdd-0.04
		// When powered from USB, Vdd can swing as low as 4.53V-4.80V in my setup (usb hub with other devices connected)
		// i.e. 0.01 to 4.76 which is a scale of 0.952 x full range. (WORST case 0.898)
		// N.B. This is best verified by using the Starburst test below. When Vdd is too low,
		//      coefficient error near Vdd causes lines to meet and cross some distance from the centre of the pattern.
		//      When the full coefficient range is available, lines should meet close to the centre.

		// Update: Even when analog section is powered from a regulated 5V supply,
		//         0.9 x full scale is about the maximum we can do before beginning to hit nonlinearity.

		// Integrator Vin = max 2.25V above "zero"
		// R = 15kΩ   C = 10nF
		// dV/dt = 2.25 / (15000*0.00000001) V/second = 0.015 V/µs

    // premultiply the length denominator for a little more precision in the square root
		// max dx, dy: 4096 = 2^12; squared and summed: 2^25; then the scale factor must be < 6 bits!
		// choose the largest possible square scale factor of 64 and divisor of 7 i.e. 7/8 = 0.875
	  uint16_t len = isqrt(64*((int32_t)dx*dx + (int32_t)dy*dy));
		uint16_t xcf = 2048 + ((int32_t)dx*2047*7)/len;
		uint16_t ycf = 2048 + ((int32_t)dy*2047*7)/len;

		line->x_coeff = DAC_A | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | (xcf+DAC_COEFF_X_ADJ);
		line->y_coeff = DAC_B | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | (ycf+DAC_COEFF_Y_ADJ);

		// ** Important: The Limit DACs are not _absolute_ voltages,
		//               but are compared to integrator outputs.
		//               Integrator "zero" (i.e. start of line) is at 2.5v,
		//               or 2048 in Limit DAC input units.

		// We run the position DACs at gain x1 and the limit DACs at gain x2
		// so divide position delta by 2 to get the limit DAC input delta.

		// To ensure integration will stop even if the "delta" overflows,
		// we clamp the limit DAC output to the correct extreme.

		// TODO: Review this factor. With the MCP6292 I think I am seeing
		//       the integrators reach ~ +50mV before becoming nonlinear
		//       at a sharp "knee". (Check amp datasheet)
		#define LIMIT_DAC_MAX 2000

		uint16_t delta = (uint16_t)abs(larger_delta)/2;
		int16_t clamp;
		if (line_limit_low) {
			clamp = delta > LIMIT_DAC_MAX ? 0 : 2048 - delta;
		} else {
			clamp = delta > LIMIT_DAC_MAX ? 4095 : 2048 + delta;
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
		// or self calibrated (TODO).

		if (line_limit_x) {
			clamp += dac_limit_x_adj;
			line->limit = (uint16_t)( DAC_A | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | (uint16_t)clamp );
		} else {
			clamp += dac_limit_y_adj;
			line->limit = (uint16_t)( DAC_B | DAC_BUFFERED | DAC_GAINx2 | DAC_ACTIVE | (uint16_t)clamp );
		}
	}

	line->x_pos = DAC_A | DAC_BUFFERED | DAC_GAINx1 | DAC_ACTIVE | (uint16_t)posx;
	line->y_pos = DAC_B | DAC_BUFFERED | DAC_GAINx1 | DAC_ACTIVE | (uint16_t)posy;

	// suppress lines that push DACs out of bounds
	// TODO: proper clipping
	uint8_t active = posx >= 0 && posx < 0x1000 && posy >= 0 && posy < 0x1000;

	line->flags = (is_point ? IS_POINT_MASK : 0)
              | (active ? LINE_ACTIVE_MASK : 0)
              | (line_limit_x ? LINE_LIMIT_X_MASK : 0)      // set if X is faster changing integrator
              | (line_limit_low ? LINE_LIMIT_LOW_MASK : 0) // set if the integrator is decreasing (coefficient positive))
              | dash;

	return active;
}

uint8_t setup_line_int(struct line *line, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t dash) {
	/* This higher level call will try to position line in a normalised direction (X increasing, Y increasing)
	// and if that leads to a starting position outside DAC limits,
	// will then try to place line reversed (which is probably not so good for display list sorting
	// but necessary to have the partial line rendered at all).*/

	if (NORMALISE_DIRECTIONS && x0 > x1) {
		return setup_line_int_(line, x1, y1, x0, y0, dash)
				|| setup_line_int_(line, x0, y0, x1, y1, dash);
	}

	return setup_line_int_(line, x0, y0, x1, y1, dash)
			|| setup_line_int_(line, x1, y1, x0, y0, dash);
}

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
	// Assumes this was run before any points are drawn:
	// IO_POINT_SETUP()
  IO_UNBLANK_Z();

	delay(20); // This duration can be adjusted!

	IO_BLANK_Z();
}

enum{DO_NOT_USE_INTERRUPT, USE_INTERRUPT};

struct line *object[MAX_OBJECTS];

volatile uint8_t task_done;
volatile uint8_t current_object;
struct line *current_line;

static void execute_line_top(struct line *line, uint8_t use_interrupt) {
	static uint16_t last_pos_x, last_pos_y;

	if (!line) { // reset state at start of display list
		last_pos_x = last_pos_y = 0;
		return;
	}
/*
	if(FRAME_SYNC && i == 0) {
		// Sync to timer interrupt
		for(uint32_t current_tick = ticks; current_tick == ticks;)
			;
	}*/

	if(!(line->flags & LINE_ACTIVE_MASK)) return;

	if (line->flags & IS_POINT_MASK) {
		// Assume this initial state
		//BOARD_INITPINS_Z_ENABLE_FGPIO->PCOR = BOARD_INITPINS_Z_ENABLE_GPIO_PIN_MASK;

		IO_POINT_SETUP();

		uint16_t max_slew = 0;
		uint16_t dx = abs(line->x_pos - last_pos_x);
		if (dx) {
			spi(DAC_POS, line->x_pos);
			last_pos_x = line->x_pos;
			if (dx > max_slew) {
				max_slew = dx;
			}
		}
		uint16_t dy = abs(line->y_pos - last_pos_y);
		if (dy) {
			spi(DAC_POS, line->y_pos);
			last_pos_y = line->y_pos;
			if (dy > max_slew) {
				max_slew = dy;
			}
		}

		delay(max_slew/112); // TODO: Check this factor; also avoid division

    //if(i == 0) IO_RAISE_TRIGGER();

    IO_ENABLE_AND_UNBLANK_Z();

    uint8_t dwell = (line->flags & 0xf) << 2;
		delay(dwell ? dwell : 3); // This duration can be adjusted!

		IO_DISABLE_AND_BLANK_Z();

    IO_DROP_TRIGGER();

		return;
	}

	// We cannot release HOLD and RESET until coefficient DACs have settled.

	// Set either a high-crossing or low-crossing threshold at the limit DAC.
	spi(DAC_LIMIT, line->limit);

	if (line->x_coeff) {
		spi(DAC_COEFF, line->x_coeff);
	}
	if (line->y_coeff) {
		spi(DAC_COEFF, line->y_coeff);
	}

	if (line->x_pos) {
		spi(DAC_POS, line->x_pos);
	}
	if (line->y_pos) {
		spi(DAC_POS, line->y_pos);
	}

	// Add some settling time for limit DAC. Without this, some lines may be dropped/truncated.
	// (This also reduces ringing/ripple that I noticed on the 2nd pcb build.)

	_delay_us(12);


	// By testing the display list in random order, we can see that repeatability
	// is not very good even if ample settling time (e.g. 12µs) is allowed here.
	//   Therefore the display list should be ordered for good locality,
	// not just in position, but also coefficients to an extent, which tend to slew far
	// even for close-together lines.
	//   TODO: Try running the coefficient DACs at GAIN X 1
	// (can compensate by halving the integration resistors), which makes the slew faster?
	// TODO: Although it's still unclear whether buffered/unbuffered affects this.


  //if(i == 0) IO_RAISE_TRIGGER();


	// If limitlow[i] is set, it means we must invert the comparator output

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

	if (use_interrupt) {
		HW_ENABLE_INTRS();
	}

	// All the above takes about 30-32 µs

  IO_OPEN_RESET();

  io_line_start(line->flags);
}

static void execute_line_bottom() {
  IO_END_LINE();

  IO_CLOSE_RESET();

	IO_DROP_TRIGGER();
}

void execute_line_isr() {
	execute_line_bottom();

	// Need to find the next vector workload and start it

	if (END_OBJECT(current_line)) {
		do {
			++current_object;
		} while (current_object < MAX_OBJECTS && !object[current_object]);

		if (current_object < MAX_OBJECTS) {
			current_line = object[current_object];
		}
	} else {
		++current_line;
	}

	if (current_line) {
		execute_line_top(current_line, USE_INTERRUPT);
	} else {
		task_done = 1;
	}
}

// Start drawing lines async, using STOP interrupt to cue next line

void start_objects() {
	task_done = 0;
	current_object = 0;
	current_line = object[current_object];
	execute_line_top(current_line, USE_INTERRUPT);
}

// Draw the line synchronously.
// Function does not return until line is complete and Z blanked.

void execute_line(struct line *line) {

  uint8_t dash = line_dash_style[line->flags & 0xf];


	// If an interrupt occurs in the wait loop, the dash pattern will visibly shimmer.
	// So they must be disabled.
	if (dash) {
		HW_DISABLE_INTRS();
	}

	execute_line_top(line, DO_NOT_USE_INTERRUPT);

	// Wait integrating time

	if (dash) {
		while(IO_GET_STOP()) {
			IO_BLANK_Z_BIT(dash & 1);
			dash = (dash << 1) | ((dash >> 6) & 1);
		}

	  HW_ENABLE_INTRS();
	} else {
		// solid line
		while(IO_GET_STOP())
			;
	}

	execute_line_bottom();
};
