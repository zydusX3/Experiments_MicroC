#include "myGlobal.h"
#include "myFunctions.h"
#include "myOLED.h"
#include "myRGB.h"
#include "myBlue.h"
#include "myGPS.h"

bool ISR_flag = false;
bool dbg_serial = true;
String myGPS = "19.99, 20.22";              // GPS variable to be Notified in BLE
String lastResult = "";                     // To track the last value
String LiveGPS = "";                        // Store GPS Data here

int lastISRtime = 0;                        // last valid ISR trigger time
int ISRminDelay = 1000;                     // Minimum delay between ISR trigger
bool btn_st = false;                        // initial Button state

bool last_OLED_state = true;
bool last_GPS_state  = true;
bool last_BLE_state  = true;
unsigned long lastPOWERDebounceTime = 0;         // The last time the button was toggled
unsigned long POWERdebounceDelay = 5000;           // Debounce delay in milliseconds


void IRAM_ATTR buttonISR() {            // Interrupt Service Routine
  if ((millis() - lastISRtime) > ISRminDelay) {
    lastISRtime = millis();
    ISR_flag = true;
    // buttonCharacteristic.writeValue(btn_st);
    // onButtonPress();
  }
}

void PowerAccess() {
  if(last_OLED_state != OLED_state) {
    if(!OLED_state) {
      // on 0 (Button Up) - turn off
      turnOffOLED();
    } else {
      // on 1 (Button Down) - turn on
      turnOnOLED();
    }
    last_OLED_state = OLED_state;
  }
  if(last_GPS_state != GPS_state) {
    if(!GPS_state) {
      // on 0 (Button Up) - turn off
      turnOffGPS();
    } else {
      // on 1 (Button Down) - turn on
      turnOnGPS();
    }
    last_GPS_state = GPS_state;
  }
  if(last_BLE_state != BLE_state) {
    if(!BLE_state) {
      // on 0 (Button Up) - turn off
      turnOffBLE();
    } else {
      // on 1 (Button Down) - turn on
      turnOnBLE();
    }
    last_BLE_state = BLE_state;
  }
}

void setup() {
  my_init();
  setColor(255,255,255);                // White
  OLED_init();
  setColor(0, 255, 0);                  // Red
  GPS_init();
  ble_init();
  setColor(255, 0, 0);                  // Green
  ble_config();
  setColor(0, 0, 255);                  // Blue
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);  // attach ISR check to pin 7
}

void loop() {
  if(ISR_flag) {
    buttonCharacteristic.writeValue(btn_st);
    onButtonPress();
  }
  scanBLE();
  scanGPS_1s();
  if ((millis() - lastPOWERDebounceTime) > POWERdebounceDelay) {
    lastPOWERDebounceTime = millis();
    Serial.println("Check button state for POWER");
    CheckBtnState();
    PowerAccess();
  }
  delay(10);
}
