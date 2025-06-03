#include <wifi/wifi.h>

using namespace SensorSensei::Services;

WifiService::WifiService(String wifiSsid, String wifiPassword)
    : wifiId(wifiSsid), wifiPass(wifiPassword) {}

bool WifiService::connect()
{
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++)
    {
        Serial.printf("%d: %s (%d dBm)\n", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }

    WiFi.begin(wifiId.c_str(), wifiPass.c_str());

    Serial.print("Connecting to ");
    Serial.println(wifiId);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10)
    {
        delay(1000);
        Serial.print(".");
        attempts++;
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    else
    {
        Serial.println("WifiService.connect -> unable to connect");
        return false;
    }
}

bool WifiService::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

bool SensorSensei::Services::WifiService::sendPostRequest(const String &url, const std::map<String, String> &headers, const String &body)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("[WifiService] Not connected to WiFi.");

        if (connect())
        {
            Serial.println("[WifiService] Reconnected to WiFi.");
        }
        else
        {
            Serial.println("[WifiService] Failed to reconnect to WiFi.");
            return false;
        }
        return false;
    }
    Serial.println("[sendPostRequest] connected to WiFi :)");

    HTTPClient http;
    http.begin(url);

    // Add headers
    for (std::map<String, String>::const_iterator it = headers.begin(); it != headers.end(); ++it)
    {
        http.addHeader(it->first, it->second);
    }

    // POST request
    int httpResponseCode = http.POST(body);

    // Debug only
    if (httpResponseCode > 0)
    {
        Serial.printf("[WifiService] POST sent. Response code: %d\n", httpResponseCode);
        Serial.println("[WifiService] Response: " + http.getString());
    }
    else
    {
        Serial.printf("[WifiService] POST failed. Error code: %d\n", httpResponseCode);
    }

    http.end();
    return httpResponseCode > 0;
}

String WifiService::getCurrentMacAddress()
{
    if (isConnected())
    {
        return WiFi.macAddress();
    }
    return String();
}
