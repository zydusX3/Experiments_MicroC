#ifndef MYBLUE_H
#define MYBLUE_H
#include <Arduino.h>
#include "myGlobal.h"
#include <ArduinoBLE.h>

extern BLEByteCharacteristic buttonCharacteristic;

void ble_init(void);
void ble_config(void);
void turnOnBLE(void);
void turnOffBLE(void);
void scanBLE(void);

#endif  // MYBLUE_H