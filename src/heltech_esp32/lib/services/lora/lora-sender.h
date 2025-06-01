#ifndef LORA_SERVICE_H
#define LORA_SERVICE_H

#include "LoRaWan_APP.h"
#include "Heltec.h"
#include "Arduino.h"

#pragma region Defines
#define RF_FREQUENCY 868000000  // Hz
#define TX_OUTPUT_POWER 5       // dBm
#define LORA_BANDWIDTH 0        // [0: 125 kHz,
                                //  1: 250 kHz,
                                //  2: 500 kHz,
                                //  3: Reserved]
#define LORA_SPREADING_FACTOR 7 // [SF7..SF12]
#define LORA_CODINGRATE 1       // [1: 4/5,
                                //  2: 4/6,
                                //  3: 4/7,
                                //  4: 4/8]
#define LORA_PREAMBLE_LENGTH 8  // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0   // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false
#define RX_TIMEOUT_VALUE 1000
#define BUFFER_SIZE 30 // Define the payload size here
static RadioEvents_t RadioEvents;
extern bool lora_idle;
#pragma endregion

// Function prototypes

namespace SensorSensei::Services
{

    class LoraSenderService
    {
    public:
        LoraSenderService();

        void loraSenderSetup();
        void loraSend(float meanAudio, float pm2_5, float pm10);

    private:
        static void onTxDone();
        static void onTxTimeout();

        char txpacket[BUFFER_SIZE];
        char rxpacket[BUFFER_SIZE];
        double txNumber;
    };
}

#endif
