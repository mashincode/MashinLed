// have 3 independent CRGBs - 2 for the sources and one for the output
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];


void animationA() {                                             // running red stripe.

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t red = (millis() / 10) + (i * 12);                    // speed, length
    if (red > 128) red = 0;
    leds2[i] = CRGB(red, 0, 0);
  }
} // animationA()



void animationB() {                                               // running green stripe in opposite direction.
  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t green = (millis() / 5) - (i * 12);                    // speed, length
    if (green > 128) green = 0;
    leds3[i] = CRGB(0, green, 0);
  }
} // animationB()

void animations() {

  animationA();                                               // render the first animation into leds2   
  animationB();                                               // render the second animation into leds3

  uint8_t ratio = beatsin8(2);                                // Alternate between 0 and 255 every minute
  
  for (int i = 0; i < NUM_LEDS; i++) {                        // mix the 2 arrays together
    leds[i] = blend( leds2[i], leds3[i], ratio );
  }

  if (irrecv.isIdle()) {
    FastLED.show(); 
  }
}
