# Binary Clock

A binary Clock that can used as a normal watch. The "INT0BTN"-Button can be used to stop the clock and change into config mode.
In config mode you can select between "hour", "minute" and "second" - Leds. During a selection you can count up to setup the clock.

## avr-libc

https://www.nongnu.org/avr-libc/user-manual/modules.html

## compile

    avr-gcc main.cpp -o main.elf -mmcu=atmega328 -DF_CPU=16000000UL -Os
    avr-objcopy main.elf -O ihex main.hex

## upload

    avrdude -c arduino -p m328p -U flash:w:"main.hex":a -P /dev/ttyACM0

## Helpful Links

    https://www.rapidtables.com/calc/math/binary-calculator.html

    https://www.rapidtables.com/convert/number/binary-to-hex.html

## Pinout

    ARDUINO UNO             SHIFT REGISTERS    
    GND                     Pin  8 GND - ALL
    5V                      Pin 16 Vcc - ALL
                            Pin 13 OE - ALL -> GND
    Pin 8                   Pin 12 SS - Hours -> R10k -> GND
    Pin 9                   Pin 12 SS - Minutes -> R10k -> GND 
    Pin 10                  Pin 12 SS - Seconds -> R10k -> GND 
    Pin 11                  Pin 14 MOSI - ALL -> R10k -> GND 
    Pin 13                  Pin 11 CLK - ALL -> R10k -> GND 
                            Pin 10 MR -> 5V
                            Pin 7 Output Bit 7 -> LED
                            Pin 6 Output Bit 6 -> LED
                            Pin 5 Output Bit 5 -> LED
                            Pin 4 Output Bit 4 -> LED
                            Pin 3 Output Bit 3 -> LED
                            Pin 2 Output Bit 2 -> LED


                            INT0BTN
    3.3V                    Pin 1
    Pin 2                   Pin 2                    
    
                            
                            COUNTBTN
    Pin 6                   Pin 1
                            Pin 2 -> GND


                            SELECTBTN
    Pin 7                   Pin 1
                            Pin 2 -> GND


    DESCRIPTION
    R10K - 10 kOhm PullDown Resistor
