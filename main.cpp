#include "Arduino.h"
#include "services/audio_sensor/audio_sensor_service.h"
#include "dust_sensor_service.h"
#include "LoRaWan_APP.h"
#include "lora_sender.h"

// Declare functions and initialize components
void setup() {
    // Initialize serial communication for debugging
    Serial.begin(115200);
    delay(1000);  // Wait for serial monitor to start

    // Initialize the sensors and LoRa sender
    audio_setup();           // Setup audio sensor
    dust_sensor_setup();     // Setup dust sensor
    lora_sender_setup();     // Setup LoRa sender
    
    Serial.println("Sensors and LoRa Sender Initialized!");
}

void loop() {
    // Collect data from sensors and process it
    audio_loop();            // Process audio sensor data
    dust_sensor_loop();      // Process dust sensor data

    // You can now send the collected sensor data via LoRa
    // Example: Collect some sensor data and send it
    String data = "Dust: " + String(get_dust_value()) + " mg/m3, Audio: " + String(get_audio_mean());
    
    // Send the data over LoRa
    lora_sender_send(data.c_str());  // Function from lora_sender to send data via LoRa
    
    delay(10000);  // Wait for a while before next reading
}
