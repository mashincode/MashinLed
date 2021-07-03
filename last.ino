

int max_bright = 255;

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND


unsigned int sample = 0;



void soundmems() {                                       // Rolling average counter - means we don't have to go through an array each time.
  
  int tmp = analogRead(SOUND_SENSOR_PIN) - 512;
  sample = abs(tmp);
  
}  // soundmems()


void sndwave() {
  
  leds[NUM_LEDS/2] = ColorFromPalette(currentPalette, sample, sample*2, currentBlending); 
  
  for (int i = NUM_LEDS - 1; i > NUM_LEDS/2; i--) {       //move to the left
    leds[i] = leds[i - 1];
  }

  for (int i = 0; i < NUM_LEDS/2; i++) {                  // move to the right
    leds[i] = leds[i + 1];
  }
} // sndwave()




void last() {
  
  currentPalette = PartyColors_p;
  currentBlending = LINEARBLEND; 

  EVERY_N_MILLISECONDS(100) {                                 // AWESOME palette blending capability.
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }


  EVERY_N_SECONDS(5) {                                        // Change the palette every 5 seconds.
    for (int i = 0; i < 16; i++) {
      targetPalette[i] = CHSV(random8(), 255, 255);
    }
  }

  EVERY_N_MILLISECONDS(20) {
    fadeToBlackBy(leds, NUM_LEDS, 4);                            // 8 bit, 1 = slow, 255 = fast    
    sndwave();
    soundmems();
  }

   if (irrecv.isIdle()) {
      FastLED.show(); 
    }

} // loop()
