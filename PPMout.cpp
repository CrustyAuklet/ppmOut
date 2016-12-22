#include "PPMout.h"

PPMout::PPMout(int outPin) {
    _outPin = outPin;
    _defValue = 1500;
}

void PPMout::init() {
    // Initialize with the default values
    init(22500, 300, 1);
}

void PPMout::init(uint_fast16_t frameLength, uint_fast16_t pulseLength, uint_fast8_t polarity) {
    // set the PPM parameter values
    _frameLength = frameLength;
    _pulseLength = pulseLength;
    _polarity    = polarity;

    // Center all the channels to the default value
    centerAllChannels();

    // Set output pin to off state
    pinMode(_outPin, OUTPUT);
    digitalWrite(_outPin, !_polarity);

    cli();
    TCCR1A  = 0;             // set entire TCCR1 register to 0
    TCCR1B  = 0;

    OCR1A   = 100;           // compare match register, change this
    TCCR1B |= (1 << WGM12);  // turn on CTC mode
    TCCR1B |= (1 << CS11);   // 8 prescaler: 0,5 microseconds at 16mhz
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    sei();
}

void PPMout::setFrameLength(uint_fast16_t frameLength) {
    _frameLength = frameLength;
}

void PPMout::setPulseLength(uint_fast16_t pulseLength) {
    _pulseLength = pulseLength;
}

void PPMout::setPolarity(uint_fast8_t polarity) {
    _polarity = polarity;
}

void PPMout::setDefaultValue(uint_fast16_t defValue) {
    _defValue = defValue;
}

void PPMout::write(uint_fast8_t channel, uint_fast16_t value) {
    _ppmAR[channel-1] = value;
}

void PPMout::centerAllChannels() {
    // Set all values in the ppm array to the center value
    for(int i = 0; i < NUMBER_OF_CHANNELS; i++){
        _ppmAR[i] = _defValue;
    }
}
