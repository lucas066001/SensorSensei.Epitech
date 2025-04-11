#include "LoRaWan_APP.h"
#include "Arduino.h"

// LoRa Parameters
#define RF_FREQUENCY        868000000 // Hz
#define LORA_BANDWIDTH      0         // 125 kHz
#define LORA_SPREADING_FACTOR 7       // SF7
#define LORA_CODINGRATE     1         // 4/5
#define LORA_PREAMBLE_LENGTH 8
#define LORA_SYMBOL_TIMEOUT 0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false
#define RX_TIMEOUT_VALUE    1000
#define BUFFER_SIZE         50  // Adjusted size for combined dust and audio data

char rxpacket[BUFFER_SIZE];
bool lora_idle = true;  // Flag to track LoRa receiver status

static RadioEvents_t RadioEvents;  // Define event handler

// Callback functions for LoRa events
void OnRxDone(void);
void OnRxTimeout(void);

// Function to send data over LoRa
void lora_sender_send(const char* data);

void setup() {
    Serial.begin(115200);  // Start serial communication
    delay(1000);  // Wait for serial monitor to start

    // Initialize MCU (change HELTEC_BOARD and SLOW_CLK_TPYE as needed for your board)
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);

    // Set up LoRa radio events
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.RxTimeout = OnRxTimeout;

    // Initialize LoRa radio with specified configuration
    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR, 
                      LORA_CODINGRATE, LORA_PREAMBLE_LENGTH, LORA_SYMBOL_TIMEOUT,
                      LORA_FIX_LENGTH_PAYLOAD_ON, LORA_IQ_INVERSION_ON, 0, true);

    Serial.println("LoRa Gateway Receiver Setup Complete!");
}

void loop() {
    // Read sensor data
    float dustDensity = 34.5;
    int audioMean = 80;

    char payload[BUFFER_SIZE];
    snprintf(payload, BUFFER_SIZE, "Dust:%.2f mg/m3 Audio Mean:%d dB", dustDensity, audioMean);

    lora_sender_send(payload);  // Transmit over LoRa

    delay(5000);  // Send every 5 seconds
}


// Callback function for received data (on receiving packet)
void OnRxDone(void) {
    // Get the received data
    int packetSize = Radio.GetPayloadLength();
    Radio.Read(rxpacket, packetSize);

    // Null terminate the received data for easy printing
    rxpacket[packetSize] = '\0';

    // Print the received packet data
    Serial.print("Received packet: ");
    Serial.println(rxpacket);

    // Parse and display received data (for example, dust and audio values)
    String packetData = String(rxpacket);

    int dustIndex = packetData.indexOf("Dust:");
    int audioIndex = packetData.indexOf("Audio Mean:");

    if (dustIndex != -1 && audioIndex != -1) {
        String dustValue = packetData.substring(dustIndex + 6, packetData.indexOf("mg/m3"));
        String audioValue = packetData.substring(audioIndex + 12);

        // Print the received data values
        Serial.print("Dust Density: ");
        Serial.print(dustValue);
        Serial.println(" mg/m3");

        Serial.print("Audio Mean: ");
        Serial.println(audioValue);
    }

    // Prepare to receive the next packet
    lora_idle = true;
}

// Callback function for receive timeout (no packet received)
void OnRxTimeout(void) {
    Serial.println("RX Timeout");
    lora_idle = true;
}

// Function to send data over LoRa
void lora_sender_send(const char* data) {
    // Send the data over LoRa
    Radio.Send((uint8_t*)data, strlen(data));
    Serial.print("Sent via LoRa: ");
    Serial.println(data);
}
