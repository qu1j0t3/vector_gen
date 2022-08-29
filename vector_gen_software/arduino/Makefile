TOOLBIN=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin
CC=$(TOOLBIN)/avr-gcc
OBJCOPY=$(TOOLBIN)/avr-objcopy
CPPFLAGS=-D__AVR_ATmega328__ -I.
CFLAGS=-flto -Os -DF_CPU=16000000UL -mmcu=atmega328 -Wall -Wextra
LDFLAGS=-flto -mmcu=atmega328 -Wl,-Map,arduino.map
#CPPFLAGS=-D__AVR_ATmega168__ -I.
#CFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega168 -Wall -Wextra
#LDFLAGS=-mmcu=atmega168 -Wl,-Map,arduino.map

LD=$(CC)

vpath %.c ../portable

obj/%.o : %.c ; $(CC) -o $@ -c $< $(CFLAGS) $(CPPFLAGS)

OBJ = $(addprefix obj/, main.o hw_impl.o display_list.o isqrt.o tests.o \
												demo_square.o demo_boxes.o demo_maze.o \
												demo_rocks.o demo_starburst.o demo_horiz.o)

arduino.hex : arduino
	$(OBJCOPY) -O ihex -R .eeprom $^ $@
	@ ls -l $@

arduino : $(OBJ) ; $(CC) -o $@ $^ $(CFLAGS)

clean : ; rm -f arduino arduino.hex arduino.map $(OBJ)

%.s : %.c ; $(CC) -S -c $< $(CFLAGS) $(CPPFLAGS)

#
# make arduino.hex && avrdude -p m168 -c arduino -P /dev/cu.usbserial-A6008hFI -U flash:w:arduino.hex

# 328:
# make arduino.hex && avrdude -p m328 -c arduino -P /dev/cu.usbserial-A6008hFI -b 38400 -U flash:w:arduino.hex


