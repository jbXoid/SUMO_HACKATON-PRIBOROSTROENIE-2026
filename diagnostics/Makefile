.PHONY: all flash clean

all: build/firmware.hex

build:
	mkdir -p build

build/firmware.elf: main.c timer1.c Timer2.c SHARP_ADC.c USART.c DC_motor_driver.c | build
	avr-gcc -mmcu=atmega168p -Os \
		-ffunction-sections -fdata-sections \
		-Wl,--gc-sections $^ -o $@

build/firmware.hex: build/firmware.elf
	avr-objcopy -O ihex -R .eeprom $< $@

flash: build/firmware.hex
	@echo "Reminder: make sure you rebuilt the project."
	avrdude -c usbasp -p m168p -U flash:w:$<

clean:
	rm -rf build

