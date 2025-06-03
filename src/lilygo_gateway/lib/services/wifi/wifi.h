#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <gateway_config.h>
#include <WiFi.h>
#include <Arduino.h>
#include <HTTPClient.h> // ✅ Requis pour requêtes HTTP
#include <map>

namespace SensorSensei::Services
{
    class WifiService
    {
    public:
        WifiService(String wifiSsid, String wifiPassword);

        bool connect();
        bool isConnected();
        bool sendPostRequest(const String &url, const std::map<String, String> &headers, const String &body);
        String getCurrentMacAddress();

    private:
        String wifiId;
        String wifiPass;
    };
}

#endif
