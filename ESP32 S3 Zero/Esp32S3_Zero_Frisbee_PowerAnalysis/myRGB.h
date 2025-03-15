#ifndef MYRGB_H
#define MYRGB_H
#include <Arduino.h>
#include "myGlobal.h"
#include <Adafruit_NeoPixel.h>

// extern bool led_st;
void setColor(uint8_t red, uint8_t green, uint8_t blue);
void RGBControl(char BLEchar);
void toggleRed(void);

#endif  // MYRGB_H