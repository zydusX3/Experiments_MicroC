#include "myOLED.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);                     // Create an SSD1306 object

void OLED_init(void) {
  //Wire.begin(SDA_PIN, SCL_PIN);         // Initialize I2C with custom pins
  Wire.begin();                           // Initialize I2C with default pins (SDA_PIN 4, SCL_PIN 5)
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

void turnOnOLED() {
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.display();                           // Refresh screen content
  Serial.println("OLED turned on");
}

void turnOffOLED() {
  display.ssd1306_command(SSD1306_DISPLAYOFF);
  Serial.println("OLED turned off");
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