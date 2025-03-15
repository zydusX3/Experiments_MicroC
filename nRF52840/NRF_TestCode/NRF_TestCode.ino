#include<SoftwareSerial.h>

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    Serial.write("Hi");
  }
  digitalWrite(LED_BUILTIN, HIGH);
  if (Serial.available()) {
    Serial.println("HiGH");
  }
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  if (Serial.available()) {
    Serial.println("LoW");
  }
  delay(500);

}
