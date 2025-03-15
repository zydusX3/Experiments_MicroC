#ifndef MYOLED_H
#define MYOLED_H
#include <Arduino.h>
#include "myGlobal.h"
#include <Wire.h>

void OLED_init(void);
void turnOnOLED(void);
void turnOffOLED(void);
void print_speed(void);

#endif  // MYOLED_H