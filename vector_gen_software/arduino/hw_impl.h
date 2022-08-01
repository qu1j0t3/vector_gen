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

#ifndef __VG_HW_IMPL__

#define __VG_HW_IMPL__

#include <avr/io.h>
#include <util/delay.h>

// GPIO map for PCB - Arduino Uno R3
//
// (NXP)      R3?     STM32    (Duemilanove)
// -----------------------------------------
//
// PTF7       C0      PA0      A0 PC0 (J2) 6     NOTCS_DAC_0_GPIO_PIN_MASK (LIMIT)
// PTC0       C1      PA1      A1 PC1      5     NOTCS_DAC_1_GPIO_PIN_MASK (POS)
// PTC1       C2      PA4      A2 PC2      4     NOTCS_DAC_COEFF_GPIO_PIN_MASK
// PTC2       C3      PB0      A3 PC3      3     STOP_GPIO_PIN_MASK ----- PCINT11 on 168
// PTC3       C4      PC1/PB9  A4 PC4      2     -- Utility  - Put test point on PCB "analog in 1" "adc4"
// PTF6       C5      PC0/PA15 A5 PC5      1     -- Utility  - Put test point on PCB "analog in 0" "adc5"

#define PORTC_OUTPUT_MASK 0b000111
#define PORTC_STOP_MASK   (1 << 3)
#define PORTC_5 (1 << 5)
#define PORTC_4 (1 << 4)

// chip select - inverted at DAC
#define DAC_LIMIT  0b110
#define DAC_POS    0b101
#define DAC_COEFF  0b011


// PTB2       B5      PA5      D13 PB5 (SCK)  6  SPI_CLOCK_GPIO_PIN_MASK
// PTB4       B4      PA6      D12 PB4 (MISO) 5
// PTB3       B3      PA7      D11 PB3 (MOSI) 4  SPI_DATAOUT_GPIO_PIN_MASK
// PTB5       B2      PB6      D10 PB2 (SS)   3  ** must be configured as an output, although it is not connected; see Atmega 168 datasheet 19.3.2
// PTH0       B1      PC7      D9  PB1        2  INT_RESET_GPIO_PIN_MASK (a - optional)
// PTA0       B0      PA9      D8  PB0        1  -- Utility  - Put test point on PCB

#define PORTB_OUTPUT_MASK    0b101111
#define PORTB_INT_RESET_MASK (1 << 1)
#define PORTB_TRIGGER_MASK   (1 << 0) // Scope trigger for testing

// PTA7       D7      PA8      D7 PD7  (J1)   8  Z_ENABLE_GPIO_PIN_MASK   (a - optional)
// PTB4       D6      PB10     D6 PD6         7  LIMIT_LOW_GPIO_PIN_MASK  (a)
// PTD1       D5      PB4      D5 PD5         6  X_COMP_SEL_GPIO_PIN_MASK (a)
// PTA1       D4      PB5      D4 PD4         5  Y_COMP_SEL_GPIO_PIN_MASK (a)
// PTD0       D3      PB3      D3 PD3         4  INT_HOLD_GPIO_PIN_MASK (a)
// PTD5       D2      PA10     D2 PD2         3  Z_BLANK_GPIO_PIN_MASK (a)
// PTB1       D1      PA2/PC4  D1 PD1 (TX)    2  -- Utility  - Put test point on PCB
// PTB0       D0      PA3/PC5  D0 PD0 (RX)    1  -- Utility  - Put test point on PCB

#define PORTD_OUTPUT_MASK     0b11111100
#define PORTD_Z_BLANK_MASK    (1 << 2)
#define PORTD_INT_HOLD_MASK   (1 << 3)
#define PORTD_Y_COMP_SEL_MASK (1 << 4)
#define PORTD_X_COMP_SEL_MASK (1 << 5)
#define PORTD_LIMIT_LOW_MASK  (1 << 6)
#define PORTD_Z_ENABLE_MASK   (1 << 7)

#define IO_UNBLANK_Z() (PORTD |= PORTD_Z_BLANK_MASK)
#define IO_BLANK_Z()   (PORTD &= ~PORTD_Z_BLANK_MASK)
#define IO_BLANK_Z_BIT(flag) (PORTD = (PORTD & ~PORTD_Z_BLANK_MASK) | ((flag) << 2))

#define IO_ENABLE_AND_UNBLANK_Z() (PORTD |= PORTD_Z_ENABLE_MASK | PORTD_Z_BLANK_MASK)
#define IO_DISABLE_AND_BLANK_Z()  (PORTD &= ~(PORTD_Z_ENABLE_MASK | PORTD_Z_BLANK_MASK))

// Z output equation:
// OUT = Z_ENABLE(switch) * (((COMP ^ LIMIT_LOW) ^ Z_BLANK) ^ HOLD)
// When X_COMP_SEL=0 and Y_COMP_SEL=0 then COMP = 0 (and/or select truth table)

// IO_POINT_SETUP() sets up logic so that Z output can be gated by Z_BLANK line (and Z_ENABLE switch)
#define IO_POINT_SETUP() \
  (PORTD &= ~( PORTD_Z_BLANK_MASK | \
               PORTD_INT_HOLD_MASK | \
               PORTD_Y_COMP_SEL_MASK | \
               PORTD_X_COMP_SEL_MASK | \
               PORTD_LIMIT_LOW_MASK ))

#define IO_GET_STOP() (PINC & PORTC_STOP_MASK)

#define IO_END_LINE() (PORTD &= ~(PORTD_Z_ENABLE_MASK | PORTD_INT_HOLD_MASK | PORTD_Z_BLANK_MASK));

#define IO_RAISE_TRIGGER() (PORTB |= PORTB_TRIGGER_MASK)
#define IO_DROP_TRIGGER()  (PORTB &= ~PORTB_TRIGGER_MASK)

#define IO_OPEN_RESET()  (PORTB &= ~PORTB_INT_RESET_MASK)
#define IO_CLOSE_RESET() (PORTB |= PORTB_INT_RESET_MASK)

#define HW_DISABLE_INTRS() ;
#define HW_ENABLE_INTRS() ;

#define four_microseconds() _delay_us(4)

// Variable delay

// These timings from NXP:
//delay(0); // approx 7-8 ns
//delay(100); // approx 12.56µs
//delay(1000); // approx 125µs

// Arduino gives about 18.8µs for x = 100 (a bit longer than NXP board)
#define delay(x) _delay_loop_1(x)

void uart_putchar(char c);
char uart_getchar(void);
char uart_getchar_poll(void);
void uart_init(void);
void uart_puts(char *s);

void io_line_start(uint8_t flags);
void hw_setup();

#endif // __VG_HW_IMPL__

