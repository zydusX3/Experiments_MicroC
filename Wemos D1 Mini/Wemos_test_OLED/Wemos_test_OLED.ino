#include <Wire.h>
#include "myWifi.h"
#include "Oled_Disp.h"
#include "myFunctions.h"

bool b = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Wire.begin(D2, D1);
  initWifi();
  // i2cScanner();
  initOLED();
}

void loop() {
  // if(!b){
  //   oledDispString("Peee...");
  // } else {
  //   oledDispString("Pooo...");
  // }
  // delay(800);
  // toggleLed();
  // b=!b;
  wink();  delay(1000);
}
