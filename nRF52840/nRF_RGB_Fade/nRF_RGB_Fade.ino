#include <Adafruit_TinyUSB.h>

Adafruit_USBD_CDC usb_serial;

#define LED_RED   11   // Predefined in variant.h
#define LED_GREEN 13   // Predefined in variant.h
#define LED_BLUE  12   // Predefined in variant.h

void setup() {
  Serial.begin(115200);
    usb_serial.begin(115200);
    // ⏳ Timeout after 5 seconds
    unsigned long startMillis = millis();
    while (!usb_serial && millis() - startMillis < 5000) {
      delay(100);
    }

    if (usb_serial) {
        usb_serial.println("USB Serial is active!");
    } // else {
        // Continue running even if USB is not connected
    // }

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
}

void loop() {
    if (usb_serial) {
      usb_serial.println("USB Serial is connected.");
    } // else {
    //     // Handle USB not being connected
    // }

    fadeLED(LED_RED, 255, 1000);   // Fade in Red
    fadeLED(LED_RED, 0, 1000);     // Fade out Red

    fadeLED(LED_GREEN, 255, 1000); // Fade in Green
    fadeLED(LED_GREEN, 0, 1000);   // Fade out Green

    fadeLED(LED_BLUE, 255, 1000);  // Fade in Blue
    fadeLED(LED_BLUE, 0, 1000);    // Fade out Blue
}

void fadeLED(int pin, int brightness, int duration) {
    int step = (brightness > 0) ? 5 : -5;  // Step up or down
    for (int i = 0; i != brightness; i += step) {
        analogWrite(pin, i);
        delay(duration / (brightness / abs(step)));  // Smooth transition
    }
    if (usb_serial) {                   // Not working
      usb_serial.print("LED on pin ");
      usb_serial.print(pin);
      usb_serial.print(" set to ");
      usb_serial.println(brightness);
    }
    if(Serial) {                        // work only if USB_serial is defined
      Serial.print("S LED on pin ");
      Serial.print(pin);
      Serial.print(" set to ");
      Serial.println(brightness);
    }
}
