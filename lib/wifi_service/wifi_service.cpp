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

// WiFi Configuration - update with your WiFi hotspot SSID and password
const char* ssid = "";
const char* password = "";
String thingSpeakApiKey = "";

// Web server running on port 80
AsyncWebServer server(80);
AsyncEventSource events("/events");

// Timing variables for periodic uploads and web updates
unsigned long lastWiFiUpload = 0;
unsigned long lastWebUpdate = 0;
const unsigned long wifiUploadDelay = 30000; // 30 seconds delay between uploads
const unsigned long webUpdateDelay = 5000;   // 5 seconds delay between web page updates

// JSON variable to hold sensor readings
JSONVar readings;

// Function to scan available WiFi networks and print their SSIDs to Serial Monitor
void scan_wifi_networks() {
    Serial.println("Scanning for WiFi networks...");
    int networkCount = WiFi.scanNetworks();
    if (networkCount == 0) {
        Serial.println("No WiFi networks found.");
    } else {
        Serial.printf("Found %d WiFi networks:\n", networkCount);
        for (int i = 0; i < networkCount; ++i) {
            Serial.printf("%d: %s\n", i + 1, WiFi.SSID(i).c_str());
        }
    }
}

void wifi_service_setup() {
    // Initialize SPIFFS filesystem
    if (!SPIFFS.begin(true)) {
        Serial.println("An error occurred while mounting SPIFFS");
    } else {
        Serial.println("SPIFFS mounted successfully");
    }

    // Scan for available WiFi networks before attempting connection
    scan_wifi_networks();

    // Set WiFi mode to station (client)
    WiFi.mode(WIFI_STA);
    Serial.printf("Connecting to WiFi network: %s\n", ssid);
    WiFi.begin(ssid, password);

    // Wait for connection with timeout of 30 seconds
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000) {
        Serial.print(".");
        delay(500);
    }
    Serial.println();

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Failed to connect to WiFi within timeout.");
            switch (WiFi.status()) {
                case WL_NO_SSID_AVAIL:
                    Serial.println("Reason: SSID not available.");
                    break;
                case WL_CONNECT_FAILED:
                    Serial.println("Reason: Connection failed (may be wrong password or signal issue).");
                    break;
                case WL_DISCONNECTED:
                    Serial.println("Reason: Disconnected.");
                    break;
                default:
                    Serial.printf("Reason: WiFi status code %d\n", WiFi.status());
                    break;
        }

        return; // Do not continue to start web server if not connected
    }

    Serial.print("Connected to WiFi! IP address: ");
    Serial.println(WiFi.localIP());

    // Setup web server after successful WiFi connection
    setup_web_server();
}

void wifi_service_loop() {
    // Periodically send sensor data to connected clients via Server-Sent Events (SSE)
    if ((millis() - lastWebUpdate) > webUpdateDelay) {
        events.send(get_sensor_readings_json().c_str(), "sensor_readings", millis());
        lastWebUpdate = millis();
    }

    // Periodically upload sensor data to ThingSpeak server every wifiUploadDelay milliseconds
    if ((millis() - lastWiFiUpload) > wifiUploadDelay) {
        if (WiFi.status() == WL_CONNECTED) {
            float audio_level = get_audio_mean();
            float dust_density = get_dust_density();
            upload_to_thingspeak(audio_level, dust_density);
        } else {
            Serial.println("WiFi disconnected, attempting to reconnect...");
            WiFi.begin(ssid, password);
        }
        lastWiFiUpload = millis();
    }
}

void setup_web_server() {
    // Serve the main webpage with live sensor readings via Server-Sent Events
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

    // Serve static files from SPIFFS (if any)
    server.serveStatic("/", SPIFFS, "/");

    // Setup Server-Sent Events endpoint
    events.onConnect([](AsyncEventSourceClient * client) {
        if (client->lastId()) {
            Serial.printf("Client reconnected! Last message ID: %u\n", client->lastId());
        }
        client->send("Connected!", NULL, millis(), 10000);
    });

    server.addHandler(&events);
    server.begin();

    Serial.println("Web server started and listening on port 80");
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
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Cannot upload to ThingSpeak: WiFi is not connected.");
        return;
    }

    HTTPClient http;

    // ThingSpeak API endpoint URL
    String url = "http://api.thingspeak.com/update";

    // Prepare POST data string for ThingSpeak fields
    String postData = "api_key=" + thingSpeakApiKey;
    postData += "&field1=" + String(audio_level, 2);  // Audio level data
    postData += "&field2=" + String(dust_density, 2); // Dust density data

    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("ThingSpeak upload successful: " + response);
    } else {
        Serial.printf("Error uploading to ThingSpeak. HTTP response code: %d\n", httpResponseCode);
    }

    http.end();
}

void upload_to_custom_platform(float audio_level, float dust_density) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Cannot upload to custom platform: WiFi is not connected.");
        return;
    }

    HTTPClient http;

    // Custom server endpoint URL (replace with your actual endpoint)
    String url = "https://your-custom-server.com/api/sensors";

    // Prepare JSON payload
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
        Serial.printf("Error uploading to custom platform. HTTP response code: %d\n", httpResponseCode);
    }

    http.end();
}

bool is_wifi_connected() {
    return WiFi.status() == WL_CONNECTED;
}