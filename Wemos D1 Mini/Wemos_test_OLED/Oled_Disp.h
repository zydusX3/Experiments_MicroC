#ifndef OLED_DISP_H
#define OLED_DISP_H
#include <Arduino.h>
// Function declarations
void i2cScanner();
void initOLED();
void oledDispString(String message);
void wink();
void drawFace();

#endif // OLED_DISP_H