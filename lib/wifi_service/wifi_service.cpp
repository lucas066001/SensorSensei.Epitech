// wifi_service.h
#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

// Configuration
extern const char* ssid;
extern const char* password;
extern String thingSpeakApiKey;

// Function declarations
void wifi_service_setup();
void wifi_service_loop();
void upload_to_thingspeak(float audio_level, float dust_density);
void upload_to_custom_platform(float audio_level, float dust_density);
bool is_wifi_connected();
void setup_web_server();
String get_sensor_readings_json();

#endif // WIFI_SERVICE_H

// wifi_service.cpp
#include "wifi_service.h"
#include "audio_sensor_service.h"
#include "dust_sensor_service.h"
#include "SPIFFS.h"

// WiFi Configuration - define these in your main project or config file
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
String thingSpeakApiKey = "YOUR_THINGSPEAK_API_KEY";

// Web server
AsyncWebServer server(80);
AsyncEventSource events("/events");

// Timing variables
unsigned long lastWiFiUpload = 0;
unsigned long lastWebUpdate = 0;
const unsigned long wifiUploadDelay = 30000; // 30 seconds
const unsigned long webUpdateDelay = 5000;   // 5 seconds

// JSON Variable to Hold Sensor Readings
JSONVar readings;

void wifi_service_setup() {
    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("An error has occurred while mounting SPIFFS");
    }
    Serial.println("SPIFFS mounted successfully");
    
    // Initialize WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println();
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
    
    // Setup web server
    setup_web_server();
}

void wifi_service_loop() {
    // Handle web server updates
    if ((millis() - lastWebUpdate) > webUpdateDelay) {
        events.send(get_sensor_readings_json().c_str(), "sensor_readings", millis());
        lastWebUpdate = millis();
    }
    
    // Handle cloud uploads
    if ((millis() - lastWiFiUpload) > wifiUploadDelay) {
        if (WiFi.status() == WL_CONNECTED) {
            float audio_level = get_audio_mean();
            float dust_density = get_dust_density();
            upload_to_thingspeak(audio_level, dust_density);
        } else {
            Serial.println("WiFi disconnected, attempting reconnection...");
            WiFi.begin(ssid, password);
        }
        lastWiFiUpload = millis();
    }
}

void setup_web_server() {
    // Handle Web Server
    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        String html = "<!DOCTYPE html><html><head><title>Audio & Dust Sensor</title></head><body>";
        html += "<h1>Heltec LoRa WiFi Sensor Station</h1>";
        html += "<div id='sensorData'></div>";
        html += "<script>";
        html += "if (!!window.EventSource) {";
        html += "var source = new EventSource('/events');";
        html += "source.addEventListener('sensor_readings', function(e) {";
        html += "var obj = JSON.parse(e.data);";
        html += "document.getElementById('sensorData').innerHTML = ";
        html += "'<p>Audio Level: ' + obj.audio + ' dB</p>' + ";
        html += "'<p>Dust Density: ' + obj.dust + ' µg/m³</p>' + ";
        html += "'<p>Last Update: ' + new Date().toLocaleTimeString() + '</p>';";
        html += "}, false);}";
        html += "</script></body></html>";
        request->send(200, "text/html", html);
    });
    
    server.serveStatic("/", SPIFFS, "/");
    
    // Handle Web Server Events
    events.onConnect([](AsyncEventSourceClient * client) {
        if (client->lastId()) {
            Serial.printf("Client reconnected! Last message ID: %u\n", client->lastId());
        }
        client->send("Connected!", NULL, millis(), 10000);
    });
    
    server.addHandler(&events);
    server.begin();
    Serial.println("Web server started");
}

String get_sensor_readings_json() {
    float mean_audio_value = get_audio_mean();
    float current_dust_density = get_dust_density();
    
    readings["audio"] = String(mean_audio_value, 2);
    readings["dust"] = String(current_dust_density, 2);
    readings["timestamp"] = String(millis());
    
    String jsonString = JSON.stringify(readings);
    return jsonString;
}

void upload_to_thingspeak(float audio_level, float dust_density) {
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    
    // ThingSpeak API endpoint
    String url = "http://api.thingspeak.com/update";
    
    // POST data - adjust field numbers as needed
    String postData = "api_key=" + thingSpeakApiKey;
    postData += "&field1=" + String(audio_level, 2);  // Audio level
    postData += "&field2=" + String(dust_density, 2); // Dust density
    
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    int httpResponseCode = http.POST(postData);
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("ThingSpeak upload successful: " + response);
    } else {
        Serial.println("Error uploading to ThingSpeak: " + String(httpResponseCode));
    }
    
    http.end();
}

void upload_to_custom_platform(float audio_level, float dust_density) {
    if (WiFi.status() != WL_CONNECTED) return;
    
    HTTPClient http;
    
    // Custom server endpoint
    String url = "https://your-custom-server.com/api/sensors";
    
    // JSON payload
    String jsonPayload = "{";
    jsonPayload += "\"audio\":" + String(audio_level, 2) + ",";
    jsonPayload += "\"dust\":" + String(dust_density, 2) + ",";
    jsonPayload += "\"device_id\":\"heltec_001\",";
    jsonPayload += "\"timestamp\":" + String(millis());
    jsonPayload += "}";
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    int httpResponseCode = http.POST(jsonPayload);
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Custom platform upload successful: " + response);
    } else {
        Serial.println("Error uploading to custom platform: " + String(httpResponseCode));
    }
    
    http.end();
}

bool is_wifi_connected() {
    return WiFi.status() == WL_CONNECTED;
}