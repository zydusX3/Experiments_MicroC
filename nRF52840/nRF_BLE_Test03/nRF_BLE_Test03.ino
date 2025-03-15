#include <bluefruit.h>

// Define LED pins (Check your board's documentation)
#define LED_RED    11
#define LED_GREEN  13
#define LED_BLUE   12

BLEUart bleUart;  // BLE UART Service

void setup() {
    Serial.begin(115200);
    // while (!Serial); // Wait for Serial Monitor

    // Initialize Bluefruit
    Bluefruit.begin();
    Bluefruit.setName("XIAO_BLE_LED");

    // Configure and start the BLE UART service
    bleUart.begin();
    
    // Set up advertising
    startAdv();
    
    // Set LED pins as output
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);

    if(Serial) Serial.println("BLE Ready! Waiting for connections...");
}

void loop() {
    if (bleUart.available()) {
        char receivedChar = bleUart.read();
        if(Serial) Serial.print("Received: ");
        if(Serial) Serial.println(receivedChar);

        // Control LED based on received character
        controlLED(receivedChar);
    }
}

// Function to control RGB LED
void controlLED(char command) {
    if (command == 'R' || command == 'r') {
        analogWrite(LED_RED, 0);
        analogWrite(LED_GREEN, 255);
        analogWrite(LED_BLUE, 255);
    } else if (command == 'G' || command == 'g') {
        analogWrite(LED_RED, 255);
        analogWrite(LED_GREEN, 0);
        analogWrite(LED_BLUE, 255);
    } else if (command == 'B' || command == 'b') {
        analogWrite(LED_RED, 255);
        analogWrite(LED_GREEN, 255);
        analogWrite(LED_BLUE, 0);
    } else if (command == 'O' || command == 'o') {  // Turn Off LED
        analogWrite(LED_RED, 255);
        analogWrite(LED_GREEN, 255);
        analogWrite(LED_BLUE, 255);
    } else if (command == 'I' || command == 'i') {  // Turn on LED
        analogWrite(LED_RED, 0);
        analogWrite(LED_GREEN, 0);
        analogWrite(LED_BLUE, 0);
    }else {
        if(Serial) Serial.println("Invalid Command! Use 'R', 'G', 'B', or 'O'.");
    }
}

// Function to start BLE advertising
void startAdv() {
    Bluefruit.Advertising.addService(bleUart);
    Bluefruit.Advertising.addName();
    Bluefruit.Advertising.start();
}
