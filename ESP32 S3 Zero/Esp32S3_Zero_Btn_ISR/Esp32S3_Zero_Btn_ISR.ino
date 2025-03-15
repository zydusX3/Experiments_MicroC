
#define BUTTON_PIN 7                        // GPIO pin connected to the button
bool btn_st = false;                        // Button state T/F
int lastISRtime = 0;                        // last valid ISR trigger time
int ISRminDelay = 1000;                     // Minimum delay between ISR trigger

void IRAM_ATTR buttonISR() {
  Serial.println("BTN delay: " + String((millis() - lastISRtime)));
  if ((millis() - lastISRtime) > ISRminDelay) {
    lastISRtime = millis();
    btn_st = !btn_st; // Toggle the button's logical state
    Serial.println("BTN State: " + String(btn_st));
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<10;i++) {
    Serial.println(i);
    delay(1000);
  }
  Serial.println("LOop...");
}
