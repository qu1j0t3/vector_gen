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

#include "hw_impl.h"
#include "../portable/display_list.h"

#define DD_MOSI 3
#define DD_SCK 5

void io_line_start(uint8_t flags) {
  PORTD = PORTD_Z_BLANK_MASK |
          PORTD_Z_ENABLE_MASK |
          PORTD_INT_HOLD_MASK |
          (flags & LINE_LIMIT_LOW_MASK ? 0 : PORTD_LIMIT_LOW_MASK) |
          (flags & LINE_LIMIT_X_MASK ? PORTD_X_COMP_SEL_MASK : PORTD_Y_COMP_SEL_MASK);
}

void spi(uint8_t cs, uint16_t word) {
  // Select addressed DAC
  PORTC = cs;

  SPDR = word >> 8;
  while(!(SPSR & (1 << SPIF)))
    ;

  SPDR = word;
  while(!(SPSR & (1 << SPIF)))
    ;

  // Deselect all DACs
  PORTC = DAC_LIMIT | DAC_POS | DAC_COEFF;
}

void hw_setup() {
  PORTB = PORTC = PORTD = 0; // disables pullup on STOP pin (PORTC)

  // set pin directions
  DDRB = PORTB_OUTPUT_MASK; // SPI MOSI/SCK and utility pin 0
  DDRC = PORTC_OUTPUT_MASK; // DAC chip select outputs
  DDRD = PORTD_OUTPUT_MASK; // drawing control outputs

  /* Enable SPI, Master, set clock rate fck/4
    SPE: 1 (enabled)
    DORD: 0 (MSB first)
    MSTR: 1 (master mode)
    CPOL: 0 (SCK low when idle)
    CPHA: 0 (sample on leading edge)
    SPR1,0: 0, 0 (Fosc/4)
  */
  SPCR = (1<<SPE) | (1<<MSTR);
  SPSR = (1<<SPI2X); // double speed in master mode (Fosc/2)
}



// see: https://www.appelsiini.net/2011/simple-usart-with-avr-libc/

#define BAUD 115200
#include <util/setbaud.h>

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void uart_putchar(char c) {
   loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
   UDR0 = c;
}

char uart_getchar(void) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

char uart_getchar_poll(void) {
    return UCSR0A & (1 << RXC0) ? UDR0 : 0;
}

void uart_puts(char *s) {
   while(*s) {
    uart_putchar(*s++);
   }
}