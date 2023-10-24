#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define INPUT 0
#define OUTPUT 1
#define HIGH 1

bool configMode = false;

class Pin {
    private:
        volatile uint8_t* _port;
        volatile uint8_t* _dport;
        uint8_t _portNumber;
        bool _isHigh;
        bool _isInput;
        bool _pullUpEnabled;
    public:
        Pin(volatile uint8_t* port, volatile uint8_t* dport, uint8_t portNumber, uint8_t mode){
            _port = port;
            _dport = dport;
            _portNumber = portNumber;
            if(mode == INPUT){
                _isInput = true;
                *_dport &= ~(1<<_portNumber);
            } else if (mode == OUTPUT){
                _isInput = false;
                *_dport |= (1<<_portNumber);
            }
            _isHigh = false;
        }
        void setHigh() {
            _isHigh = true;
            *_port |= (1<<_portNumber);
        }
        void setLow() {
            _isHigh = false;
            *_port &= ~(1<<_portNumber);
        }
        void enablePullUp(){
            if(_isInput){
                *_port |= (1<<_portNumber);
            }
        }
        void disablePullUp(){
            if(_isInput){
                *_port &= ~(1<<_portNumber);
            }
        }
        volatile uint8_t* getPort(){
            return _port;
        }
        uint8_t getPortNumber(){
            return _portNumber;
        }
};

class SPI {        
    public:
        SPI(){
            /* SPI Master Init */
            // Enable SPI, Master, set clock rate fck/16
            SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
            DDRB |= (1 << PORT2) | (1 << PORT3) | (1 << PORT5);
        }
        void sendByte(uint8_t* byte){
            SPDR = *byte;               // start transmission
            PORTB |= (1 << PORT2); 
            while(!(SPSR & (1<<SPIF))); // wait for transmission complete
            PORTB &= ~(1 << PORT2);
        }
};

class SoftSPI {
    // SPI without SPI Hardware, fully imlplemented in Software
    // CPU has more to do than in Hardware SPI
    private:
        Pin* _ss;
    public:
        SoftSPI(Pin* ss){
            _ss = ss;
            // PORTD3 MOSI
            // PORTD5 SCK
            DDRB |= (1 << PORT2) | (1 << PORT3) | (1 << PORT5);
            // PORTB |= (1<<2);                // LATCH HIGH
            _ss->setHigh();                 // Slave Select HIGH
        }
        void sendByte(uint8_t* byte){
            // PORTB &= ~(1<<2);               // LATCH LOW
            _ss->setLow();                  // Slave Select LOW
            for(int i=0; i<8; i++){
                if(*byte & (HIGH << i)){    // if Bit at Position i is 1    
                    PORTB |= (1<<3);        // DATA HIGH
                } else {
                    PORTB &= ~(1<<3);       // DATA LOW
                }

                PORTB |= (1<<5);            // CLOCK HIGH
                // no need to wait, a clock cycle is enough wait time
                PORTB &= ~(1<<5);           // CLOCK LOW
            }
            // PORTB |= (1<<2);                // LATCH HIGH
            _ss->setHigh();                 // Slave Select HIGH
        }
        void selectSlave(Pin* ss){
            _ss = ss;
        }
};

class Clock {
    private:
        SoftSPI* _spi;
        Pin* _secondSS;
        Pin* _minuteSS;
        Pin* _hourSS;

        uint8_t _hour;
        uint8_t _minute;
        uint8_t _second;
        
        void _countSeconds(){
            if(_second < 59){
                _second += 1;
            } else {
                _second = 0;
            }
        }
        void _countMinutes(){
            if(_second == 0){
                if(_minute < 59){
                    _minute += 1;
                } else {
                    _minute = 0;
                }
            }
        }
        void _countHours(){
            if(_minute == 0 && _second == 0){
                if(_hour < 23){
                    _hour += 1;
                } else {
                    _hour = 0;
                }
            }
        }   
    public:
        Clock(SoftSPI* spi, Pin* secondSS, Pin* minuteSS, Pin* hourSS){
            _spi = spi;
            _secondSS = secondSS;
            _minuteSS = minuteSS;
            _hourSS = hourSS;

            _hour = 0;
            _minute = 0;
            _second = 0;
        }
        void tick(){
            _countSeconds();
            _countMinutes();
            _countHours();
        }
        void show(){
            _spi->selectSlave(_secondSS);
            _spi->sendByte(&_second);

            _spi->selectSlave(_minuteSS);
            _spi->sendByte(&_minute);

            _spi->selectSlave(_hourSS);
            _spi->sendByte(&_hour);
        }
};

// void showStartAnimation(){
//     turnOnAll();
//     _delay_ms(200);
//     turnOffAll();
//     _delay_ms(200);
//     turnOnAll();
//     _delay_ms(200);
//     turnOffAll();
//     for(uint8_t i=0; i<=5; i++){
//         PORTB ^= (1 << i);
//         _delay_ms(100);
//         PORTB ^= (1 << i);
//     }
//     for(uint8_t i=0; i<=5; i++){
//         PORTC ^= (1 << i);
//         _delay_ms(100);
//         PORTC ^= (1 << i);
//     }
//     for(uint8_t i=0; i<=4; i++){
//         PORTD ^= (1 << i+3);        // +3 because LED0 is not at PD0, but at PD3
//         _delay_ms(100);
//         PORTD ^= (1 << i+3);
//     }
    
// }

void initINT0() {
    EICRA = 3;                  // rising edge of INT0 generates an interrupt request
    EIMSK = 1;                  // INT0: External Interrupt Request 0 Enable
    SREG = 128;                 // enable interrupts globally
    _delay_ms(200);
}

ISR(INT0_vect){
    //turnOffAll();
    configMode = !configMode;
}

int main() {
    Pin secondSS(&PORTB, &DDRB, 2, OUTPUT);
    secondSS.setHigh();
    Pin minuteSS(&PORTB, &DDRB, 1, OUTPUT);
    minuteSS.setHigh();
    Pin hourSS(&PORTB, &DDRB, 0, OUTPUT);
    hourSS.setHigh();

    SoftSPI spi(&secondSS);

    Clock clock(&spi, &secondSS, &minuteSS, &hourSS);

    while(1){
        clock.show();
        clock.tick();
        _delay_ms(1000);
    }
}