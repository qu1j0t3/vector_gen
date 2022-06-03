TOOLBIN=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin
CC=$(TOOLBIN)/avr-gcc
OBJCOPY=$(TOOLBIN)/avr-objcopy
CPPFLAGS=-D__AVR_ATmega168__
CFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega168
LD=$(CC)
LDFLAGS=-mmcu=atmega168 -Wl,-Map,arduino.map

arduino.hex : arduino
	$(OBJCOPY) -O ihex -R .eeprom $^ $@

arduino : arduino.o

clean : ; rm -f arduino arduino.hex arduino.o arduino.map arduino.s

d : arduino.o
	$(TOOLBIN)/avr-objdump -h -S $^

arduino.s : arduino.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -S $^

s : arduino.s

#
# make arduino.hex && avrdude -p m168 -c arduino -P /dev/cu.usbserial-A6008hFI -U flash:w:arduino.hex

