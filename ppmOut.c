#include "Arduino.h"
#include "ppmOut.h"

/***********************************/
/***        PRIVATE DATA         ***/
/***********************************/
static int           _outPin;                // The output pin for the PPM output, must be a digital pin
static uint_fast16_t _frameLength = 22500;   // The PPM frame width (all channels) in microseconds (1ms = 1000us), defaults to 22500
static uint_fast16_t _pulseLength = 300;     // The pulse length of any individual channel in microseconds, defualts to 300
static uint_fast8_t  _polarity    = 1;       // The polarity of pulses, defaults to HIGH (HIGH or LOW / 0 or 1)
static uint_fast16_t _ppmAR[NUMBER_OF_PPM_CHANNELS];

/*******************************************/
/***         FUNCTION DEFENITIONS        ***/
/*******************************************/

void ppmOut_init(int outPin) {
    // Setup the output pin
    _outPin = outPin;
    pinMode(_outPin, OUTPUT);
    digitalWrite(_outPin, !_polarity);

    // Center all the channels to the default value
    ppmOut_writeAll(1500);

    // Setup Timer1
    cli();
    // Clear out timer 1 config registers
    TCCR1A  = 0;
    TCCR1B  = 0;

    // Setup timer 1 for PPM output and enable interrupts
    OCR1A   = 100;           // compare match register, change this
    TCCR1B |= (1 << WGM12);  // turn on CTC mode (Clear Timer on Compare Match)
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    sei();
}

void ppmOut_config(uint_fast16_t frameLength, uint_fast16_t pulseLength, uint_fast8_t polarity) {
    // set the PPM parameter values
    _frameLength = frameLength;
    _pulseLength = pulseLength;
    _polarity    = polarity;

    // Set output pin to off state in case polarity changed
    digitalWrite(_outPin, !_polarity);
}

void ppmOut_begin() {
    cli();
    TCNT1 = 0;
    TCCR1B |= (1 << CS11);   // 8 prescaler: 0.5 microseconds at 16mhz
    sei();
}

void ppmOut_stop() {
    cli();
    TCCR1B &= (0 << CS11);  // Disable the timer, thereby disabline the PPM output
    sei();
}

void write(uint_fast8_t channel, uint_fast16_t value) {
    _ppmAR[channel-1] = value;
}

void ppmOut_writeAll(uint_fast16_t value) {
    // Set all values in the ppm array to the center value
    for(int i = 0; i < NUMBER_OF_PPM_CHANNELS; i++){
        _ppmAR[i] = value;
    }
}

ISR(TIMER1_COMPA_vect) {
    boolean state = true;

   TCNT1 = 0;

   // START PULSE
   if(state) {
       digitalWrite(_outPin, _polarity);
       OCR1A = _pulseLength * 2;
       state = false;
   }
   //end pulse and calculate when to start the next pulse
   else {
       static byte currentChannel;
       static unsigned int calcRest;

       digitalWrite(_outPin, !_polarity);
       state = true;

       if(currentChannel >= NUMBER_OF_PPM_CHANNELS){
           currentChannel = 0;
           calcRest = calcRest + _pulseLength;
           OCR1A = (_frameLength - calcRest) * 2;
           calcRest = 0;
       }
       else{
           OCR1A = (_ppmAR[currentChannel] - _pulseLength) * 2;
           calcRest = calcRest + _ppmAR[currentChannel];
           currentChannel++;
       }
    }
}
