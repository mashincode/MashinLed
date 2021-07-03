const int brightnessPot = A1; //pin for the brightness pot
int volumeLevel; //iniger for volume level
#define updateLEDS 5 //number of LEDs to update

void resetLedState() { //function to turn off all the LEDs
  for (int turnMeOff = 0; turnMeOff < 61; turnMeOff++) {
    leds[turnMeOff] = CRGB(0, 0, 0);
  }
   if (irrecv.isIdle()) {
      FastLED.show(); 
    }
}

void audioRead() { //function to read the audio  
  int rawValue = analogRead(SOUND_SENSOR_PIN) * (SOUND_SENSOR_LEVEL-2);
  volumeLevel = map(rawValue, 0, 1023, 0, 900); //map it to 900 values
  if (volumeLevel > 900) { //if it's higher than 900 it lowers it back to 900
    volumeLevel = 900;
  }
}


void setLedBrightness() { //function to set the brigthess set on the pot
  LEDS.setBrightness(255);
}

/*
 * a lot of IF statements to set the right color.
 * colors go from red to blue, then green and orange at the end(red is already used), at 0 volume leds are off/black, at 900/max they're white
 */
void displayFilter(int volume, int lucka) {
  if (volume < 1) {
    leds[lucka] = CRGB(0, 0, 0); //0, black
  }
  if (volume > 1 && volume < 51) {
    leds[lucka] = CRGB(255, 0, 0); //1-50
  }
  if (volume > 50 && volume < 101) {
    leds[lucka] = CRGB(255, 0, 85); //51-100
  }
  if (volume > 100 && volume < 151) {
    leds[lucka] = CRGB(255, 0, 170); //101-150
  }
  if (volume > 150 && volume < 201) {
    leds[lucka] = CRGB(255, 0, 255); //151-200
  }
  if (volume > 200 && volume < 251) {
    leds[lucka] = CRGB(170, 0, 255); //201-250
  }
  if (volume > 250 && volume < 301) {
    leds[lucka] = CRGB(85, 0, 255); //251-300
  }
  if (volume > 300 && volume < 351) {
    leds[lucka] = CRGB(0, 0, 255); //301-350
  }
  if (volume > 350 && volume < 401) {
    leds[lucka] = CRGB(0, 85, 255); //351-400
  }
  if (volume > 400 && volume < 451) {
    leds[lucka] = CRGB(0, 170, 255); //401-450
  }
  if (volume > 450 && volume < 501) {
    leds[lucka] = CRGB(0, 255, 255); //451-500
  }
  if (volume > 500 && volume < 551) {
    leds[lucka] = CRGB(0, 255, 170); //501-550
  }
  if (volume > 550 && volume < 601) {
    leds[lucka] = CRGB(0, 255, 85); //551-600
  }
  if (volume > 600 && volume < 651) {
    leds[lucka] = CRGB(0, 255, 0); //601-650
  }
  if (volume > 650 && volume < 701) {
    leds[lucka] = CRGB(85, 255, 0); //651-700
  }
  if (volume > 700 && volume < 751) {
    leds[lucka] = CRGB(170, 255, 0); //701-750
  }
  if (volume > 750 && volume < 801) {
    leds[lucka] = CRGB(255, 255, 0); //751-800
  }
  if (volume > 800 && volume < 851) {
    leds[lucka] = CRGB(255, 170, 0); //801-850
  }
  if (volume > 850 && volume < 900) {
    leds[lucka] = CRGB(255, 85, 0); //851-899
  }
  if (volume > 899) {
    leds[lucka] = CRGB(255, 255, 255); //900, white
  }
}


void octave() {
  setLedBrightness(); //set the new brightness, you can comment this line at the beginning to disable it
  
  audioRead(); //reap the audio level and map it


  //for loop to move all the LEDs down the strip 
  for (int i = NUM_LEDS - 1; i >= updateLEDS; i--) {
    leds[i] = leds[i - updateLEDS];
  }
  
  //for loop to set the first LED corresponding to the audio level
  for (int i = 0; i < updateLEDS; i++) {
    displayFilter(volumeLevel, i); //set the right colour magic
  }

   if (irrecv.isIdle()) {
      FastLED.show(); 
    }

  //delay(20); //delay
}
