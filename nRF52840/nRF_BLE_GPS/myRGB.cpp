#include "myRGB.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, Ring_RGB_PIN, NEO_GRB + NEO_KHZ800); // Create a NeoPixel object
bool t_red = false;
bool led_st = false;

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    // strip.setPixelColor(0, strip.Color(red, green, blue));
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
}

void nRF_setColor(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(LED_RED, 255-red);
  analogWrite(LED_GREEN, 255-green);
  analogWrite(LED_BLUE, 255-blue);
}

void RGBControl(char BLEchar) {
  switch (BLEchar) {
    case '1': Serial.println("LED on");
              nRF_setColor(255, 255, 255);
              setColor(255, 255, 255);
              // digitalWrite(LED_PIN, HIGH);
              break;
    case 'r':
    case 'R': Serial.println("RGB R");
              nRF_setColor(255, 0, 0);
              setColor(255, 0, 0);
              break;
    case 'g':
    case 'G': Serial.println("RGB G");
              nRF_setColor(0, 255, 0);
              setColor(0, 255, 0);
              break;
    case 'b':
    case 'B': Serial.println("RGB B");
              nRF_setColor(0, 0, 255);
              setColor(0, 0, 255);
              break;
    case 'y':
    case 'Y': Serial.println("RGB Y");
              nRF_setColor(255, 255, 0);
              setColor(255, 255, 0);
              break;
    case 'c':
    case 'C': Serial.println("RGB C");
              nRF_setColor(0, 255, 255);
              setColor(0, 255, 255);
              break;
    case 'm':
    case 'M': Serial.println("RGB M");
              nRF_setColor(255, 0, 255);
              setColor(255, 0, 255);
              break;
    default:  Serial.println("LED off");
              nRF_setColor(0, 0, 0);
              setColor(0, 0, 0);
              // digitalWrite(LED_PIN, LOW);
              break;
  }
}

void GRBControl(char BLEchar) {
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