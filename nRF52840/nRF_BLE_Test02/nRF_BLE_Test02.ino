#include <bluefruit.h>

BLEUart bleuart;  // Create UART service

void setup() {
  Serial.begin(115200);
  Bluefruit.begin();
  Bluefruit.setName("XIAO-BLE");  // Set device name

  bleuart.begin();  // Star

  Bluefruit.setEventCallback(myBLEEventHandler);
  
  Bluefruit.autoConnLed(true);
  Bluefruit.setConnLedInterval(1000);
  Bluefruit.Advertising.clearData();
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addName();        // Add the custom name to advertisements
  Bluefruit.Advertising.addService(bleuart);

  // Start advertising BLE
  Bluefruit.Advertising.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (bleuart.available()) {
    char c = bleuart.read();
    delay(100);
    Serial.print(c);
  }
}

void myBLEEventHandler(ble_evt_t* evt) {
    if (evt == nullptr) return;

    switch (evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            Serial.println("✅ BLE Device Connected!");
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            Serial.println("❌ BLE Device Disconnected!");
            break;

        case BLE_GAP_EVT_ADV_SET_TERMINATED:
            Serial.println("🔄 Advertising Stopped!");
            break;

        default:
            Serial.print("📢 BLE Event ID: ");
            Serial.println(evt->header.evt_id);
            break;
    }
}
