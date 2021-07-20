#include "EEPROM.h"                                          // Removed because it's different between platforms. Was developed for UNO/Nano.

int micIn;                                                    // Raw microphone input is up to 12 bit A/D.


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))            // Definition for the array of routines to display.

uint8_t gCurrentPatternNumber = 1;                            // Index number of which pattern is current.
uint8_t gHue = 0;                                             // Rotating "base color" used by many of the patterns.

                                   
typedef void (*SimplePatternList[])();                        // Define a type that's a pointer to an array.
SimplePatternList gPatterns = {rainbow};     // Here's an array of patterns.

void rainbow() {
//   fill_rainbow( leds, NUM_LEDS, gHue, 7);                  // Original routine.
  fadeToBlackBy(leds, NUM_LEDS,32);
  int tmp = (random8()+micIn) % NUM_LEDS;
  leds[tmp] += CHSV(gHue+tmp*7,255,micIn*4);
} // rainbow()

void analog_sample() {

  static float micLev;                        // Zeroing variable needs to be a float. Otherwise an integer smoothing calculation would be very inaccurate.
  static float micSm;                         // Used for exponential smoothing. Also needs to be a float.
  const float smWeight = 0.1;                 // Exponential smoothing, between 0 and 1.0. Lower value = smoother.
  
  micIn = (analogRead(SOUND_SENSOR_PIN) * SOUND_SENSOR_LEVEL);
  #ifdef ESP32                                // Convert 12 bit A/D to 10 bit A/D for ESP32.
    micIn = micIn >> 2;
  #endif
  micLev = ((micLev*31)+micIn)/32;            // Quickie exponential smoothing to get average value, which is used for zeroeing.
  micIn -= micLev;                            // Zero out micIn, so that we don't have to define an offset for different analog microphones.
  micIn = abs(micIn);
  micSm = (smWeight * (float)micIn + (1.0-smWeight) * micSm); // Longer version of exponential smoothing. Does the same thing.
  micIn = (uint8_t)micSm;                     // We have now zeroed out and smoothed the microphone input a bit. Can't do FFT with this.
  micIn = (micIn <= SOUND_SENSOR_LEVEL) ? 0: micIn;      // Get rid of background noise.
}


void colorMusic() {
  analog_sample();
  gCurrentPatternNumber = EEPROM.read(0);

  EVERY_N_MILLIS(1000/60) {        
    gPatterns[gCurrentPatternNumber]();
    gHue++;
    if (irrecv.isIdle()) {
      FastLED.show(); 
    }
  }
} 
