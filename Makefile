.PHONY: all flash clean

all: build/firmware.hex

build:
	mkdir -p build

build/firmware.elf: main.c  DC_motor_driver.c FSM_Go.c FSM_Perimeter.c FSM_Start.c FSM_Tornado.c SHARP_ADC.c Timer2.c USART.c messages.c timer1.c | build
	avr-gcc -mmcu=atmega168p -Os \
		-ffunction-sections -fdata-sections \
		-Wl,--gc-sections $^ -o $@

build/firmware.hex: build/firmware.elf
	avr-objcopy -O ihex -R .eeprom $< $@

flash:
	@echo "Reminder: make sure you rebuilt the project."
	avrdude -c usbasp -p m168p -U flash:w:$<:i

clean:
	rm -rf build

