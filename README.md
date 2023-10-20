# Binary Clock

## avr-libc

https://www.nongnu.org/avr-libc/user-manual/modules.html

## compile

    avr-gcc main.cpp -o main.elf -mmcu=atmega328 -DF_CPU=16000000UL -Os
    avr-objcopy main.elf -O ihex main.hex

## upload

    avrdude -c arduino -p m328p -U flash:w:"main.hex":a -P /dev/ttyACM0

## Helpfull Links

    https://www.rapidtables.com/calc/math/binary-calculator.html

    https://www.rapidtables.com/convert/number/binary-to-hex.html
    