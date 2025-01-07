#include "myWifi.h"
#include <Arduino.h>

#include<ESP8266WiFi.h>

const char* ssid = "Improvians";
const char* password = "ImproviansA1+";

void initWifi() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println();
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    //toggleLed();
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
}