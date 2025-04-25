#include "gps_service.h"

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

void setup_gps() {
    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, TX_PIN, RX_PIN);
}

void loop_gps() {
    while (gpsSerial.available() > 0) {
        char c = gpsSerial.read();
        if (gps.encode(c)) {
            displayInfo();
        }
    }
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
        Serial.printf("%02d/%02d/%04d\n", gps.date.month(), gps.date.day(), gps.date.year());
    } else {
        Serial.println("Not Available");
    }

    Serial.print("Time: ");
    if (gps.time.isValid()) {
        Serial.printf("%02d:%02d:%02d\n", gps.time.hour(), gps.time.minute(), gps.time.second());
    } else {
        Serial.println("Not Available");
    }

    Serial.println();
}
