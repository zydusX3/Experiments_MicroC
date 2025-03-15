#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 4         // Define custom SDA pin
#define SCL_PIN 5         // Define custom SCL pin
#define SCREEN_WIDTH 128  // Adjust based on your OLED's width
#define SCREEN_HEIGHT 64  // Adjust based on your OLED's height

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  OLED_init();
}

void loop() {
  // put your main code here, to run repeatedly:

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