#ifndef MYFUNCIONS_H
#define MYFUNCIONS_H
#include "HardwareSerial.h"
#include "myGlobal.h"
#include "myBlue.h"

// extern void IRAM_ATTR buttonISR();
void my_init(void);
void serialDBG(bool enable);
void onButtonPress(void);
void CheckBtnState(void);

#endif  // MYFUNCIONS_H