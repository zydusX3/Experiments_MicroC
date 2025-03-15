#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>

#define RGB_PIN    21     // GPIO pin connected to the RGB LED
#define NUM_LEDS   1      // Number of LEDs
#define BUTTON_PIN 7      // GPIO pin connected to the button
// #define LED_PIN    6      // GPIO pin connected to the LED
#define SCREEN_WIDTH 128  // Adjust based on your OLED's width
#define SCREEN_HEIGHT 64  // Adjust based on your OLED's height
#define RXD2 12           // Define the RX pins for Serial 2
#define TXD2 11           // Define the TX pins for Serial 2
#define SDA_PIN 3         // Define custom SDA pin
#define SCL_PIN 2         // Define custom SCL pin
#define LoopMax 5         // Max number of loops to be timed

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800); // Create a NeoPixel object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);                     // Create an SSD1306 object
TinyGPSPlus gps;                                                                      // Create a GPS object
HardwareSerial gpsSerial(2);                                                          // Create an instance of the HardwareSerial class for Serial 2

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic ledCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic buttonCharacteristic("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BLEStringCharacteristic functionResultCharacteristic("19B10013-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 50);

bool t_red = false;
bool led_st = false, btn_st = false;        // initial led and Button state
bool lastButtonState = HIGH;                // Assume button is unpressed at startup
bool buttonState = HIGH;                    // Current button state
unsigned long lastDebounceTime = 0;         // The last time the button was toggled
unsigned long debounceDelay = 50;           // Debounce delay in milliseconds
static const uint32_t GPSBaud = 9600;       // Define GPS Baud rate
String myGPS = "19.99, 20.22";              // GPS variable to be Notified in BLE
String lastResult = "";                     // To track the last value
String LiveGPS = "";                        // Store GPS Data here
int loopcount = 0;                          // Loop counter for Time estimation
int lastISRtime = 0;                        // last valid ISR trigger time
int ISRminDelay = 1000;                     // Minimum delay between ISR trigger

void IRAM_ATTR buttonISR() {
  // Interrupt Service Routine
  if ((millis() - lastISRtime) > ISRminDelay) {
    lastISRtime = millis();
    buttonCharacteristic.writeValue(btn_st);
    onButtonPress();
  }
}

void setup() {
  Serial.begin(115200);
  // pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setColor(255,255,255);                // White
  Wire.begin(SDA_PIN, SCL_PIN);         // Initialize I2C with custom pins
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C is the default I2C address for most OLEDs
    Serial.println("OLED initialization failed!");
    while (true);
  }
  Serial.println("OLED initialized successfully!");
  display.clearDisplay();               // Clear the display buffer
  display.setTextSize(1);               // Text size: 1 = small, 2 = medium, etc.
  display.setTextColor(SSD1306_WHITE);  // White text
  display.setCursor(0, 0);              // Start at the top-left corner
  display.println("Hello GUYs!");       // Display "Hello GUYs"
  display.display();                    // Push the buffer to the screen
  Serial.println("Display GPS data on OLED");
  setColor(0, 255, 0);                  // Red
  gpsSerial.begin(GPSBaud, SERIAL_8N1, RXD2, TXD2); // Start Serial 2 with the defined RX and TX pins and a baud rate of 9600
  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1) {
      toggleRed();                      // Blink Red
    }
  }
  setColor(255, 0, 0);                  // Green
  BLE.setLocalName("Frisbee");          // Set the local name peripheral advertises
  BLE.setAdvertisedService(ledService); // Set the UUID for the service this peripheral advertises:
  // Add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);
  ledService.addCharacteristic(buttonCharacteristic);
  ledService.addCharacteristic(functionResultCharacteristic);
  // Add the service
  BLE.addService(ledService);
  ledCharacteristic.writeValue(led_st);
  buttonCharacteristic.writeValue(btn_st);
  functionResultCharacteristic.writeValue("Ready");
  BLE.advertise();                      // Start advertising
  Serial.println("Bluetooth® device active, waiting for connections...");
  setColor(0, 0, 255);                  // Blue
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);
}

void loop() {
  // if(loopcount<LoopMax) Serial.println(millis());
  BLE.poll();   // poll for Bluetooth® Low Energy events
  // if(loopcount<LoopMax) Serial.println(millis());
  if (Serial.available()){
    myGPS = Serial.readString();
    Serial.println("Updated GPS is: "+myGPS);
  }
  // if(loopcount<LoopMax) Serial.println(millis());
  // bool reading = digitalRead(BUTTON_PIN);
  // if (reading != lastButtonState) {
  //   lastDebounceTime = millis();
  // }
  // if ((millis() - lastDebounceTime) > debounceDelay) {
  //   if (reading != buttonState) {
  //     buttonCharacteristic.writeValue(btn_st);
  //     buttonState = reading;
  //     if (buttonState == LOW) {
  //       btn_st = !btn_st; // Toggle the button's logical state
  //       Serial.println("BTN State: " + String(btn_st));
  //       onButtonPress();
  //     }
  //   }
  // }
  // lastButtonState = reading;
  // if(loopcount<LoopMax) Serial.println(millis());
  //  Notify if GPS Change
  if (functionResultCharacteristic.subscribed()) {
    if (myGPS != lastResult) {
      lastResult = myGPS;
      functionResultCharacteristic.writeValue(myGPS);
      Serial.println("Sent function result via notification: " + myGPS);
    }
  }
  // if(loopcount<LoopMax) Serial.println(millis());
  //  RGB control
  if (ledCharacteristic.written()) {
    Serial.print("ledCharacteristic = ");
    char charBLE = ledCharacteristic.value();
    Serial.println(charBLE);
    RGBControl(charBLE);
  }
  // if(loopcount<LoopMax) Serial.println(millis());

  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (gpsSerial.available()) {
      unsigned long start = millis();
      if (gps.encode(gpsSerial.read())) {
        newData = true;
      }
    }
  }
  // if(loopcount<LoopMax) Serial.println(millis());
  if(newData == true) {
    newData = false;
    Serial.println(gps.satellites.value());
    print_speed();
  } else {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Data");
    display.display();
    LiveGPS = "No Data";
  }
  // if(loopcount<LoopMax) Serial.println(millis());
  loopcount++;
  delay(10);
}

////////////////////////////////////////////////////////////////////////////////////////

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    strip.setPixelColor(0, strip.Color(red, green, blue));
    strip.show();
}

void RGBControl(char BLEchar) {
  switch (BLEchar) {
      case '1': Serial.println("LED on");
                // digitalWrite(LED_PIN, HIGH);
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
                  // digitalWrite(LED_PIN, LOW);
                  break;
    }
}

void onButtonPress() {
  // Button Press execution here
  myGPS = LiveGPS;
}

void toggleRed(){
  t_red = !t_red;
  if(t_red) {
    setColor(255, 0, 0); // Red
  } else {
    setColor(0, 0, 0);     // Off
  }
}

void print_speed() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
       
  if (gps.location.isValid() == 1) {
   //String gps_speed = String(gps.speed.kmph());
    display.setTextSize(1);
    
    display.setCursor(25, 5);
    display.print("Lat: ");
    display.setCursor(50, 5);
    display.print(gps.location.lat(),6);
 
    display.setCursor(25, 20);
    display.print("Lng: ");
    display.setCursor(50, 20);
    display.print(gps.location.lng(),6);
 
    display.setCursor(25, 35);
    display.print("Speed: ");
    display.setCursor(65, 35);
    display.print(gps.speed.kmph());
    
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("SAT:");
    display.setCursor(25, 50);
    display.print(gps.satellites.value());
 
    display.setTextSize(1);
    display.setCursor(70, 50);
    display.print("ALT:");
    display.setCursor(95, 50);
    display.print(gps.altitude.meters(), 0);
 
    display.display();
    LiveGPS = String(gps.location.lat(), 6) +", "+ String(gps.location.lng(), 6);
  } else {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Data");
    display.display();
    LiveGPS = "No Data";
  }
}