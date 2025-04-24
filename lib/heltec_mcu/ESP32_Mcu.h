#ifndef McuSet_H
#define McuSet_H

#include <Arduino.h>
#include "SPI.h"
#include "driver/rtc-board.h"
#include "driver/board-config.h"
#include "driver/lorawan_spi.h"
#include "driver/sx126x.h"

#define LORA_DEFAULT_NSS_PIN    18
#define LORA_DEFAULT_RESET_PIN  14
#define LORA_DEFAULT_DIO0_PIN   26
#define LORA_DEFAULT_DIO1_PIN   33
#define Timer_DEFAULT_DIV       80

extern uint8_t mcuStarted;

class McuClass {
public:
    McuClass();
    // Removed setlicense()
    int begin(uint8_t board_type, uint8_t ex_32k);
    void addwakeio(uint8_t gpio);
    void setSPIFrequency(uint32_t frequency);
    void timerhandler();
    void sleep(uint8_t classMode, uint8_t debugLevel, uint8_t board_type, uint8_t ex_32k);
    SPISettings _spiSettings;
private:
};

extern TimerEvent_t TxNextPacketTimer;

#ifdef __cplusplus
extern "C" {
    uint8_t SpiInOut(Spi_t *obj, uint8_t outData );
    uint64_t timercheck();
    uint64_t getID();
    void SX126xIoInit(void);
    void SX126xIoIrqInit(DioIrqHandler dioIrq);
    void RadioOnDioIrq(void);
    void SX126xReset(void);
    void sx126xSleep(void);
    void calrtc();
    void lora_printf(const char *format, ...);
#ifdef RADIO_CHIP_SX127X
    void SX1276IoInit(void);
    void SX1276IoIrqInit(DioIrqHandler **irqHandlers);
    extern DioIrqHandler *DioIrq[];
#endif
}
#endif

extern McuClass Mcu;

#endif
