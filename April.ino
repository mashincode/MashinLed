#define UPDATES_PER_SECOND 100

// STANDARD VISUALIZER VARIABLES
int loop_max = 0;
int k = 255; // COLOR WHEEL POSITION
int decay = 0; // HOW MANY MS BEFORE ONE LIGHT DECAY
int decay_check = 0;
long pre_react = 0; // NEW SPIKE CONVERSION
long react = 0; // NUMBER OF LEDs BEING LIT
long post_react = 0; // OLD SPIKE CONVERSION

// rainbow WAVE SETTINGS
int wheel_speed = 3;

// FUNCTION TO GENERATE COLOR BASED ON VIRTUAL WHEEL
// https://github.com/NeverPlayLegit/rainbow_mashin-Fader-FastLED/blob/master/rainbow_mashin.ino
CRGB Scroll(int pos) {
  CRGB color (0,0,0);
  if(pos < 85) {
    color.g = 0;
    color.r = ((float)pos / 85.0f) * 255.0f;
    color.b = 255 - color.r;
  } else if(pos < 170) {
    color.g = ((float)(pos - 85) / 85.0f) * 255.0f;
    color.r = 255 - color.g;
    color.b = 0;
  } else if(pos < 256) {
    color.b = ((float)(pos - 170) / 85.0f) * 255.0f;
    color.g = 255 - color.b;
    color.r = 1;
  }
  return color;
}


// FUNCTION TO GET AND SET COLOR
// THE ORIGINAL FUNCTION WENT BACKWARDS
// THE MODIFIED FUNCTION SENDS WAVES OUT FROM FIRST LED
// https://github.com/NeverPlayLegit/rainbow_mashin-Fader-FastLED/blob/master/rainbow_mashin.ino
void rainbow_mashin()
{
  for(int i = 0; i <= NUM_LEDS - 1; i++) {
    if (i < react)
      //leds[i] = Scroll((i * 256 / 50 + k) % 256);
      leds[(NUM_LEDS-1)-i] = CRGB(100, 0, 100);
    else
      leds[(NUM_LEDS-1)- i] = CRGB(0, 0, 100);      
  }
    if (irrecv.isIdle()) {
      FastLED.show(); 
    }
}



void april(){
  int SOUND_SENSOR_PIN_input = analogRead(SOUND_SENSOR_PIN) * SOUND_SENSOR_LEVEL; // ADD x2 HERE FOR MORE SENSITIVITY  

  if (SOUND_SENSOR_PIN_input > 0)
  {
    pre_react = ((long)NUM_LEDS * (long)SOUND_SENSOR_PIN_input) / 1023L; // TRANSLATE SOUND_SENSOR_PIN LEVEL TO NUMBER OF LEDs

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react = pre_react;
  }

  rainbow_mashin(); // APPLY COLOR

  k = k - wheel_speed; // SPEED OF COLOR WHEEL
  if (k < 0) // RESET COLOR WHEEL
    k = 255;

  // REMOVE LEDs
  decay_check++;
  if (decay_check > decay)
  {
    decay_check = 0;
    if (react > 0)
      react--;
  }
  //delay(1);
}
