/*
    No inbult RGB, onlu one USER led (red)
    Fading working
*/

#include <SoftwareSerial.h>

#define RED 17
#define GREEN 16
#define BLUE 18

void RedOn() {
  Serial.println("RedOn...");
  digitalWrite(17, LOW);
  digitalWrite(16, HIGH);
  digitalWrite(18, HIGH);
  delay(1000);
}

void GreenOn() {
  Serial.println("GreenOn...");
  digitalWrite(17, HIGH);
  digitalWrite(16, LOW);
  digitalWrite(18, HIGH);
  delay(1000);
}

void BlueOn() {
  Serial.println("BlueOn...");
  digitalWrite(17, HIGH);
  digitalWrite(16, HIGH);
  digitalWrite(18, LOW);
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop() {
  //RedOn();
  //GreenOn();
  //BlueOn();
  Serial.println("Bright...");
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(LED_BUILTIN, brightness); // Adjust brightness
    delay(10);                            // Wait a bit
  }
  Serial.println("Fade...");
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(LED_BUILTIN, brightness); // Adjust brightness
    delay(10);                            // Wait a bit
  }
}
