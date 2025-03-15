#include <Adafruit_NeoPixel.h>

#define Ring_RGB_PIN  0     // Control RGB LED strip
#define NUM_LEDS      25    // Number of LEDs

int i = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, Ring_RGB_PIN, NEO_GRB + NEO_KHZ800); // Create a NeoPixel object

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    // strip.setPixelColor(0, strip.Color(red, green, blue));
    for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
}

void setup() {
  Serial.begin(115200);
  pinMode(Ring_RGB_PIN, OUTPUT);
}

void loop() {
  // loop_loneRunner();
  // loop_tailRunner(3);
  rainbowCycle(50);
}

void loop_loneRunner() {
  while(i <= NUM_LEDS) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
    strip.show();
    if(i==0)
      strip.setPixelColor(NUM_LEDS-1, strip.Color(0, 0, 0));
    else
      strip.setPixelColor(i-1, strip.Color(0, 0, 0));
    Serial.println(i);
    i++;
    strip.show();
    if(i == NUM_LEDS)
      i = 0;
    delay(500);
  }
}

void loop_tailRunner(int bodyLen) {
  while(i <= NUM_LEDS) {
    // strip.setPixelColor(i, strip.Color(255, 0, 0));
    for (int n = 0; n < NUM_LEDS; n++) {
      strip.setPixelColor(n, strip.Color(0, 0, 0));
    }
    strip.show();
    // if(i>=(bodyLen-1)){
      for(int n=i; n>(i-bodyLen); n--) {
        if(n<0) {
          strip.setPixelColor((NUM_LEDS+n), strip.Color(255, 0, 0));
        } else {
          strip.setPixelColor(n, strip.Color(255, 0, 0));
        }
        Serial.println(n);
      }
    // }
    strip.show();
    // if(i==0)
    //   strip.setPixelColor(NUM_LEDS-1, strip.Color(0, 0, 0));
    // else
    //   strip.setPixelColor(i-1, strip.Color(0, 0, 0));
    i++;
    // strip.show();
    if(i == NUM_LEDS) {
      i = 0;
    }
    delay(500);
  }
}

// Function to create a moving rainbow effect
void rainbowCycle(int wait) {
    strip.setBrightness(50); // Range 0-255
    uint16_t i, j;

    for (j = 0; j < 256; j++) {  // Full cycle of rainbow colors
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels() + j) & 255));
        }
        strip.show();
        delay(wait); // Adjust speed
    }
}

// Function to generate rainbow colors
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}