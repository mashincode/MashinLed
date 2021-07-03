#include "EEPROM.h"                                          // Removed because it's different between platforms. Was developed for UNO/Nano.

int micIn;                                                    // Raw microphone input is up to 12 bit A/D.


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))            // Definition for the array of routines to display.

uint8_t gCurrentPatternNumber = 1;                            // Index number of which pattern is current.
uint8_t gHue = 0;                                             // Rotating "base color" used by many of the patterns.

                                   
typedef void (*SimplePatternList[])();                        // Define a type that's a pointer to an array.
SimplePatternList gPatterns = {rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };     // Here's an array of patterns.



void addGlitter(uint16_t chanceOfGlitter) {                   // Changed fract8 to uint16_t.
  if(random8() < chanceOfGlitter) leds[ random16(NUM_LEDS) ] += CRGB::White;
} // addGlitter()


void rainbow() {
//   fill_rainbow( leds, NUM_LEDS, gHue, 7);                  // Original routine.
  fadeToBlackBy(leds, NUM_LEDS,32);
  int tmp = (random8()+micIn) % NUM_LEDS;
  leds[tmp] += CHSV(gHue+tmp*7,255,micIn*4);
} // rainbow()


void rainbowWithGlitter() {
  rainbow();                                                  // Built-in FastLED rainbow, plus some random sparkly glitter.
//  addGlitter(80);                                           // Original chance of glitter.
  addGlitter(micIn*8);                                        // Increase our chance of glitter.
} // rainbowWithGlitter()


void confetti() {                                             // Random colored speckles that blink in and fade smoothly.
  fadeToBlackBy(leds, NUM_LEDS, 64);
//  leds[pos] += CHSV( gHue + random8(64), 200, 255);         // Original routine.
  for (int i=0; i<8; i++) leds[random16(NUM_LEDS)] += CHSV(gHue + random8(64), 200, micIn*6);  // Updated routine with slower fps needs more action.
} // confetti()


void sinelon() {                                              // A colored dot sweeping back and forth, with fading trails.
  fadeToBlackBy(leds, NUM_LEDS, 20);
//  int pos = beatsin16(17,0,NUM_LEDS-1);                     // Original routine.
//  leds[pos] += CHSV( gHue, 255, 192);                       // Original routine.
  for (int i=0; i<7; i++) {
    int pos = beatsin16((i+1)*4,0,NUM_LEDS-1);
    leds[pos] += CHSV(gHue, 255, micIn*6);
  }
} // sinelon()


void juggle() {                                               // Eight colored dots, weaving in and out of sync with each other.
  fadeToBlackBy(leds, NUM_LEDS, 20);
//  byte dothue = 0;                                          // Original routine.
  byte dothue = micIn;
  for(int i = 0; i < 8; i++) {
//    leds[beatsin16(i+7,0,NUM_LEDS-1)] |= CHSV(dothue, 200, 255);  // Original routine.
    leds[beatsin16(i+7,0,NUM_LEDS-1)] |= CHSV(dothue, 200, micIn*4);
    dothue += 32;
  }
} // juggle()


void bpm() {                                                  // Colored stripes pulsing at a defined Beats-Per-Minute.
  fadeToBlackBy(leds, NUM_LEDS, 20);
  uint8_t beat = beatsin8(17, 64, 255);
  for(int i = 0; i < NUM_LEDS; i++) leds[i] = ColorFromPalette(palette,gHue+(i*3), beat-gHue+(i*10) );
  if (micIn > 0) leds[random16() % NUM_LEDS] = CRGB::White;
} // bpm()



void analog_sample() {

  static float micLev;                        // Zeroing variable needs to be a float. Otherwise an integer smoothing calculation would be very inaccurate.
  static float micSm;                         // Used for exponential smoothing. Also needs to be a float.
  const float smWeight = 0.1;                 // Exponential smoothing, between 0 and 1.0. Lower value = smoother.
  
  micIn = analogRead(SOUND_SENSOR_PIN);
  #ifdef ESP32                                // Convert 12 bit A/D to 10 bit A/D for ESP32.
    micIn = micIn >> 2;
  #endif
  micLev = ((micLev*31)+micIn)/32;            // Quickie exponential smoothing to get average value, which is used for zeroeing.
  micIn -= micLev;                            // Zero out micIn, so that we don't have to define an offset for different analog microphones.
  micIn = abs(micIn);
  micSm = (smWeight * (float)micIn + (1.0-smWeight) * micSm); // Longer version of exponential smoothing. Does the same thing.
  micIn = (uint8_t)micSm;                     // We have now zeroed out and smoothed the microphone input a bit. Can't do FFT with this.
  micIn = (micIn <= SOUND_SENSOR_LEVEL) ? 0: micIn;      // Get rid of background noise.

  Serial.println(micIn);
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





