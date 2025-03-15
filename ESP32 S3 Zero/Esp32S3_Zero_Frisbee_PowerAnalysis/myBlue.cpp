#include "myBlue.h"
#include "myRGB.h"

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEStringCharacteristic functionResultCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 50);

// bool led_st = false;                        // initial Button state

void ble_init() {
  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1) {
      toggleRed();                      // Blink Red
    }
  }
}

void ble_config() {
  BLE.setLocalName("Frisbee");          // Set the local name peripheral advertises
  BLE.setAdvertisedService(ledService); // Set the UUID for the service this peripheral advertises:
  // Add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  ledService.addCharacteristic(buttonCharacteristic);
  ledService.addCharacteristic(functionResultCharacteristic);
  // Add descriptors for user-friendly names
  BLEDescriptor char1Descriptor("2901", "RGB Control");
  BLEDescriptor char2Descriptor("2901", "Button State");
  BLEDescriptor char3Descriptor("2901", "GPS Data");
  ledCharacteristic.addDescriptor(char1Descriptor);
  buttonCharacteristic.addDescriptor(char2Descriptor);
  functionResultCharacteristic.addDescriptor(char3Descriptor);
  // Add the service
  BLE.addService(ledService);
  ledCharacteristic.writeValue(led_st);
  buttonCharacteristic.writeValue(btn_st);
  functionResultCharacteristic.writeValue("Ready");
  BLE.advertise();                      // Start advertising
  Serial.println("Bluetooth® device active, waiting for connections...");
}

void turnOnBLE() {
  if (BLE.begin()) { // Restart BLE peripheral
    Serial.println("BLE turned on");
    ble_config();
    // BLE.advertise(); // Restart advertising (if needed)
  } else {
    Serial.println("Failed to start BLE");
  }
}

void turnOffBLE() {
  BLE.end();        // Stops the BLE peripheral and advertising
  Serial.println("BLE turned off");
}

void scanBLE() {
  BLE.poll();   // poll for Bluetooth® Low Energy events
  if (Serial.available()){
    myGPS = Serial.readString();
    Serial.println("Updated GPS is: "+myGPS);
  }
  //  Notify if GPS Change
  if (functionResultCharacteristic.subscribed()) {
    if (myGPS != lastResult) {
      lastResult = myGPS;
      functionResultCharacteristic.writeValue(myGPS);
      Serial.println("Sent function result via notification: " + myGPS);
    }
  }
  //  RGB control
  if (ledCharacteristic.written()) {
    Serial.print("ledCharacteristic = ");
    char charBLE = ledCharacteristic.value();
    Serial.println(charBLE);
    RGBControl(charBLE);
  }
}