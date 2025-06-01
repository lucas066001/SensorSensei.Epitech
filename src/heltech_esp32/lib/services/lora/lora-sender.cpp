#include <lora-sender.h>
#include "LoRaWan_APP.h"
#include "Arduino.h"

bool lora_idle = true;

using namespace SensorSensei::Services;

LoraSenderService::LoraSenderService()
{
}

void LoraSenderService::loraSenderSetup()
{
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);

    txNumber = 0;

    RadioEvents.TxDone = onTxDone;
    RadioEvents.TxTimeout = onTxTimeout;

    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                      LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                      LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                      true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
}

void LoraSenderService::loraSend(float meanAudio, float pm2_5, float pm10)
{
    if (lora_idle)
    {
        // Construire le message avec le format demandé
        String payload = "<" + String(meanAudio, 2) + "><" + String(pm2_5, 2) + "><" + String(pm10, 2) + ">";
        payload.toCharArray(txpacket, sizeof(txpacket));

        Serial.printf("\r\nSending LoRa packet \"%s\", length %d\r\n", txpacket, strlen(txpacket));

        Radio.Send((uint8_t *)txpacket, strlen(txpacket));
        lora_idle = false;
    }

    Radio.IrqProcess(); // Nécessaire pour le traitement des interruptions LoRa
}

void LoraSenderService::onTxDone(void)
{
    Serial.println("TX done......");
    lora_idle = true;
}

void LoraSenderService::onTxTimeout(void)
{
    Radio.Sleep();
    Serial.println("TX Timeout......");
    lora_idle = true;
}