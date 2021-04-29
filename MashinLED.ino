#include "FastLED.h"

#define NUM_LEDS 150
#define LED_PIN 7
#define BRIGHTNESS 255
#define SOUND_SENSOR_PIN A0 

////////////////////////////////
int MODE = 7;//////////////////
//1 - pride //////////////////
//2 - cylon /////////////////
//3 - center ///////////////
//4 - twinkleFox //////////
//5 - pacifica ///////////
//6 - palleteTemperature();
//7 - palletteFire();

//////COLOR MUSIC //////

//8 - Last ////////////
//9 - April //////////
//10 - Octave ///////


// Define color structure for rgb
struct color {
  int r;
  int g;
  int b;
};

typedef struct color Color;

CRGBArray<NUM_LEDS> leds;

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS); 
  FastLED.setBrightness(BRIGHTNESS); //0-250
  pinMode(SOUND_SENSOR_PIN, INPUT);
  //fadeall();
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } FastLED.show(); }

void loop() {
  checkCommand();
  play();
}
void checkCommand(){
  if (Serial.available()){
    int serial_value = Serial.parseInt();
    if (serial_value != 0){
      MODE = serial_value;
      Serial.print("Switch mode to ");
      Serial.print(MODE);
      Serial.println();
      fadeall();
    }
   }
}

void play(){  
  switch (MODE) {
   case 1:
      pride();
      break;
   case 2:
      cylon();
      break;
   case 3:
      center();
      break;
   case 4:
       twinkleFox();
       break;
   case 5:
       pacifica();
       break;
   case 6:
       palleteTemperature();
       break;
   case 7:
       palletteFire();
       
       break;
   case 8:
      last();
      break;
   case 9:
      april();
      break;
   case 10:
       octave();
      break;
   default:
      fadeall();
  }
}
