#include<ESP8266WiFi.h>

const char* ssid = "Improvians";
const char* password = "ImproviansA1+";
bool a = false;

void toggleLed() {
  a = !a;
  if(a) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void initWifi() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println();
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    toggleLed();
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
}

void blinkLED(int a) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(a);
  digitalWrite(LED_BUILTIN, LOW);
  delay(a);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  initWifi();
}

void loop() {
  blinkLED(200);
}
