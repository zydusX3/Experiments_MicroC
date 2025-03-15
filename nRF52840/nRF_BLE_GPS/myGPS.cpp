#include "myGPS.h"
#include "myOLED.h"

// HardwareSerial gpsSerial(2);                                                          // Create an instance of the HardwareSerial class for Serial 2
TinyGPSPlus gps;                                                                      // Create a GPS object
static const uint32_t GPSBaud = 9600;                                                 // Define GPS Baud rate

void GPS_init() {
  // gpsSerial.begin(GPSBaud, SERIAL_8N1, RXD2, TXD2);                                   // Start Serial 2 with the defined RX and TX pins and a baud rate of 9600
  Serial1.begin(GPSBaud);                                                             // Hardware UART (D6/TX, D7/RX)
}


void turnOffGPS() {
  // gpsSerial.flush();                                                                  // Wait for any outgoing data to complete
  delay(10);                                                                          // Short delay for stability
  // gpsSerial.end();                                                                    // Stop communication with GPS module
  Serial.println("GPS turned off");
}
void turnOnGPS() {
  // gpsSerial.begin(9600);                                                           // Restart communication with GPS module
  // gpsSerial.begin(GPSBaud, SERIAL_8N1, RXD2, TXD2);
  Serial.println("GPS turned on");
}

void scanGPS_1s() {
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (Serial1.available()) {
      unsigned long start = millis();
      if (gps.encode(Serial1.read())) {
        newData = true;
      }
    }
  }
  if(newData == true) {
    newData = false;
    Serial.println(gps.satellites.value());
    print_speed();
  } else {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Data");
    display.display();
    LiveGPS = "No Data";
  }
}