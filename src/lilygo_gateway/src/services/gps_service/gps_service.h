#ifndef GPS_SERVICE_H
#define GPS_SERVICE_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Pin definitions for GPS
#define RX_PIN 12
#define TX_PIN 34
#define GPS_BAUD 9600

extern TinyGPSPlus gps;
extern HardwareSerial gpsSerial;  // Declared here, defined in .cpp

void setup_gps();
void loop_gps();
void displayInfo();

#endif
