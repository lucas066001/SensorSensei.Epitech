#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

// Configuration constants
extern const char* ssid;
extern const char* password;
extern String thingSpeakApiKey;
extern const char* thingSpeakServer;

// Timing constants
extern const unsigned long wifiUploadDelay;
extern const unsigned long webUpdateDelay;

// Function declarations
void wifi_service_setup();
void wifi_service_loop();
void upload_to_thingspeak(float audio_level, float dust_density);
void upload_to_custom_platform(float audio_level, float dust_density);
bool is_wifi_connected();
void setup_web_server();
String get_sensor_readings_json();
void init_wifi();
void init_spiffs();

#endif // WIFI_SERVICE_H