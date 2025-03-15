#include <bluefruit.h>
#include <Adafruit_NeoPixel.h>

#define Ring_RGB_PIN  0     // Control RGB LED strip
#define NUM_LEDS      5     // Number of LEDs
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, Ring_RGB_PIN, NEO_GRB + NEO_KHZ800); // Create a NeoPixel object

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // Use a custom UUID

// Define Characteristics
BLECharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_NOTIFY);
BLECharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", CHR_PROPS_READ | CHR_PROPS_NOTIFY);
BLECharacteristic functionResultCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", CHR_PROPS_READ | CHR_PROPS_NOTIFY, 50);

bool led_st = false;
bool btn_st = false;
String myGPS = "";
String lastResult = "";

// 🔹 BLE Initialization
void ble_init() {
  Serial.begin(115200);
  Serial.println("Starting BLE...");

  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.Periph.setConnectCallback(ble_connect_callback);
  Bluefruit.Periph.setDisconnectCallback(ble_disconnect_callback);

  configureBLE();

  Serial.println("BLE Initialized");
}

// 🔹 BLE Configuration
void configureBLE() {
  Bluefruit.setName("Frisbee");

  // Start the Service
  ledService.begin();

  // Start Characteristics
  ledCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  ledCharacteristic.setFixedLen(1);
  ledCharacteristic.begin();

  // Register callback for when the central writes data
  ledCharacteristic.setWriteCallback(ledCharacteristicWriteCallback);

  buttonCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  buttonCharacteristic.setFixedLen(1);
  buttonCharacteristic.begin();

  functionResultCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  functionResultCharacteristic.setMaxLen(50);
  functionResultCharacteristic.begin();

  // Start Advertising
  startAdvertising();

  Serial.println("BLE Service & Characteristics Configured");
}

// 🔹 Start BLE Advertising
void startAdvertising() {
  Bluefruit.Advertising.addService(ledService);
  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.start();
}

// 🔹 Callback Functions for Connection & Disconnection
void ble_connect_callback(uint16_t conn_handle) {
  Serial.println("BLE Connected");
}

void ble_disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  Serial.println("BLE Disconnected. Restarting advertising...");
  Bluefruit.Advertising.start();
}

// 🔹 Function to Turn On BLE
void turnOnBLE() {
  // Bluefruit.begin();
  Serial.println("BLE turned ON");
  // configureBLE();
}

// 🔹 Function to Turn Off BLE
void turnOffBLE() {
  // Bluefruit.Advertising.stop();
  // Bluefruit.Central.disconnect(); // Corrected disconnect function
  Serial.println("BLE turned OFF");
}

// 🔹 BLE Scanning & Updates
void scanBLE() {
  if (Serial.available()) {
    myGPS = Serial.readString();
    Serial.println("Updated GPS: " + myGPS);
  }

  // Notify if GPS Data Changes
  if (functionResultCharacteristic.notifyEnabled() && myGPS != lastResult) {
    lastResult = myGPS;
    functionResultCharacteristic.notify(myGPS.c_str(), myGPS.length());
    Serial.println("Sent GPS update: " + myGPS);
  }

  // // Handle LED Control
  // if (ledCharacteristic.notifyEnabled()) { // Corrected condition
  //   char charBLE = ledCharacteristic.read8();
  //   Serial.print("LED Characteristic Received: ");
  //   Serial.println(charBLE);
  //   RGBControl(charBLE);
  // }
}

void setup() {
  ble_init();
}

// 🔹 BLE Loop
void loop() {
  scanBLE();   // Check for BLE interactions
  delay(100);  // Avoid busy looping
}

// Function to handle when LED characteristic is written
void ledCharacteristicWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
    if (len > 0) {
        char charBLE = data[0];  // Read first byte of received data
        Serial.print("LED Characteristic Received: ");
        Serial.println(charBLE);
        RGBControl(charBLE); // Call function to update LED state
    }
}

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    // strip.setPixelColor(0, strip.Color(red, green, blue));
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
}

void nRF_setColor(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(LED_RED, 255-red);
  analogWrite(LED_GREEN, 255-green);
  analogWrite(LED_BLUE, 255-blue);
}

void RGBControl(char BLEchar) {
  switch (BLEchar) {
    case '1': Serial.println("LED on");
              nRF_setColor(255, 255, 255);
              setColor(255, 255, 255);
              // digitalWrite(LED_PIN, HIGH);
              break;
    case 'r':
    case 'R': Serial.println("RGB R");
              nRF_setColor(255, 0, 0);
              setColor(255, 0, 0);
              break;
    case 'g':
    case 'G': Serial.println("RGB G");
              nRF_setColor(0, 255, 0);
              setColor(0, 255, 0);
              break;
    case 'b':
    case 'B': Serial.println("RGB B");
              nRF_setColor(0, 0, 255);
              setColor(0, 0, 255);
              break;
    case 'y':
    case 'Y': Serial.println("RGB Y");
              nRF_setColor(255, 255, 0);
              setColor(255, 255, 0);
              break;
    case 'c':
    case 'C': Serial.println("RGB C");
              nRF_setColor(0, 255, 255);
              setColor(0, 255, 255);
              break;
    case 'm':
    case 'M': Serial.println("RGB M");
              nRF_setColor(255, 0, 255);
              setColor(255, 0, 255);
              break;
    default:  Serial.println("LED off");
              nRF_setColor(0, 0, 0);
              setColor(0, 0, 0);
              // digitalWrite(LED_PIN, LOW);
              break;
  }
}