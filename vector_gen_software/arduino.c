
// Copyright (C) 2018-2022 Toby Thain <toby@telegraphics.com.au>

// Driver software for vector generator board
// Arduino version


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
// PTC3       C4      PC1/PB9  A4 PC4      2     -- Utility  - Put test point on PCB
// PTF6       C5      PC0/PA15 A5 PC5      1     -- Utility  - Put test point on PCB

#define PORTC_OUTPUT_MASK 0b000111

// PTB2       B5      PA5      D13 PB5 (SCK)  6  SPI_CLOCK_GPIO_PIN_MASK
// PTB4       B4      PA6      D12 PB4 (MISO) 5
// PTB3       B3      PA7      D11 PB3 (MOSI) 4  SPI_DATAOUT_GPIO_PIN_MASK
// PTB5       B2      PB6      D10 PB2 (SS)   3  ** must be configured as an output, although it is not connected; see Atmega 168 datasheet 19.3.2
// PTH0       B1      PC7      D9  PB1        2  INT_RESET_GPIO_PIN_MASK (a - optional)
// PTA0       B0      PA9      D8  PB0        1  -- Utility  - Put test point on PCB

#define PORTB_OUTPUT_MASK 0b101111

// PTA7       D7      PA8      D7 PD7  (J1)   8  Z_ENABLE_GPIO_PIN_MASK   (a - optional)
// PTB4       D6      PB10     D6 PD6         7  LIMIT_LOW_GPIO_PIN_MASK  (a)
// PTD1       D5      PB4      D5 PD5         6  X_COMP_SEL_GPIO_PIN_MASK (a)
// PTA1       D4      PB5      D4 PD4         5  Y_COMP_SEL_GPIO_PIN_MASK (a)
// PTD0       D3      PB3      D3 PD3         4  INT_HOLD_GPIO_PIN_MASK (a)
// PTD5       D2      PA10     D2 PD2         3  Z_BLANK_GPIO_PIN_MASK (a)
// PTB1       D1      PA2/PC4  D1 PD1 (TX)    2  -- Utility  - Put test point on PCB
// PTB0       D0      PA3/PC5  D0 PD0 (RX)    1  -- Utility  - Put test point on PCB

#define PORTD_OUTPUT_MASK 0b11111100

// ==================== Microcontroller Dependent definitions

#define DD_MOSI 3
#define DD_SCK 5
#define PB0_UTILITY 0

// chip select - inverted at DAC
#define DAC_LIMIT  0b110
#define DAC_POS    0b101
#define DAC_COEFF  0b011

// ==================== Portable definitions
// units
#define DAC_A      0u
#define DAC_B      (1u << 15)

// control
#define DAC_BUFFERED (1u << 14) // Since we do not buffer the DAC Vrefs off chip, should always be configured buffered
#define DAC_UNBUFFERED 0u
#define DAC_GAINx2   0u
#define DAC_GAINx1   (1u << 13)
#define DAC_ACTIVE   (1u << 12)

// ==================== Microcontroller Dependent code


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

void setup() {
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
  SPCR = (1<<SPE) | (1<<MSTR);// | (1<<SPR1) | (1<<SPR0);
  SPSR = (1<<SPI2X); // double speed in master mode (Fosc/2)
}

int main (void)
{
  setup();

  while (1) {
#if 1
      PORTB |= (1<<PB0_UTILITY);
      spi(DAC_COEFF, DAC_A | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 0);
      spi(DAC_COEFF, DAC_B | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 0);
      PORTB &= ~(1<<PB0_UTILITY);
      _delay_us(20);
      spi(DAC_COEFF, DAC_A | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 2048);
      spi(DAC_COEFF, DAC_B | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 2048);
      _delay_us(20);
      spi(DAC_COEFF, DAC_A | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 4095);
      spi(DAC_COEFF, DAC_B | DAC_UNBUFFERED | DAC_GAINx2 | DAC_ACTIVE | 4095);
      _delay_us(20);
#endif
  }

  return 1;
}
