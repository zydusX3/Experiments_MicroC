#include <bluefruit.h>

#define BATTERY_ADC_PIN PIN_VBAT  // Read battery voltage from VBAT pin
#define BATTERY_MAX_VOLTAGE 3.7   // Maximum battery voltage
#define BATTERY_MIN_VOLTAGE 3.0   // Minimum

// Create BLE Battery Service & Characteristic
BLEService Battery_Service("180F");  // Standard BLE Battery Service UUID
BLECharacteristic Battery_Status("2A19", CHR_PROPS_READ | CHR_PROPS_NOTIFY, 4); // Standard Battery Level UUID

float lastBatteryLevel = -1;  // Store last battery level

void setup() {
    Serial.begin(115200);
    // while (!Serial) delay(10);

    // Initialize LEDs
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    // digitalWrite(LED_RED, LOW);  // Default: Not Connected

    // Initialize BLE
    Bluefruit.begin();
    Bluefruit.setName("XIAO_Battery");
    Bluefruit.Periph.setConnectCallback(connectCallback);
    Bluefruit.Periph.setDisconnectCallback(disconnectCallback);

    // Configure Battery Service & Characteristic
    Battery_Service.begin();

    Battery_Status.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    Battery_Status.setUserDescriptor("Battery Level");
    Battery_Status.setFixedLen(4);
    Battery_Status.begin();

    // Add Battery Service to BLE Advertising
    Bluefruit.Advertising.addService(Battery_Service);
    Bluefruit.Advertising.addName();
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.start();
}

// Function to read battery voltage
float getBatteryLevel() {
    const int numSamples = 10;
    int total = 0;
    for (int i = 0; i < numSamples; i++) {
        total += analogRead(BATTERY_ADC_PIN);
        delay(5);
    }
    float adcValue = total / (float) numSamples;
    float voltage = (adcValue * (3.3 / 1023.0)) * 2; // nRF52840 has an internal 1:2 voltage divider

    // int adcValue = analogRead(BATTERY_ADC_PIN); 
    // float voltage = adcValue * (3.6 / 1023.0);  // Convert ADC to voltage

    return voltage;
}

// Function to update battery level (only once when it changes)
void updateBatteryLevel() {
    digitalWrite(LED_BLUE, LOW);  // Blink Blue when checking battery
    delay(1000);
    digitalWrite(LED_BLUE, HIGH);

    float batteryVoltage = getBatteryLevel();

    if (batteryVoltage != lastBatteryLevel) {  // Send update only if changed
        Serial.print("Battery Level Updated: ");
        Serial.println(batteryVoltage, 2);

        Battery_Status.notify((uint8_t*)&batteryVoltage, sizeof(batteryVoltage));
        lastBatteryLevel = batteryVoltage;
    }
}

void loop() {
    updateBatteryLevel();
    delay(5000);  // Check battery every 5 seconds
}

// BLE Connection Callback
void connectCallback(uint16_t conn_handle) {
    Serial.println("Connected to Central Device!");
    // digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);  // Green LED ON
}

// BLE Disconnection Callback
void disconnectCallback(uint16_t conn_handle, uint8_t reason) {
    Serial.println("Disconnected from Central Device.");
    digitalWrite(LED_GREEN, HIGH);
    // digitalWrite(LED_RED, LOW);  // Red LED ON
}