#include "myRGB.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800); // Create a NeoPixel object
bool t_red = false;
bool led_st = false;

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    strip.setPixelColor(0, strip.Color(red, green, blue));
    strip.show();
}

void RGBControl(char BLEchar) {
  switch (BLEchar) {
    case '1': Serial.println("LED on");
              setColor(255, 255, 255);
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
    case 'C': Serial.println("RGB C");
              setColor(255, 0, 255);
              break;
    case 'm':
    case 'M': Serial.println("RGB M");
              setColor(0, 255, 255);
              break;
    default:  Serial.println("LED off");
              setColor(0, 0, 0);
              // digitalWrite(LED_PIN, LOW);
              break;
  }
}

void toggleRed() {
  t_red = !t_red;
  if(t_red) {
    setColor(255, 0, 0); // Red
  } else {
    setColor(0, 0, 0);     // Off
  }
}