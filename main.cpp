#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ALL_OUTPUT 0xFF

bool configMode = false;

enum Selected {
    HOUR,
    MINUTE,
    SECOND
};

void turnOnAll(){
    PORTB = 0xFF;
    PORTC = 0xFF;
    PORTD = 0xFF;
}

void turnOffAll(){
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
}

void showStartAnimation(){
    turnOnAll();
    _delay_ms(200);
    turnOffAll();
    _delay_ms(200);
    turnOnAll();
    _delay_ms(200);
    turnOffAll();
    for(uint8_t i=0; i<=5; i++){
        PORTB ^= (1 << i);
        _delay_ms(100);
        PORTB ^= (1 << i);
    }
    for(uint8_t i=0; i<=5; i++){
        PORTC ^= (1 << i);
        _delay_ms(100);
        PORTC ^= (1 << i);
    }
    for(uint8_t i=0; i<=4; i++){
        PORTD ^= (1 << i+3);        // +3 because LED0 is not at PD0, but at PD3
        _delay_ms(100);
        PORTD ^= (1 << i+3);
    }
    
}

void showTime(uint8_t* hour, uint8_t* minute, uint8_t* second){
    PORTB = *second;
    PORTC = *minute;
    PORTD = (*hour << 3);
}

void countSeconds(uint8_t* second){
    if(*second < 59){
        *second = *second + 1;
    } else {
        *second = 0;
    }
}

void countMinutes(uint8_t* minute, uint8_t* second){
    if(*second == 0){
        if(*minute < 59){
            *minute = *minute + 1;
        } else {
            *minute = 0;
        }
    }
}

void countHours(uint8_t* hour, uint8_t* minute, uint8_t* second){
    if(*minute == 0 && *second == 0){
        if(*hour < 23){
            *hour = *hour + 1;
        } else {
            *hour = 0;
        }
    }
}

void initOutputs(){
    DDRB = ALL_OUTPUT;          // set whole PORTB as output
    DDRC = ALL_OUTPUT;          // set whole PORTC as output
    DDRD = ALL_OUTPUT;          // set whole PORTD as output
}

void initINT0() {
    EICRA = 3;                  // rising edge of INT0 generates an interrupt request
    EIMSK = 1;                  // INT0: External Interrupt Request 0 Enable
    SREG = 128;                 // enable interrupts globally
    _delay_ms(200);
}

void initInputPins() {
    DDRD &= ~(1<<0);            // set PD0 as Input
    DDRD &= ~(1<<1);            // set PD1 as Input
}

ISR(INT0_vect){
    //turnOffAll();
    configMode = !configMode;
}

int main() {
    uint8_t hour = 0;          // PORTD displays the hours
    uint8_t minute = 0;         // PORTC displays the minutes
    uint8_t second = 0;         // PORTB display the seconds
    
    enum Selected selected = HOUR;

    initOutputs();
    initINT0();
    initInputPins();
    showStartAnimation();

    while(1) {
        while(configMode){
            turnOffAll();
            switch(selected){
                case HOUR:
                    break;
                case MINUTE:
                    break;
                case SECOND:
                    break;
            }
        }

        while(!configMode){
            showTime(&hour, &minute, &second);
            countSeconds(&second);
            countMinutes(&minute, &second);
            countHours(&hour, &minute, &second);
            _delay_ms(1000);
        }
    }
}