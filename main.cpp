#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ALL_OUTPUT 0xFF

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
    for(int i=0; i<=5; i++){
        PORTD ^= (1 << i);
        _delay_ms(100);
        PORTD ^= (1 << i);
    }
    for(int i=0; i<=5; i++){
        PORTC ^= (1 << i);
        _delay_ms(100);
        PORTC ^= (1 << i);
    }
    for(int i=0; i<=4; i++){
        PORTB ^= (1 << i);
        _delay_ms(100);
        PORTB ^= (1 << i);
    }
}

void initTime(char* hour, char* minute, char* second){
    *hour = 0;
    *minute = 0;
    *second = 0;
}

void showTime(char* hour, char* minute, char* second){
    PORTB = *hour;
    PORTC = *minute;
    PORTD = *second;
}

void countSeconds(char* second){
    if(*second < 59){
        *second = *second + 1;
    } else {
        *second = 0;
    }
}

int main() {
    DDRB = ALL_OUTPUT;          // set whole PORTB as output
    DDRC = ALL_OUTPUT;          // set whole PORTC as output
    DDRD = ALL_OUTPUT;          // set whole PORTD as output
    char hour;                  // PORTB displays the hours
    char minute;                // PORTC displays the minutes
    char second;                // PORTD display the seconds
    initTime(&hour, &minute, &second);
    showStartAnimation();

    while(1){
        showTime(&hour, &minute, &second);
        countSeconds(&second);
        _delay_ms(1000);
    }
}