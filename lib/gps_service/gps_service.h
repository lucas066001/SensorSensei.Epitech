#ifndef GPS_SERVICE_H
#define GPS_SERVICE_H

#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define GPS_BAUD 9600
#define RX_PIN 12
#define TX_PIN 34

extern TinyGPSPlus gps;
extern HardwareSerial gpsSerial;

void setup_gps();
void loop_gps();
void displayInfo();

#endif
