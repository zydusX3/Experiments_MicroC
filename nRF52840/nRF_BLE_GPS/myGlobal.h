#ifndef MYGLOBAL_H
#define MYGLOBAL_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>

#define RGB_PIN_R     11    // GPIO pin connected to the RGB LED
#define RGB_PIN_G     13    // GPIO pin connected to the RGB LED
#define RGB_PIN_B     12    // GPIO pin connected to the RGB LED
#define Ring_RGB_PIN  0     // Control RGB LED strip
#define NUM_LEDS      25    // Number of LEDs
#define BUTTON_PIN    3     // GPIO pin connected to the button
// #define LED_PIN       8     // GPIO pin connected to the LED
// #define BUTTON_OLED   4     // GPIO pin to Toggle OLED on/off
// #define BUTTON_GPS    5     // GPIO pin to Toggle GPS on/off
// #define BUTTON_BLE    6     // GPIO pin to Toggle BLE on/off
#define SCREEN_WIDTH  128   // Adjust based on your OLED's width
#define SCREEN_HEIGHT 64    // Adjust based on your OLED's height
#define RXD2          7     // Define the RX pins for Serial 2
#define TXD2          6     // Define the TX pins for Serial 2
#define SDA_PIN       4     // Define SDA pin
#define SCL_PIN       5     // Define SCL pin
#define LoopMax       5     // Max number of loops to be timed

extern bool ISR_flag;
extern bool dbg_serial;   // Enable or disable Serial print
extern String myGPS;      // GPS variable to be Notified in BLE
extern String lastResult; // To track the last value
extern String LiveGPS;    // Store GPS Data here

extern bool btn_st;       // initial Button state
extern bool led_st;       // initial Button state
// bool lastButtonState = HIGH;                // Assume button is unpressed at startup
// bool buttonState = HIGH;                    // Current button state
// unsigned long lastDebounceTime = 0;         // The last time the button was toggled
// unsigned long debounceDelay = 50;           // Debounce delay in milliseconds

extern bool OLED_state;
extern bool GPS_state;
extern bool BLE_state;

extern TinyGPSPlus gps;
extern Adafruit_SSD1306 display;

#endif  // MYGLOBAL_H