#include "myFunctions.h"
#include <Arduino.h>

bool a = false;

void toggleLed() {
  a = !a;
  if(a) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}