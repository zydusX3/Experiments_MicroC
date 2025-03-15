#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Create an instance of TinyGPS++
TinyGPSPlus gps;

// Define RX and TX pins for SoftwareSerial
#define GPS_RX_PIN 6  // GPS TX → nRF52840 D6 (RX)
#define GPS_TX_PIN 7  // GPS RX → nRF52840 D7 (TX)



// Initialize SoftwareSerial
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);

void setup() {
  // Start serial communication with the PC
  Serial.begin(115200);
  while (!Serial) {
    delay(10);  // Wait for Serial to initialize
  }
  Serial.println("GPS Module Connected...");

  // Start SoftwareSerial communication with the GPS module
  gpsSerial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(D1, INPUT);
  //pinMode(D2, INPUT);
  Serial.println("Querying GPS Firmware Version...");
  gpsSerial.println("$PMTK605*31"); // Send the $PMTK605 command to request firmware version
}

void loop() {
  analogWrite(LED_BUILTIN, 0);

  /*int analogValueA1 = digitalRead(A1);
  int analogValueA2 = digitalRead(A2);
  Serial.print("Analog A1: ");
  Serial.print(analogValueA1);
  Serial.print("\tAnalog A2: ");
  Serial.println(analogValueA2);*/

  //Serial.println("Checking GPS data...");
  if (gpsSerial.available() > 0) {
    //Serial.println("GPS data available!");
    char c = gpsSerial.read(); // Read a character from GPS
    Serial.print(c);
    gps.encode(c);             // Parse the character with TinyGPS++
    // Serial.print(c);

    if (gps.satellites.isUpdated()) {
      Serial.print("Satellites: ");
      Serial.println(gps.satellites.value());
    } else {
        Serial.println("Satallite failed...");
    }
    /*analogWrite(LED_BUILTIN, 128);
    Serial.println("Get Loc");
    // Print GPS location when updated
    if (gps.location.isUpdated()) {
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
    }
    delay(100);
    analogWrite(LED_BUILTIN, 0);

    analogWrite(LED_BUILTIN, 128);
    Serial.println("Get Day");
    // Print GPS date when updated
    if (gps.date.isUpdated()) {
      Serial.print("Date: ");
      Serial.print(gps.date.month());
      Serial.print("/");
      Serial.print(gps.date.day());
      Serial.print("/");
      Serial.println(gps.date.year());
    }
    delay(100);
    analogWrite(LED_BUILTIN, 0);

    analogWrite(LED_BUILTIN, 128);
    Serial.println("Get Time");
    // Print GPS time when updated
    if (gps.time.isUpdated()) {
      Serial.print("Time: ");
      Serial.print(gps.time.hour());
      Serial.print(":");
      Serial.print(gps.time.minute());
      Serial.print(":");
      Serial.println(gps.time.second());
    }
    delay(100);
    analogWrite(LED_BUILTIN, 0);*/
  } else {
    Serial.println("No GPS data received.");
  }
  /*delay(1000);
  analogWrite(LED_BUILTIN, 0);
  delay(75);
  analogWrite(LED_BUILTIN, 255);
  delay(75);
  analogWrite(LED_BUILTIN, 0);
  delay(75);
  analogWrite(LED_BUILTIN, 255);
  delay(75);*/
}
