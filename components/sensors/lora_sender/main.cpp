#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "lora_sender.h"  // Include the lora_sender header to access its functions

void setup() {
    // Initialize the LoRa sender
    lora_sender_setup();
}

void loop() {
    // Example sensor data (you can replace this with real sensor readings)
    const char* sensorData = "Sensor data: Dust=34.5 mg/m3, Audio=80 dB";  // Example data
    lora_sender_send(sensorData);  // Send data over LoRa

    delay(5000);  // Wait for 5 seconds before sending again (adjust as needed)
}
