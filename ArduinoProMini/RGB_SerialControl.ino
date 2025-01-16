#include <FastLED.h>

#define LED_PIN     7       // Pin connected to the data pin of the LED strip
#define NUM_LEDS    25      // Number of LEDs in the strip
#define BRIGHTNESS  100     // Set brightness (0-255)
#define LED_TYPE    WS2812B // LED type (e.g., WS2812B, WS2811)
#define COLOR_ORDER GRB     // Color order (most common is GRB)

CRGB leds[NUM_LEDS]; // Define an array to hold the LED data

void setup() {
  Serial.begin(9600);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.clear(); // Turn off all LEDs initially
    FastLED.show();
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    // rainbow(); // Example: Display a rainbow effect
    if (Serial.available() > 0) {
      String inRX = Serial.readStringUntil('\n'); // Read until newline character
      inRX.trim();
      colorSelect(inRX.c_str());
    }
    delay(100);
}

void rainbow() {
    static uint8_t hue = 0; // Starting hue
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(hue + (i * 10), 255, 255); // Create a rainbow effect
    }
    FastLED.show();
    hue++; // Update the hue for the next frame
    delay(20);
}

void setColor(CRGB color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = color;
    }
    FastLED.show();
}

void colorSelect(const char* inColor) {
    Serial.println(inColor);
    if (strcmp(inColor, "rainbow") == 0) {
        rainbow(); // Call the rainbow effect function
    } else if (strcmp(inColor, "red") == 0) {
        setColor(CRGB::Red); // Set all LEDs to red
    } else if (strcmp(inColor, "blue") == 0) {
        setColor(CRGB::Blue); // Set all LEDs to blue
    } else if (strcmp(inColor, "green") == 0) {
        setColor(CRGB::Green); // Set all LEDs to green
    } else {
        // Default color (optional)
        setColor(CRGB::Black); // Turn off all LEDs
    }
}
