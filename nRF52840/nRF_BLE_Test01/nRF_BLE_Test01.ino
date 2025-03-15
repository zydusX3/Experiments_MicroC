// #define SerialHCI Serial1
#include <Adafruit_TinyUSB.h>
#include <ArduinoBLE.h>

bool toggle = true;

BLEService myService("180A");   // Custom BLE Service
BLEStringCharacteristic myCharacteristic("2A29", BLERead | BLENotify, 20);
BLEStringCharacteristic myGPSCharacteristic("2A29", BLERead | BLENotify, 20);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    // while (!Serial);

    if (!BLE.begin()) {
      Serial.println("Starting BLE failed!");
      while (1);
    }

    BLE.setLocalName("nRF52840_Device");  // Name that appears on your phone
    BLE.setAdvertisedService(myService);

    myService.addCharacteristic(myCharacteristic);
    BLE.addService(myService);

    BLE.advertise(); // Start BLE advertising

    // Serial.println("BLE Device is Ready to Connect...");
}

void loop() {
    BLEDevice central = BLE.central();

    if (central) {
        // Serial.print("Connected to: ");
        // Serial.println(central.address());

        while (central.connected()) {
            myCharacteristic.writeValue("Hello Mobile");
            delay(1000);
        }

        // Serial.println("Disconnected...");
    }
    toggleLED();
}

void toggleLED() {
  if(toggle) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  toggle = !toggle;
}