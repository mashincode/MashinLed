#include "FastLED.h"
#include <IRremote.h>
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
CRGBPalette16 palette = PartyColors_p;

const unsigned long BAUD_RATE = 9600;
IRrecv irrecv(2); // указываем пин, к которому подключен IR приемник
decode_results results;

// The bluetooth tx and rx pins must be supported by software serial.
// Visit https://www.arduino.cc/en/Reference/SoftwareSerial for unsupported pins.
// Bluetooth TX -> Arduino D8


const int BLUETOOTH_TX = 7;
const int BLUETOOTH_RX = 8;


// Bluetooth RX -> Arduino D7


int prevThrottle = 49;
int prevSteering = 49;
int throttle, steering, sliderVal, button, sliderId;

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor


void bluetooth_begin() {
  bluetooth.begin(BAUD_RATE);
  delay(100);
  Serial.println("bluetooth setup complete");
}



////////////////////////////////
int MODE = 1;//////////////////
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

#define NUM_LEDS 100
#define LED_PIN 6
#define BRIGHTNESS 255
#define SOUND_SENSOR_PIN A0 
#define SOUND_SENSOR_LEVEL 10

// Define color structure for rgb
struct color {int r;int g;int b;};
typedef struct color Color;
CRGBArray<NUM_LEDS> leds;

void setup() { 
  Serial.begin(9600);
  irrecv.enableIRIn(); // запускаем прием инфракрасного сигнала
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS); 
  FastLED.setBrightness(BRIGHTNESS); //0-250
  pinMode(SOUND_SENSOR_PIN, INPUT);
  bluetooth_begin();
  //fadeall();
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); }if (irrecv.isIdle()) {FastLED.show();} }

void loop() {
  checkCommand();
   play();
   
}

void switchMode(int value){
  MODE = value;
  Serial.println("Switch mode to ");
  Serial.println(MODE);
  Serial.println();  
}
void checkCommand(){
   
 if (irrecv.decode(&results)){
    if (results.value == 16753245)  {
      switchMode(1);
    }
    else if (results.value == 16736925) {
       switchMode(2);
    } else if (results.value == 16769565) {
       switchMode(3);
    }  else if (results.value == 16720605) {
       switchMode(4);
    }  else if (results.value == 16712445) {
       switchMode(5);
    }  else if (results.value == 16761405) {
       switchMode(6);
    }  else if (results.value ==  16769055) {
       switchMode(7);
    }  else if (results.value == 16754775) {
       switchMode(8);
    }  else if (results.value == 16748655) {
       switchMode(9);
    }  else if (results.value == 16738455) {
       switchMode(10); // * 
    }  else if (results.value ==  16756815) {
       switchMode(-1); //# turn off
    } 
    
    Serial.println(results.value);
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  
  int button = phone.getButton();
  String str = phone.getText();

  if (button != -1){

    MODE = button; 
    Serial.println("Switch mode to ");
    Serial.println(MODE);
    Serial.println();

   }
   
  else if (str != "") {
        MODE = str.toInt(); 
        Serial.println(str);
  }
  else{
    
    if (Serial.available()){
      int serial_value = Serial.parseInt();
      if (serial_value != 0){
        MODE = serial_value;
        Serial.println("Switch mode to ");
        Serial.println(MODE);
        Serial.println();
        fadeall();
      }
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
       colorMusic();
      break;
   case 11:
       fadeall();
      break;
   default:
      fadeall();
  }
   
}
