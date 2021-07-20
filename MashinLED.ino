#include "FastLED.h"
#include <IRremote.h> //2.7
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>



const int BLUETOOTH_TX = 7;
const int BLUETOOTH_RX = 8;
IRrecv irrecv(2);//9); // указываем пин, к которому подключен IR приемник
#define NUM_LEDS 200 //422 // количество диодов
#define LED_PIN 6//30 // pin для ленты
#define BRIGHTNESS 255 // яркость ленты (0-255)
#define SOUND_SENSOR_PIN A0 // аналоговый датчик звука
#define SOUND_SENSOR_LEVEL 10 // уровень звука


//const int BLUETOOTH_TX = 7;
//const int BLUETOOTH_RX = 8;
//IRrecv irrecv(9); //9); // указываем пин, к которому подключен IR приемник
//#define NUM_LEDS 422 //420 // количество диодов
//#define LED_PIN 30//30 // pin для ленты
//#define BRIGHTNESS 255 // яркость ленты (0-255)
//#define SOUND_SENSOR_PIN A0 // аналоговый датчик звука
//#define SOUND_SENSOR_LEVEL 10 // уровень звука


////////////////////////////// Режимы
int MODE = 8;//////////////////
//1 - animations //////////////////
//2 - blendwave /////////////////
//3 - dot_beat ///////////////
//4 - twinkleFox //////////
//5 - pacifica ///////////
//6 - confetti;
//7 - palletteFire();
//////COLOR MUSIC //////
//8 - Last ////////////
//9 - April //////////
//10 -  colorMusic///////


CRGBPalette16 palette = PartyColors_p;
const unsigned long BAUD_RATE = 9600;
decode_results results;
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

void testColors() { for(int i = 0; i < NUM_LEDS; i++) { leds[i]=CRGB(255, 0, 0); }if (irrecv.isIdle()) {FastLED.show();} }

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
    } else {
      Serial.println(results.value);  
    };
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
      animations();
      break;
   case 2:
      blendwave();
      break;
   case 3:
      dot_beat();
      break;
   case 4:
       twinkleFox();
       break;
   case 5:
       pacifica();
       break;
   case 6:
       confetti();
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
