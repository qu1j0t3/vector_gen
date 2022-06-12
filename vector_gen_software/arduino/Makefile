TOOLBIN=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin
CC=$(TOOLBIN)/avr-gcc
OBJCOPY=$(TOOLBIN)/avr-objcopy
CPPFLAGS=-D__AVR_ATmega168__ -I.
CFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega168 -Wall -Wextra
LD=$(CC)
LDFLAGS=-mmcu=atmega168 -Wl,-Map,arduino.map

vpath %.c ../portable

obj/%.o : %.c ; $(CC) -o $@ -c $< $(CFLAGS) $(CPPFLAGS)

OBJ = $(addprefix obj/, main.o hw_impl.o test1.o test2.o display_list.o demo_square.o isqrt.o)

arduino.hex : arduino
	$(OBJCOPY) -O ihex -R .eeprom $^ $@

arduino : $(OBJ) ; $(CC) -o $@ $^ $(CFLAGS)

clean : ; rm -f arduino arduino.hex arduino.map $(OBJ)

#
# make arduino.hex && avrdude -p m168 -c arduino -P /dev/cu.usbserial-A6008hFI -U flash:w:arduino.hex

