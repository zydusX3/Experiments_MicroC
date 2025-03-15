#include "myFunctions.h"
#include <Arduino.h>

bool OLED_state = true;
bool GPS_state = true;
bool BLE_state = true;
bool buttonState_OLED = LOW;             // Current state of the button
bool buttonState_GPS = LOW;
bool buttonState_BLE = LOW;
bool lastButtonState_OLED = LOW;         // Previous state of the button
bool lastButtonState_GPS = LOW;
bool lastButtonState_BLE = LOW;
unsigned long lastDebounceTime_OLED = 0; // The last time the button was toggled
unsigned long lastDebounceTime_GPS = 0;
unsigned long lastDebounceTime_BLE = 0;
unsigned long debounceDelay = 50;       // Debounce delay in milliseconds

void my_init() {
  Serial.begin(115200);
  // pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON_OLED, INPUT_PULLUP);
  pinMode(BUTTON_GPS, INPUT_PULLUP);
  pinMode(BUTTON_BLE, INPUT_PULLUP);
}

void serialDBG(bool enable) {
  dbg_serial = enable;
  if(dbg_serial) Serial.println("!!! ENABLE SERIAL DEBUG !!!");
  if(!dbg_serial) Serial.println("!!! DISABLE SERIAL DEBUG !!!");
}

void onButtonPress() {
  // Button Press execution here
  myGPS = LiveGPS;
}

void CheckBtnState() {
  bool readingA = digitalRead(BUTTON_OLED);
  if (readingA != lastButtonState_OLED) {
    lastDebounceTime_OLED = millis();
  }
  if ((millis() - lastDebounceTime_OLED) > debounceDelay) {
    if (readingA != buttonState_OLED) {
      buttonState_OLED = readingA;
        OLED_state = !OLED_state;
        Serial.println("OLED state: "+String(OLED_state));
    }
  }
  lastButtonState_OLED = readingA;
  bool readingB = digitalRead(BUTTON_GPS);
  if (readingB != lastButtonState_GPS) {
    lastDebounceTime_GPS = millis();
  }
  if ((millis() - lastDebounceTime_GPS) > debounceDelay) {
    if (readingB != buttonState_GPS) {
      buttonState_GPS = readingB;
        GPS_state = !GPS_state;
        Serial.println("GPS state: "+String(GPS_state));
    }
  }
  lastButtonState_GPS = readingB;
  bool readingC = digitalRead(BUTTON_BLE);
  if (readingC != lastButtonState_BLE) {
    lastDebounceTime_BLE = millis();
  }
  if ((millis() - lastDebounceTime_BLE) > debounceDelay) {
    if (readingC != buttonState_BLE) {
      buttonState_BLE = readingC;
        BLE_state = !BLE_state;
        Serial.println("BLE state: "+String(BLE_state));
    }
  }
  lastButtonState_BLE = readingC;
}