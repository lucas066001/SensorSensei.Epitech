#ifndef GPS_SERVICE_H
#define GPS_SERVICE_H

#include <gateway_config.h>
#include <spatioData.h>
#include <temporalData.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Pin definitions for GPS
#define RX_PIN 12
#define TX_PIN 34
#define GPS_BAUD 9600

namespace SensorSensei::Services
{

    class GpsService
    {
    public:
        GpsService();

        void setup_gps();
        void update_spatio_data();

    private:
        void replace_global_data();
        TinyGPSPlus gps;
        HardwareSerial gpsSerial;
    };
}
#endif
