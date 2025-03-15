#ifndef MYGPS_H
#define MYGPS_H
#include <Arduino.h>
#include "myGlobal.h"

void GPS_init(void);
void turnOffGPS(void);
void turnOnGPS(void);
void scanGPS_1s(void);

#endif  // MYGPS_H