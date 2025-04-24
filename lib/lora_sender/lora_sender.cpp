#include "lora_sender.h"
#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "audio_sensor_service.h"
#include "dust_sensor_service.h"

#define RF_FREQUENCY                868000000 // Frequency in Hz
#define TX_OUTPUT_POWER             5         // Transmission power in dBm

#define LORA_BANDWIDTH              0         // [0: 125 kHz]
#define LORA_SPREADING_FACTOR       7         // [SF7..SF12]
#define LORA_CODINGRATE             1         // [1: 4/5]
#define LORA_PREAMBLE_LENGTH        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON        false

#define LORA_TRANSMISSION_TIMEOUT   3000
#define BUFFER_SIZE                 64        // Max payload size

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

bool lora_idle = true; // Ready to transmit

static RadioEvents_t RadioEvents;

void OnTxDone(void);
void OnTxTimeout(void);

// Setup function
void lora_sender_setup() {
    Serial.begin(115200);
    delay(1000);

    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);

    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;

    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetTxConfig(
        MODEM_LORA,
        TX_OUTPUT_POWER,
        0,
        LORA_BANDWIDTH,
        LORA_SPREADING_FACTOR,
        LORA_CODINGRATE,
        LORA_PREAMBLE_LENGTH,
        LORA_FIX_LENGTH_PAYLOAD_ON,
        true,
        0,
        0,
        LORA_IQ_INVERSION_ON,
        LORA_TRANSMISSION_TIMEOUT
    );

    Serial.println("LoRa sender module initialized (license-free).");
}

// Loop function
void lora_sender_loop() {
    if (!lora_idle) {
        return;
    }

    audio_sensor_loop();
    dust_sensor_loop();

    float mean_audio_value = get_audio_mean();
    float current_dust_density = get_dust_density();

    snprintf(txpacket, BUFFER_SIZE, "Audio=%.2f, Dust=%.2f", mean_audio_value, current_dust_density);

    Serial.printf("Sending packet: \"%s\" (length: %d)\n", txpacket, strlen(txpacket));

    Radio.Send((uint8_t *)txpacket, strlen(txpacket));
    lora_idle = false;

    Radio.IrqProcess(); // Process interrupts manually, like in your base version
}

// Callback: Transmission done
void OnTxDone(void) {
    Serial.println("TX done...");
    lora_idle = true;
}

// Callback: Transmission timeout
void OnTxTimeout(void) {
    Serial.println("TX Timeout...");
    Radio.Sleep();
    lora_idle = true;
}
