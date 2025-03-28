#include <TinyGPS++.h>

// Choose two Arduino pins to use for software serial
int RXPin = 12;
int TXPin = 34;

int GPSBaud = 9600;

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
// SoftwareSerial gpsSerial(RXPin, TXPin);
HardwareSerial gpsSerial(2);

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(115200);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud, SERIAL_8N1, TXPin,RXPin);
  gpsSerial.println("$PMTK225,0*2B");  // Active le GPS
  gpsSerial.println("$PMTK220,1000*1F"); // Fix toutes les secondes
  gpsSerial.println("$PMTK300,1000,0,0,0,0*1C"); // Mode normal
  gpsSerial.println("$PMTK314,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1*29"); // Activer toutes les trames NMEA

}

void loop()
{

while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    Serial.write(c);  // Affiche les trames GPS brutes

    if (gps.encode(c)) {
      displayInfo();
    }
  }



  // // This sketch displays information every time a new sentence is correctly encoded.
  // while (gpsSerial.available() > 0)
  //   if (gps.encode(gpsSerial.read()))
  //     displayInfo();

  // // If 5000 milliseconds pass and there are no characters coming in
  // // over the software serial port, show a "No GPS detected" error
  // if (millis() > 5000 && gps.charsProcessed() < 10)
  // {
  //   Serial.println("No GPS detected");
  //   while(true);
  // }
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}