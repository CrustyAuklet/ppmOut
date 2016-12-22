#ifndef PPMOUT_H
#define PPMOUT_H

#include "Arduino.h"

#define NUMBER_OF_CHANNELS 8

class PPMout {
public:
    PPMout(int outPin);
    void init();
    void init(uint_fast16_t frameLength, uint_fast16_t pulseLength, uint_fast8_t polarity);

    void setFrameLength(uint_fast16_t frameLength);
    void setPulseLength(uint_fast16_t pulseLength);
    void setPolarity(uint_fast8_t polarity);
    void setDefaultValue(uint_fast16_t defValue);

    void write(uint_fast8_t channel, uint_fast16_t value);
    void centerAllChannels();

private:
    int           _outPin;          // The output pin for the PPM output, must be a digital pin
    uint_fast16_t _frameLength;     // The PPM frame width (all channels) in microseconds (1ms = 1000us), defaults to 22500
    uint_fast16_t _pulseLength;     // The pulse length of any individual channel in microseconds, defualts to 300
    uint_fast8_t  _polarity;        // The polarity of pulses, defaults to HIGH (HIGH or LOW / 0 or 1)
    uint_fast16_t _ppmAR[NUMBER_OF_CHANNELS];

    uint_fast16_t _defValue;        // The default value of each PPM channel, set to 1500 initially
};

#endif /* PPMOUT_H */
