#include "myGlobal.h"
#include "myBLE.h"
#include "myGPS.h"
#include "myOLED.h"
#include "myFunctions.h"
#include "myRGB.h"

bool ISR_flag = false;
bool dbg_serial = true;
String myGPS = "19.99, 20.22";              // GPS variable to be Notified in BLE
String lastResult = "";                     // To track the last value
String LiveGPS = "";                        // Store GPS Data here

int lastISRtime = 0;                        // last valid ISR trigger time
int ISRminDelay = 1000;                     // Minimum delay between ISR trigger
bool btn_st = false;                        // initial Button state

// bool last_OLED_state = true;
// bool last_GPS_state  = true;
// bool last_BLE_state  = true;
// unsigned long lastPOWERDebounceTime = 0;         // The last time the button was toggled
// unsigned long POWERdebounceDelay = 5000;           // Debounce delay in milliseconds

void buttonISR() {            // Interrupt Service Routine
  unsigned long currentTime = millis();
  if ((currentTime - lastISRtime) > ISRminDelay) {
    lastISRtime = currentTime;
    ISR_flag = true;
  }
}

void setup() {
  my_init();
  nRF_setColor(0,0,0);                // White
  OLED_init();
  nRF_setColor(255, 0, 0);                  // Red
  GPS_init();
  ble_init();
  nRF_setColor(0, 255, 0);                  // Green
  ble_config();
  nRF_setColor(0, 0, 255);                  // Blue
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);  // attach ISR check to pin 7
}

void loop() {
  if(ISR_flag) {
    Serial.println("ISR ISR ISR ...!");
    // buttonCharacteristic.writeValue(btn_st);
    onButtonPress();
    ISR_flag = false;  // Reset flag after processing
  }
  scanBLE();
  scanGPS_1s();
  delay(10);
}
