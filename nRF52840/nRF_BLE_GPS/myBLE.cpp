#include "myBLE.h"
#include "myRGB.h"

// Define Service
BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // Use a custom UUID

// Define Characteristics
BLECharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_NOTIFY);
BLECharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", CHR_PROPS_READ | CHR_PROPS_NOTIFY);
BLECharacteristic functionResultCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", CHR_PROPS_READ | CHR_PROPS_NOTIFY, 50);

// Function to handle when LED characteristic is written
void ledCharacteristicWriteCallback(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
    if (len > 0) {
        char charBLE = data[0];  // Read first byte of received data
        Serial.print("LED Characteristic Received: ");
        Serial.println(charBLE);
        RGBControl(charBLE); // Call function to update LED state
    }
}

// 🔹 Callback Functions for Connection & Disconnection
void ble_connect_callback(uint16_t conn_handle) {
  Serial.println("BLE Connected");
}

void ble_disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  Serial.println("BLE Disconnected. Restarting advertising...");
  Bluefruit.Advertising.start();
}

// 🔹 Start BLE Advertising
void startAdvertising() {
  Bluefruit.Advertising.addService(ledService);
  Bluefruit.Advertising.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.start();
}

// // BLE Service and Characteristics
// BLEService        gpsService                = BLEService("19B10010-E8F2-537E-4F6C-D104768A1214");         // Replace with your UUID
// BLECharacteristic ledCharacteristic         = BLECharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214");  // Replace with your UUID
// BLECharacteristic gpsCharacteristic         = BLECharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214");  // Replace with your UUID
// BLECharacteristic buttonCharacteristic      = BLECharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214");  // Replace with your UUID
// BLECharacteristic gpsRequestCharacteristic  = BLECharacteristic("19B10014-E8F2-537E-4F6C-D104768A1214");  // Replace with your UUID

void ble_init() {
  // // Initialize Bluefruit
  // Bluefruit.begin();
  // Bluefruit.setName("XIAO_BLE"); // Set your device name
  // // Bluefruit.setTxPower(-4);    // Set transmit power if needed
  Serial.println("Starting BLE...");
  Bluefruit.begin();
  // Bluefruit.setTxPower(4);
  Bluefruit.Periph.setConnectCallback(ble_connect_callback);
  Bluefruit.Periph.setDisconnectCallback(ble_disconnect_callback);
}

void ble_config() {
  // // Configure services and characteristics
  // gpsService.begin();
 
  // ledCharacteristic.begin();
  // gpsCharacteristic.begin();
  // buttonCharacteristic.begin();
  // gpsRequestCharacteristic.begin();
 
  // // Set properties (Notify for GPS)
  // gpsCharacteristic.setProperties(BLECharacteristic::PROPERTY_NOTIFY);
  // gpsRequestCharacteristic.setProperties(BLECharacteristic::PROPERTY_WRITE);
 
  // // Add characteristics to the service
  // gpsService.addCharacteristic(ledCharacteristic);
  // gpsService.addCharacteristic(gpsCharacteristic);
  // gpsService.addCharacteristic(buttonCharacteristic);
  // gpsService.addCharacteristic(gpsRequestCharacteristic);
 
  // // Start advertising
  // Bluefruit.Advertising.addService(gpsService);
  // Bluefruit.Advertising.start();
  // // Set connection/disconnection callbacks
  // Bluefruit.Connection.setCallbacks(connect_callback, disconnect_callback);

  Bluefruit.setName("Frisbee2");

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

void turnOnBLE() {
  //wait
}

void turnOffBLE() {
  //wait
}

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
  // // LED Control
  // if (ledCharacteristic.available()) {
  //   uint8_t ledValue = ledCharacteristic.read8();
  //   // Example: Map received value to RGB color (adjust as needed)
  //   uint32_t color = (ledValue << 16) | (ledValue << 8) | ledValue; // Simple example
  //   setLedColor(color);
  // }
 
  // // GPS Update and Notify
  // updateGPSData();
  // gpsCharacteristic.write(&gpsValues[gpsIndex -1] , sizeof(GPSData));  // Notify connected devices.
 
  // // Button State
  // bool newButtonState = digitalRead(BUTTON_PIN);
  // if (newButtonState != buttonState) {
  //   buttonState = newButtonState;
  //   buttonCharacteristic.write8(buttonState);
  // }
 
  // // GPS Request
  // if (gpsRequestCharacteristic.available()) {
  //   gpsRequestCharacteristic.read(); // Consume the request
  //   sendGPSData();
  //   gpsRequestCharacteristic.write(gpsJson.c_str(), gpsJson.length());
  // }
 
  // // delay(100); // Adjust delay as needed
}

// void init_GPSchara() {}
// void init_BTNchar() {}

// // BLE Callbacks
// void connect_callback(uint32_t conn_handle) {
//   setLedColor(0x00FF00); // Green on connect
// }

// void disconnect_callback(uint32_t conn_handle, uint8_t reason) {
//   setLedColor(0xFF0000); // Red on disconnect
// }
 