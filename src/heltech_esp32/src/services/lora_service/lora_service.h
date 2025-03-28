#ifndef LORA_SENDER_H
#define LORA_SENDER_H

#include "LoRaWan_APP.h"
#include "Heltec.h"
#include "Arduino.h"

// LoRa configuration
#define RF_FREQUENCY 868000000  // Hz
#define TX_OUTPUT_POWER 5       // dBm
#define LORA_BANDWIDTH 0        // [0: 125 kHz, 1: 250 kHz, 2: 500 kHz]
#define LORA_SPREADING_FACTOR 7 // [SF7..SF12]
#define LORA_CODINGRATE 1       // [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define LORA_PREAMBLE_LENGTH 8  
#define LORA_SYMBOL_TIMEOUT 0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false
#define RX_TIMEOUT_VALUE 1000
#define BUFFER_SIZE 30          // Payload size

// Extern variables
extern char txpacket[BUFFER_SIZE];
extern char rxpacket[BUFFER_SIZE];
extern double txNumber;
extern bool lora_idle;

// Function prototypes
void lora_sender_setup();
void lora_sender_loop();
void OnTxDone();
void OnTxTimeout();

#endif // LORA_SENDER_H
