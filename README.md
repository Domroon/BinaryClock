# Binary Clock

## avr-libc

https://www.nongnu.org/avr-libc/user-manual/modules.html

## compile

    avr-gcc blink.cpp -o blink.elf -mmcu=atmega328 -DF_CPU=16000000UL -Os
    avr-objcopy blink.elf -O ihex blink.hex

## upload

    avrdude -c arduino -p m328p -U flash:w:"blink.hex":a -P /dev/ttyACM0
