#include <TinyGPSPlus.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 4         // Define custom SDA pin
#define SCL_PIN 5         // Define custom SCL pin
#define SCREEN_WIDTH 128  // Adjust based on your OLED's width
#define SCREEN_HEIGHT 64  // Adjust based on your OLED's height
#define RXD 7             // Define the RX pins for Serial 1
#define TXD 6             // Define the TX pins for Serial 1
#define GPSBaud 9600      // Hardware UART (RX/D7, TX/D6)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
TinyGPSPlus gps;
String myGPS = "19.99, 20.22";              // GPS variable to be Notified in BLE
String lastResult = "";                     // To track the last value
String LiveGPS = "";                        // Store GPS Data here


void setup() {
  Serial.begin(115200);
  Serial1.begin(GPSBaud); // Hardware UART (D6/TX, D7/RX)
  OLED_init();
}

void loop() {
  scanGPS_1s();
}

void OLED_init(void) {
  Wire.begin(); //(SDA_PIN, SCL_PIN);         // Initialize I2C with custom pins
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
}

void scanGPS_1s() {
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (Serial1.available()) {
      unsigned long start = millis();
      if (gps.encode(Serial1.read())) {
        newData = true;
      }
    }
  }
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