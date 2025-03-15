#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>

#define RGB_PIN    21  // GPIO pin connected to the RGB 
#define LED_PIN    6  // GPIO pin connected to the RGB LED
#define NUM_LEDS   1   // Number of LEDs
#define BUTTON_PIN 2   // GPIO pin connected to the button

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

const int ledPin = LED_PIN; // set ledPin to on-board LED
const int buttonPin = BUTTON_PIN; // set buttonPin to digital pin 4
int led_st = 0, btn_st = 0;
unsigned long lastDebounceTime = 0; // The last time the button was toggled
unsigned long debounceDelay = 50;  // Debounce delay in milliseconds
bool t_red = false;
bool t_green = false;
bool t_blue = false;
int currentColor = 0; // Tracks the current color index
String myGPS = "19.99,20.22";
unsigned long lastNotifyTime = 0; // To track the last notification time
const unsigned long notifyInterval = 1000; // Interval in milliseconds
String lastResult = ""; // To track the last value

// List of colors
uint32_t colors[] = {
  strip.Color(255, 0, 0),   // Red
  strip.Color(0, 255, 0),   // Green
  strip.Color(0, 0, 255),   // Blue
  strip.Color(255, 255, 0), // Yellow
  strip.Color(0, 255, 255), // Cyan
  strip.Color(255, 0, 255), // Magenta
  strip.Color(255, 255, 255), // White
  strip.Color(0, 0, 0)      // Off
};

int numColors = sizeof(colors) / sizeof(colors[0]);

// class MyCallbacks : public BLECharacteristicCallbacks {
//   void onRead(BLECharacteristic* pCharacteristic) {
//     String result = "Function Result: ";
//     result += millis(); // Example: Return elapsed time in milliseconds
//     pCharacteristic->setValue(result.c_str()); // Set the new value
//     Serial.println("Updated function result on read: " + result);
//   }
// };

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEStringCharacteristic functionResultCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 50);
BLEByteCharacteristic triggerCharacteristic("19B10014-E8F2-537E-4F6C-D104768A1214", BLEWrite);


void setup() {
  Serial.begin(9600);
  while (!Serial);  // Blocks if serial is not opened in Arduino
  pinMode(ledPin, OUTPUT); // use the LED as an output
  pinMode(buttonPin, INPUT_PULLUP); // use button pin as an input
  setColor(255, 255, 255); // White
  delay(500);
  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    toggleRed();
    while (1);
  }
  setColor(0, 255, 0);

  // set the local name peripheral advertises
  BLE.setLocalName("ButtonLED");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);
  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  ledService.addCharacteristic(buttonCharacteristic);
  ledService.addCharacteristic(functionResultCharacteristic);
  ledService.addCharacteristic(triggerCharacteristic);
  // add the service
  // functionResultCharacteristic.setCallbacks(new MyCallbacks());
  BLE.addService(ledService);
  ledCharacteristic.writeValue(led_st);
  buttonCharacteristic.writeValue(btn_st);
  functionResultCharacteristic.writeValue("Ready");
  // start advertising
  BLE.advertise();
  Serial.println("Bluetooth® device active, waiting for connections...");
  setColor(0, 0, 255);
}

void loop() {
  // poll for Bluetooth® Low Energy events
  BLE.poll();

  if(Serial.available()){
    myGPS = Serial.readString();
    Serial.println("Updated GPS is: "+myGPS);
  }

  // read the current button pin state
  // bool reading = digitalRead(buttonPin);
  // if (reading != btn_st) {
  //   lastDebounceTime = millis();
  // }
  // if ((millis() - lastDebounceTime) > debounceDelay) {
  //   if (reading != btn_st) {
  //     btn_st = reading;
  //     if (btn_st == LOW) {
  //       // toggleColor();
  //       Serial.println(btn_st);
  //       buttonCharacteristic.writeValue(btn_st);
  //     }
  //   }
  // }
  // -----------------
  if(digitalRead(buttonPin) != btn_st) {
    btn_st = !btn_st;
    // Serial.println(btn_st);
    buttonCharacteristic.writeValue(btn_st);
  }

  if (functionResultCharacteristic.subscribed()) {
    // unsigned long currentTime = millis();
    // if (currentTime - lastNotifyTime >= notifyInterval) {
      // lastNotifyTime = currentTime;
      // String result = "Function Result: ";
      // result += millis();
      // functionResultCharacteristic.writeValue(result);
      // Serial.println("Sent function result via notification: " + result);

      if (myGPS != lastResult) {
        lastResult = myGPS;
        functionResultCharacteristic.writeValue(myGPS);
        Serial.println("Sent function result via notification: " + myGPS);
      } //else {
      //   Serial.println("GPS nochange!");
      // }
    // }
  }

  // Check if the central wrote to the trigger characteristic
  if (triggerCharacteristic.written()) {
    // String newGPS = String((const char*)triggerCharacteristic.value()); // Read the new string value
    // ---------------------------------------------------------------
    // const unsigned char* rawValue = triggerCharacteristic.value();
    // if (rawValue == nullptr || strlen((const char*)rawValue) == 0) {
    //     Serial.println("Error: Empty or invalid value received.");
    //     return; // Exit if the value is null or empty
    // }
    // String newGPS = String((const char*)rawValue); // Convert to String safely
    // ---------------------------------------------------------------
    // String newGPS = triggerCharacteristic.value();
    // Check if the value is valid (not empty)
    // ---------------------------------------------------------------
    Serial.println("Trigger available");
    size_t valueLength = triggerCharacteristic.valueLength();
    Serial.println("valueLength available");
    const unsigned char* rawValue = triggerCharacteristic.value();
    Serial.println("rawValue available");

    // Ensure we have a valid value
    if (valueLength == 0 || rawValue == nullptr) {
        Serial.println("Error: Empty or invalid value received.");
        return;
    }
    Serial.println("rawValue after check");
    // Create a temporary buffer to hold the null-terminated string
    char valueBuffer[valueLength + 1];
    memcpy(valueBuffer, rawValue, valueLength); // Copy the raw data
    valueBuffer[valueLength] = '\0';           // Null-terminate
    Serial.println("rawValue available");
    // Convert to Arduino String
    String newGPS = String(valueBuffer);
    // ------------------------------------------------------------------
    if (newGPS.length() == 0) {
        Serial.println("Error: Empty or invalid value received.");
        return; // Exit if the value is empty
    }

    Serial.println("Triggered : " + newGPS);
    fetchGPS(); // Grab GPS value here
    functionResultCharacteristic.writeValue(myGPS.c_str());
    Serial.println("Triggered function result: " + myGPS);
    
  }

  if (ledCharacteristic.written()) {
    setColor(255, 255, 0);
    Serial.print("ledCharacteristic = ");
    Serial.println(ledCharacteristic.value());
    
    // update LED, either central has written to characteristic or button state has changed
    if (ledCharacteristic.value() == '1') {
      Serial.println("LED on");
      digitalWrite(ledPin, HIGH);
    } else {
      Serial.println("LED off");
      digitalWrite(ledPin, LOW);
    }
    delay(500);
    setColor(0, 0, 255);
  }
}


void fetchGPS() {
  // Similate the GPS fetch function. Replace with the actual function.
  Serial.println("Fetching GPS Here ...");
  for(int i=9; i>=0; i--) {
    Serial.print(i+", ");
    delay(300);
  }
  Serial.println("\nFetching GPS Done");
}

void toggleRed(){
  t_red = !t_red;
  if(t_red) {
    setColor(255, 0, 0); // Red
  } else {
    setColor(0, 0, 0);     // Off
  }
}

void toggleGreen(){
  t_green = !t_green;
  if(t_green) {
    setColor(0, 255, 0); // Green
  } else {
    setColor(0, 0, 0);      // Off
  }
}

void toggleBlue(){
  t_blue = !t_blue;
  if(t_blue) {
    setColor(0, 0, 255); // Blue
  } else {
    setColor(0, 0, 0);      // Off
  }
}

void toggleColor() {
  currentColor = (currentColor + 1) % numColors; // Cycle through colors
  strip.setPixelColor(0, colors[currentColor]);
  strip.show();
}

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    strip.setPixelColor(0, strip.Color(red, green, blue));
    strip.show();
}