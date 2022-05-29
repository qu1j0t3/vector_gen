CC=/Applications//Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-gcc
OBJCOPY=/Applications//Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-objcopy
CPPFLAGS=-D__AVR_ATmega168__
CFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega168
LD=$(CC)
LDFLAGS=-mmcu=atmega168 -Wl,-Map,arduino.map

arduino.hex : arduino
	$(OBJCOPY) -O ihex -R .eeprom $^ $@

arduino : arduino.o

clean : ; rm -f arduino arduino.hex arduino.o arduino.map arduino.s

d : arduino.o
	/Applications//Arduino.app/Contents/Java/hardware/tools/avr/bin/avr-objdump -h -S $^

arduino.s : arduino.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -S $^

s : arduino.s

#
# avrdude -p m168 -c arduino -P /dev/cu.usbserial-A6008hFI -U flash:w:arduino.hex

