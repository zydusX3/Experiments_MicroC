#define BUTTON_OLED 4
#define BUTTON_GPS 5
#define BUTTON_BLE 6

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
unsigned long debounceDelay = 50;   // Debounce delay in milliseconds

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_OLED, INPUT_PULLUP);
  pinMode(BUTTON_GPS, INPUT_PULLUP);
  pinMode(BUTTON_BLE, INPUT_PULLUP);
  Serial.println("Ready ...");
}

void loop() {
  bool readingA = digitalRead(BUTTON_OLED);
  // Serial.println("readingA: "+String(readingA));
  if (readingA != lastButtonState_OLED) {
    // Serial.println("readingA new; lastDebounceTime_OLED: "+String(lastDebounceTime_OLED));
    lastDebounceTime_OLED = millis();
  }
  if ((millis() - lastDebounceTime_OLED) > debounceDelay) {
    // Serial.println("readingA > debounceDelay");
    if (readingA != buttonState_OLED) {
      // Serial.println("readingA != buttonState_OLED "+String(buttonState_OLED));
      buttonState_OLED = readingA;
      // if (buttonState_OLED == LOW) {
        OLED_state = !OLED_state;
        Serial.println("OLED state: "+String(OLED_state));
      // }
    }
  }
  // Save the reading as the last state
  lastButtonState_OLED = readingA;
  //////////////////////////////////////////////////////
  bool readingB = digitalRead(BUTTON_GPS);
  if (readingB != lastButtonState_GPS) {
    lastDebounceTime_GPS = millis();
  }
  if ((millis() - lastDebounceTime_GPS) > debounceDelay) {
    if (readingB != buttonState_GPS) {
      buttonState_GPS = readingB;
      // if (buttonState_GPS == LOW) {
        GPS_state = !GPS_state;
        Serial.println("GPS state: "+String(GPS_state));
      // }
    }
  }
  // Save the reading as the last state
  lastButtonState_GPS = readingB;
  //////////////////////////////////////////////////////
  bool readingC = digitalRead(BUTTON_BLE);
  if (readingC != lastButtonState_BLE) {
    lastDebounceTime_BLE = millis();
  }
  if ((millis() - lastDebounceTime_BLE) > debounceDelay) {
    if (readingC != buttonState_BLE) {
      buttonState_BLE = readingC;
      // if (buttonState_BLE == LOW) {
        BLE_state = !BLE_state;
        Serial.println("BLE state: "+String(BLE_state));
      // }
    }
  }
  // Save the reading as the last state
  lastButtonState_BLE = readingC;

  delay(500);
}