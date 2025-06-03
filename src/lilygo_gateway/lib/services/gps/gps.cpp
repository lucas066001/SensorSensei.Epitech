#include <gps/gps.h>

using namespace SensorSensei;
using namespace SensorSensei::Services;

GpsService::GpsService() : gps(), gpsSerial(2)
{
}

void GpsService::setup_gps()
{
    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, TX_PIN, RX_PIN);
}

void GpsService::update_spatio_data()
{
    int iter = 0;
    bool done = false;

    while (!done && iter < Constants::GatewayConfig::GpsMaxRetry)
    {
        Serial.print("Loop");

        if (gpsSerial.available() > 0)
        {
            Serial.print("GPS Available");

            char c = gpsSerial.read();
            // Serial.write(c);  // Debug only

            if (gps.encode(c))
            {
                replace_global_data();
                done = true;
            }
        }

        if (!done)
        {
            delay(Constants::GatewayConfig::RetryDelay);
        }

        iter++;
    }

    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());
}

void GpsService::replace_global_data()
{
    Serial.print("replace_global_data");

    // Only update location data if valid format
    // Otherwise keeping previous value
    if (gps.location.isValid())
    {
        Constants::SpatioData::Latitude = gps.location.lat();
        Constants::SpatioData::Longitude = gps.location.lng();
        Constants::SpatioData::Altitude = gps.altitude.meters();

        // Debug only
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
        Serial.print("Altitude: ");
        Serial.println(gps.altitude.meters());
    }

    if (gps.date.isValid())
    {
        Constants::TemporalData::CurrentYear = gps.date.year();
        Constants::TemporalData::CurrentMonth = gps.date.month();
        Constants::TemporalData::CurrentDay = gps.date.day();

        // Debug only
        Serial.print("Date: ");
        Serial.print(gps.date.month());
        Serial.print("/");
        Serial.print(gps.date.day());
        Serial.print("/");
        Serial.println(gps.date.year());
    }

    if (gps.time.isValid())
    {
        Constants::TemporalData::CurrentHour = gps.time.hour();
        Constants::TemporalData::CurrentMinute = gps.time.minute();
        Constants::TemporalData::CurrentSecond = gps.time.second();

        // Debug only
        Serial.print("Time: ");
        if (gps.time.hour() < 10)
            Serial.print("0");
        Serial.print(gps.time.hour());
        Serial.print(":");
        if (gps.time.minute() < 10)
            Serial.print("0");
        Serial.print(gps.time.minute());
        Serial.print(":");
        if (gps.time.second() < 10)
            Serial.print("0");
        Serial.print(gps.time.second());
        Serial.print(".");
        if (gps.time.centisecond() < 10)
            Serial.print("0");
        Serial.println(gps.time.centisecond());
    }

    // debug only
    Serial.println();
    Serial.println();
}
