# Vector generator

Copyright (C) 2017-2022 Toby Thain <toby@telegraphics.com.au>

## Purpose

This circuit is designed to produce X, Y and Z (blanking) signals
for an analog CRT such as an oscilloscope or XY monitor.

It is controlled by a microcontroller using one digital input
and 12 digital outputs. The speed of the microcontroller is not
critical to the design; all timing critical functions are executed
in hardware.

## Output specifications

### Monitors tested

* Tektronix 602, 603, 604, 606

## Graphic capabilities

* Positioning to 12 bit precision
* Lines (approx 20,000 short-medium vectors per second)
  - dashed lines (dashing defined by 30 bit vector)
  - speed will be optimised further without needing hardware changes
* Points

## Microcontroller options

Dev board with Arduino Uno pinout. 5V or 3.3V.

### Tested ports

* NXP Freedom KE06Z modified for 5V

### Planned ports

* Arduino Duemilanove, Uno
* STM32 (3.3V)
* Adafruit M4 Express

## Principle of operation

## Output examples/demos

![Maze displayed on Tektronix 602](IMG_9619.jpg)

![Graph enumerations displayed on Tektronix 602](IMG_9609.jpg)

![Game of Life displayed on Tektronix 602, along with blank pcb](IMG_9606s.jpg)

![Dashed random line demo displayed on Tektronix 602](_MG_3877.jpg)

## Modifications

## Other applications

## Version history

* The first version of the board was produced in a batch of 5 by JLCPCB,
  May 2022. These were all claimed by friends and supporters
  even before the PCBs were delivered.

Original design was conceived in 2017/18.

![Breadboard close to first PCB version](_MG_3654s.jpg)

## Future plans

## Credits

I had assistance from many people, mostly via irc, including:

* `joe_z` & `Wohali` with far greater analog expertise
* `larsb`, who encouraged me to build this as an avenue for using real
  vector displays with software emulators of historical computers
* `asjackson`, fellow hacker and friend
* `genii`, who drilled many holes to work around a position defect
  on the first PCB version

## Dedication

Dedicated to my friend `bootnecklad`, 1995–2018.
