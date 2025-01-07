#include "Oled_Disp.h"
#include <Arduino.h>

#include<Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define OLED display width and height
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create an SSD1306 display object
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

void i2cScanner() {
  Serial.println("I2C Scanner");
  for (byte address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C Device at: 0x");
      Serial.println(address, HEX);
    }
    //toggleLed();
  }
}

void initOLED() {
  // Initialize OLED display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();               // Clear the buffer
  display.setTextSize(1);               // Set text size
  display.setTextColor(SSD1306_WHITE);  // Set text color
}

void oledDispString(String message) {
  display.clearDisplay();
  display.setCursor(0, 10); // X, Y
  display.println(message);
  display.display();
}

#define LeftEyeX 32
#define LeftEyeY 20
#define RightEyeX 96
#define RightEyeY 20
#define EyeRadius 10

/*
0 - Both
1 - Left
2 - Right
*/
void eyes_0(int a) {
  if(a==0 || a==1) { display.fillCircle(LeftEyeX, LeftEyeY, EyeRadius, WHITE); }
  if(a==0 || a==2) { display.fillCircle(RightEyeX, RightEyeY, EyeRadius, WHITE); }
  display.display();
}

void eyes_1(int a) {
  display.fillCircle(LeftEyeX, LeftEyeY, EyeRadius, WHITE); if(a==0 || a==1) { display.fillCircle(LeftEyeX, LeftEyeY-10, EyeRadius, BLACK); }
  display.fillCircle(RightEyeX, RightEyeY, EyeRadius, WHITE); if(a==0 || a==2) { display.fillCircle(RightEyeX, RightEyeY-10, EyeRadius, BLACK); }
  display.display();
}

void eyes_2(int a) {
  display.fillCircle(LeftEyeX, LeftEyeY, EyeRadius, WHITE); if(a==0 || a==1) { display.fillCircle(LeftEyeX, LeftEyeY-5, EyeRadius, BLACK); }
  display.fillCircle(RightEyeX, RightEyeY, EyeRadius, WHITE); if(a==0 || a==2) { display.fillCircle(RightEyeX, RightEyeY-5, EyeRadius, BLACK); }
  display.display();
}

void eyes_3(int a) {
  display.fillCircle(LeftEyeX, LeftEyeY, EyeRadius, WHITE); if(a==0 || a==1) { display.fillCircle(LeftEyeX, LeftEyeY-1, EyeRadius, BLACK); }
  display.fillCircle(RightEyeX, RightEyeY, EyeRadius, WHITE); if(a==0 || a==2) { display.fillCircle(RightEyeX, RightEyeY-1, EyeRadius, BLACK); }
  display.display();
}

void wink() {
  eyes_0(0);  delay(100);
  eyes_1(2);  delay(50);
  eyes_2(2);  delay(50);
  eyes_3(2);  delay(100);
  eyes_2(2);  delay(50);
  eyes_1(2);  delay(50);
  eyes_0(0);  delay(100);
}

void blink() {
  eyes_0(0);  delay(100);
  eyes_1(0);  delay(50);
  eyes_2(0);  delay(50);
  eyes_3(0);  delay(100);
  eyes_2(0);  delay(50);
  eyes_1(0);  delay(50);
  eyes_0(0);  delay(100);
}
//////////////////////////////////////////////////////////////////////////////
void drawFace() {
  display.drawRoundRect(0, 0, 128, 64, 8, WHITE);
  display.display();
}