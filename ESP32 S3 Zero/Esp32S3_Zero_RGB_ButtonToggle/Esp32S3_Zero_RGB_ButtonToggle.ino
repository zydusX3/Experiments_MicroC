#include <Adafruit_NeoPixel.h>

#define LED_PIN    21  // GPIO pin connected to the RGB LED
#define NUM_LEDS   1   // Number of LEDs
#define BUTTON_PIN 2   // GPIO pin connected to the button

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

bool buttonState = LOW;   // Current state of the button
bool lastButtonState = LOW; // Previous state of the button
unsigned long lastDebounceTime = 0; // The last time the button was toggled
unsigned long debounceDelay = 50;  // Debounce delay in milliseconds
int currentColor = 0; // Tracks the current color index

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

void setup() {
    Serial.begin(9600);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        toggleColor();
      }
    }
  }
  // Save the reading as the last state
  lastButtonState = reading;
    // // Cycle through some colors
    // setColor(255, 0, 0); // Red
    // delay(1000);
    // setColor(0, 255, 0); // Green
    // delay(1000);
    // setColor(0, 0, 255); // Blue
    // delay(1000);
    // setColor(255, 255, 255); // White
    // delay(1000);
    // setColor(0, 0, 0); // Off
    // delay(1000);
}

void toggleColor() {
  currentColor = (currentColor + 1) % numColors; // Cycle through colors
  strip.setPixelColor(0, colors[currentColor]);
  strip.show();
}

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
    strip.setPixelColor(0, strip.Color(red, green, blue));
    strip.show();
}
