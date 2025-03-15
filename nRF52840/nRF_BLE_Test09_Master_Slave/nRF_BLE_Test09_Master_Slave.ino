#include <bluefruit.h>

// Define UUIDs for Services & Characteristics
BLEService mainService("180D");  // Example UUID
BLECharacteristic sendDataChar("19B10011-E8F2-537E-4F6C-D104768A1214", CHR_PROPS_READ | CHR_PROPS_NOTIFY, 20);
BLECharacteristic receiveDataChar("19B10012-E8F2-537E-4F6C-D104768A1214", CHR_PROPS_WRITE, 20);

// Connection Handle
uint16_t centralConnHandle = BLE_CONN_HANDLE_INVALID;
uint16_t peripheralConnHandle = BLE_CONN_HANDLE_INVALID;

void setup() {
    Serial.begin(115200);
    delay(20);
    while(!Serial);
    Serial.println("1");
    Bluefruit.begin(2, 1); // 1 Peripheral + 1 Central Role
    Bluefruit.setName("DualRole_BLE");
    Serial.println("2");
    // --- Peripheral Setup ---
    Bluefruit.Periph.setConnectCallback(peripheralConnected);
    Bluefruit.Periph.setDisconnectCallback(peripheralDisconnected);
    Serial.println("3");
    mainService.begin();
    sendDataChar.begin();
    receiveDataChar.setWriteCallback(dataReceived);
    receiveDataChar.begin();
    Serial.println("4");
    Bluefruit.Scanner.restartOnDisconnect(true);
    Bluefruit.Scanner.filterRssi(-90); // Ignore weak signals
    // Bluefruit.Scanner.start(0);
    // startAdvertising(); // Start advertising as peripheral

    // --- Central Setup ---
    Bluefruit.Scanner.setInterval(320, 240); // Faster scanning
    Bluefruit.Scanner.useActiveScan(true);  // Get more device info
    Bluefruit.Central.setConnectCallback(centralConnected);
    Bluefruit.Central.setDisconnectCallback(centralDisconnected);
    // Bluefruit.Scanner.setRxCallback(scanCallback);
    Bluefruit.Scanner.setRxCallback(scanAllDevicesCallback);
    Bluefruit.Scanner.start(0); // Continuous scan
    Serial.println("Scanning started...");

}

void loop() {
    // Example: Send a notification if connected
  //   if (Bluefruit.Periph.connected() && sendDataChar.notifyEnabled()) {
  //       const char* data = "Hello BLE";
  //       sendDataChar.notify((uint8_t*)data, strlen(data));
  //   }
  // if (!Bluefruit.Scanner.isRunning()) {
  //   Serial.println("Restarting BLE Scan for BHG_ devices...");
  //   Bluefruit.Scanner.restartOnDisconnect(true);
  //   Bluefruit.Scanner.filterRssi(-50);  // Ignore weak signals
  //   Bluefruit.Scanner.start(0);        // Continuous scan
  // } //else {
  //   Serial.println("BLE Scanner running...");
  // }
    // delay(500);
}

// ---- Peripheral Callbacks ----
void peripheralConnected(uint16_t conn_handle) {
    Serial.println("Peripheral: Device Connected");
    peripheralConnHandle = conn_handle;
}

void peripheralDisconnected(uint16_t conn_handle, uint8_t reason) {
    Serial.println("Peripheral: Device Disconnected");
    // startAdvertising(); // Restart advertising
}

void dataReceived(uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len) {
    Serial.print("Received Data: ");
    Serial.write(data, len);
    Serial.println();
}

// ---- Central Callbacks ----
void readingBLE_report(ble_gap_evt_adv_report_t* report) {
  Serial.print("MAC Address: ");
    for (int i = 0; i < 6; i++) {
      Serial.print(report->peer_addr.addr[i], HEX);
      Serial.print(":");
    }

    Serial.println();
    Serial.print("RSSI: ");
    Serial.print(report->rssi);
    Serial.println(" dBm");

    uint8_t mfgData[32] = {0};  // Buffer for Manufacturer Data
    uint8_t mfgLength = Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA, mfgData, sizeof(mfgData));
    if (mfgLength >= 2) { // At least 2 bytes for company ID
        uint16_t companyID = (mfgData[1] << 8) | mfgData[0];  // Convert to uint16_t
        Serial.print("Manufacturer ID: 0x");
        Serial.println(companyID, HEX);
        Serial.print("Manufacturer Data: ");
        for (uint8_t i = 2; i < mfgLength; i++) {
            Serial.print(mfgData[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }

    uint8_t uuidData[32] = {0};  // Buffer for UUID data
    // Extract 16-bit Service UUIDs
    uint8_t uuidLength = Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE, uuidData, sizeof(uuidData));

    if (uuidLength > 0) {
        Serial.print("16-bit Service UUIDs: ");
        for (uint8_t i = 0; i < uuidLength; i += 2) {
            uint16_t uuid = (uuidData[i + 1] << 8) | uuidData[i];
            Serial.print("0x");
            Serial.print(uuid, HEX);
            Serial.print(" ");
        }
        Serial.println();
    }

    // Extract 128-bit Service UUIDs
    uint8_t uuid128Data[32] = {0};
    uint8_t uuid128Length = Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE, uuid128Data, sizeof(uuid128Data));

    if (uuid128Length > 0) {
        Serial.print("128-bit Service UUID: ");
        for (uint8_t i = 0; i < uuid128Length; i++) {
            Serial.print(uuid128Data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}

void scanCallback(ble_gap_evt_adv_report_t* report) {
    Serial.println("Scanning... Found something!");
    Serial.println("Scanning... Filtering!");
    // Bluefruit.printInfo();  // Print all scan info
    uint8_t nameBuffer[32] = {0};  // Use uint8_t instead of char

    // Extract device name using parseReportByType
    uint8_t nameLength = Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, nameBuffer, sizeof(nameBuffer));

    // Ensure the extracted name is null-terminated for safe comparison
    nameBuffer[nameLength] = '\0';

    // Convert uint8_t* to char* for comparison
    char* deviceName = (char*)nameBuffer;
    Serial.print("Found Device: ");
    Serial.println(deviceName);

    readingBLE_report(report);

    // If device name not found, try reading from SHORT NAME
    if (nameLength == 0) {
        nameLength = Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME, nameBuffer, sizeof(nameBuffer));
        nameBuffer[nameLength] = '\0';  
        Serial.print("Short Name Found: ");
        Serial.println(deviceName);
    }
    // Check if the device name starts with "BHG_"
    if (nameLength > 4 && strncmp(deviceName, "BHG_", 4) == 0) {
        Serial.print("Central: Found BHG Device - ");
        Serial.println(deviceName);
        
        Bluefruit.Central.connect(report);  // Connect to this device
    } else {
        Serial.print("Central: Skipping non-BHG device: ");
        Serial.println(deviceName);
    }
}

void scanAllDevicesCallback(ble_gap_evt_adv_report_t* report) {
    Serial.println("\n🔍 Scanning for all BLE devices...");

    uint8_t nameBuffer[32] = {0};  
    uint8_t nameLength = Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, nameBuffer, sizeof(nameBuffer));

    if (nameLength == 0) {
        nameLength = Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME, nameBuffer, sizeof(nameBuffer));
    }
    nameBuffer[nameLength] = '\0';

    char* deviceName = (char*)nameBuffer;
    Serial.print("📡 Found Device: ");
    Serial.println(deviceName);
    
    // Print MAC Address
    Serial.print("🆔 MAC Address: ");
    Serial.print(report->peer_addr.addr[5], HEX);
    Serial.print(":");
    Serial.print(report->peer_addr.addr[4], HEX);
    Serial.print(":");
    Serial.print(report->peer_addr.addr[3], HEX);
    Serial.print(":");
    Serial.print(report->peer_addr.addr[2], HEX);
    Serial.print(":");
    Serial.print(report->peer_addr.addr[1], HEX);
    Serial.print(":");
    Serial.println(report->peer_addr.addr[0], HEX);

    // Print RSSI (Signal Strength)
    Serial.print("📶 RSSI: ");
    Serial.print(report->rssi);
    Serial.println(" dBm");

    // Print Manufacturer Data and UUIDs
    // readingBLE_report(report);
}

void centralConnected(uint16_t conn_handle) {
    Serial.println("Central: Connected to Peripheral");
    centralConnHandle = conn_handle;
}

void centralDisconnected(uint16_t conn_handle, uint8_t reason) {
    Serial.println("Central: Disconnected");
    Serial.println(reason);
    Bluefruit.Scanner.start(0); // Restart scanning
}

// ---- Advertising Function ----
void startAdvertising() {
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addService(mainService);
    Bluefruit.Advertising.addName();
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.start(0);
}
