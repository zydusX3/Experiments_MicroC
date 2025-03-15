#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>

#define RGB_PIN    21   // GPIO pin connected to the RGB LED
#define NUM_LEDS   1    // Number of LEDs
#define BUTTON_PIN 2    // GPIO pin connected to the button
#define LED_PIN    6    // GPIO pin connected to the LED

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

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

bool t_red = false;
int currentColor = 0;                       // Tracks the current color index

// const int ledPin = LED_PIN;                 // set ledPin to on-board LED
// const int buttonPin = BUTTON_PIN;           // set buttonPin to digital pin 4

bool led_st = false, btn_st = false;                // initial led and Button state
bool lastButtonState = HIGH;                // Assume button is unpressed at startup
bool buttonState = HIGH;                    // Current button state

unsigned long lastDebounceTime = 0;         // The last time the button was toggled
unsigned long debounceDelay = 50;           // Debounce delay in milliseconds

String myGPS = "19.99,20.22";
String lastResult = "";                     // To track the last value

unsigned long lastNotifyTime = 0;           // To track the last notification time
const unsigned long notifyInterval = 1000;  // Interval in milliseconds

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEStringCharacteristic functionResultCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 50);

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setColor(255,255,255);
  // BLE
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1) {
      toggleRed();
    }
  }
  setColor(0, 255, 0);  // Green
  BLE.setLocalName("Frisbee");  // set the local name peripheral advertises
  BLE.setAdvertisedService(ledService); // set the UUID for the service this peripheral advertises:
  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  ledService.addCharacteristic(buttonCharacteristic);
  ledService.addCharacteristic(functionResultCharacteristic);
  // add the service
  BLE.addService(ledService);
  ledCharacteristic.writeValue(led_st);
  buttonCharacteristic.writeValue(btn_st);
  functionResultCharacteristic.writeValue("Ready");
  BLE.advertise();  // start advertising
  Serial.println("Bluetooth® device active, waiting for connections...");
  setColor(0, 0, 255);  // Blue
}

void loop() {
  BLE.poll();   // poll for Bluetooth® Low Energy events
  if (Serial.available()){
    myGPS = Serial.readString();
    Serial.println("Updated GPS is: "+myGPS);
  }
  bool reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonCharacteristic.writeValue(btn_st);
      buttonState = reading;
      if (buttonState == LOW) {
        btn_st = !btn_st; // Toggle the button's logical state
        Serial.println("BTN State: " + String(btn_st));
        onButtonPress();
      }
    }
  }
  lastButtonState = reading;
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
    Serial.println(ledCharacteristic.value());
    // update LED, either central has written to characteristic or button state has changed
    char charBLE = ledCharacteristic.value();
    switch (charBLE) {
      case '1': Serial.println("LED on");
                digitalWrite(LED_PIN, HIGH);
                break;
      case 'r':
      case 'R': Serial.println("RGB R");
                setColor(0, 255, 0);
                break;
      case 'g':
      case 'G': Serial.println("RGB G");
                setColor(255, 0, 0);
                break;
      case 'b':
      case 'B': Serial.println("RGB B");
                setColor(0, 0, 255);
                break;
      case 'y':
      case 'Y': Serial.println("RGB Y");
                setColor(255, 255, 0);
                break;
      case 'c':
      case 'C':  Serial.println("RGB C");
                  setColor(255, 0, 255);
                  break;
      case 'm':
      case 'M':  Serial.println("RGB M");
                  setColor(0, 255, 255);
                  break;
      default:    Serial.println("LED off");
                  digitalWrite(LED_PIN, LOW);
                  break;
    }
  }
  delay(50);
}



void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    strip.setPixelColor(0, strip.Color(red, green, blue));
    strip.show();
}

void onButtonPress() {
  // Button Press execution here
  myGPS = "GPS updated on Button press";
}

void toggleRed(){
  t_red = !t_red;
  if(t_red) {
    setColor(255, 0, 0); // Red
  } else {
    setColor(0, 0, 0);     // Off
  }
}