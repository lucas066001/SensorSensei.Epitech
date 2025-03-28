#include "gps_service.h"

TinyGPSPlus gps;                 // Define GPS object
HardwareSerial gpsSerial(2);      // Define HardwareSerial object

void setup_gps() {
    Serial.begin(115200);
    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, TX_PIN, RX_PIN);

    // Configure GPS module
    // gpsSerial.println("$PMTK225,0*2B");  // Activate GPS
    // gpsSerial.println("$PMTK220,1000*1F"); // Fix every second
    // gpsSerial.println("$PMTK300,1000,0,0,0,0*1C"); // Normal mode
    // gpsSerial.println("$PMTK314,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1*29"); // Enable all NMEA sentences
}

void loop_gps() {
    while (gpsSerial.available() > 0) {
        char c = gpsSerial.read();
        Serial.write(c);  // Display raw GPS data

        if (gps.encode(c)) {
            displayInfo();
        }
    }

    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());
}

void displayInfo() {
    if (gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
        Serial.print("Altitude: ");
        Serial.println(gps.altitude.meters());
    } else {
        Serial.println("Location: Not Available");
    }

    Serial.print("Date: ");
    if (gps.date.isValid()) {
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.print(gps.date.day());
        Serial.print("/");
        Serial.println(gps.date.year());
    } else {
        Serial.println("Not Available");
    }

    Serial.print("Time: ");
    if (gps.time.isValid()) {
        if (gps.time.hour() < 10) Serial.print("0");
        Serial.print(gps.time.hour());
        Serial.print(":");
        if (gps.time.minute() < 10) Serial.print("0");
        Serial.print(gps.time.minute());
        Serial.print(":");
        if (gps.time.second() < 10) Serial.print("0");
        Serial.print(gps.time.second());
        Serial.print(".");
        if (gps.time.centisecond() < 10) Serial.print("0");
        Serial.println(gps.time.centisecond());
    } else {
        Serial.println("Not Available");
    }

    Serial.println();
    Serial.println();
    delay(1000);
}
