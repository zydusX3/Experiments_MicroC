#include <Adafruit_TinyUSB.h>
#define BUTTON_PIN    3     // GPIO pin connected to the button

volatile  bool ISR_flag = false;
volatile  int lastISRtime = 0;                        // last valid ISR trigger time
const int ISRminDelay = 1000;                     // Minimum delay between ISR trigger
bool btn_st = false;                        // initial Button state

// void buttonISR();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  delay(200);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);  // attach ISR check to pin 7
}

void buttonISR() {            // Interrupt Service Routine
  unsigned long currentTime = millis();
  if ((currentTime - lastISRtime) > ISRminDelay) {
    lastISRtime = currentTime;
    ISR_flag = true;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(ISR_flag) {
    Serial.println("ISR ISR ISR ...!");
    ISR_flag = false;  // Reset flag after processing
  }
  delay(100);
}
