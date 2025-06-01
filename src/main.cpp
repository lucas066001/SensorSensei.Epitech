#include <Arduino.h>
#include "audio_sensor_service.h"
#include "dust_sensor_service.h"
#include "lora_sender.h"
#include "wifi_service.h"      // New WiFi service
#include "sensor_common.h"

// Enable/disable features
#define ENABLE_WIFI_FEATURES true  // Set to false if you only want LoRa

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // Initialize all sensors and communication
    audio_sensor_setup();
    dust_sensor_setup();
    lora_sender_setup();
    
    #if ENABLE_WIFI_FEATURES
    wifi_service_setup();  // Initialize WiFi, web server, and cloud services
    #endif
    
    Serial.println("All sensors and communication modules initialized.");
}

void loop() {
    // Handle LoRa transmission (reads sensors and sends via LoRa)
    lora_sender_loop();
    
    #if ENABLE_WIFI_FEATURES
    // Handle WiFi services (web server updates and cloud uploads)
    wifi_service_loop();
    #endif
    
    // Delay between consecutive readings and transmissions
    delay(SENSOR_READ_INTERVAL); // Defined in sensor_common.h
}