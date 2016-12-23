#ifndef PPMOUT_H
#define PPMOUT_H

#include "Arduino.h"

#define NUMBER_OF_PPM_CHANNELS 8

#ifdef __cplusplus
extern "C"{
#endif

// Initializes the PPM output by setting up timer1 and initializing the output pin
void ppmOut_init(int outPin);
// Changes the PPM parameters to non-default settings
void ppmOut_config(uint_fast16_t frameLength, uint_fast16_t pulseLength, uint_fast8_t polarity);
// Begins the PPM output (assumes 16Mhz processor)
void ppmOut_begin();
// Stops the PPM output
void ppmOut_stop();
// Write a value to a specific channel (No bounds checking on channel or value!)
void ppmOut_write(uint_fast8_t channel, uint_fast16_t value);
// Write a value to ALL channels (no bounds checking on value!)
void ppmOut_writeAll(uint_fast16_t value);

#ifdef __cplusplus
}
#endif

#endif /* PPMOUT_H */
