#ifndef MYBLE_H
#define MYBLE_H
#include <Arduino.h>
#include "myGlobal.h"
#include <bluefruit.h>

extern BLECharacteristic buttonCharacteristic;

void ble_init(void);
void ble_config(void);
void turnOnBLE(void);
void turnOffBLE(void);
void scanBLE(void);

#endif  // MYBLE_H