# Board assembly

## General

- Test points can be installed as needed;
  1.2mm holes suit Digikey 36-4952-ND
- IC orientations are consistent, pin 1 to left looking at front
  with power connection at right.


## Power

- Modular power supply is +5/-5V, Dura Micro MD5122-A1
- Connector J6
- R2: 330R or 270R
- LED D1
- test with +5V & GND on J6

## Voltage reference

- U13 LM336
- R1 2.2k
- with power applied, check VREF2.5 test point

## 5V MCU

- install J7, J8, J9, J10
  - Only the 2 GND pins need be soldered for J7

## 3.3V MCU

- install J1, J2, J3, J5
  - Only the 2 GND pins need be soldered for J1
- install U16, U17 level shifting buffers
- install RN2, R29, R30, R33, R3, R28

## Coefficient DACs

- install U1 MCP4922
- check levels with step voltage program (TODO: identify that program)

## Integrators

- install U6 - tested with MCP6292
  - needs to be R2R output at least, does not need to be fast, as slew rates are < 0.05V/µs
- caps C11 and C12 must be matched closely.
  - I use an RC Schmitt oscillator and frequency counter to get relative values,
    sort, and take closest pairs.
- install R5, R7
  - these should be closely matched
  - tested between 4.7k (fast) and 15k (slow); should work even faster
    but with more timing error and noise susceptibility (I think)
- check integrator ramps with test program (TODO: identify that program)

## Switches

- install U5 SN74LV4066AN
- install R34, R35 47R - current limiting resistors
- check integrator run and reset with test program (TODO: identify that program)



