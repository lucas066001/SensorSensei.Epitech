# LoRa Communication Between Heltec WiFi LoRa V3 Boards

## Overview
This project demonstrates a simple LoRa communication system using two **Heltec WiFi LoRa V3** boards, where one acts as a transmitter (sender) and the other as a receiver. The system transmits messages over LoRa and displays received messages on the serial monitor.

## Hardware Requirements
- **2x Heltec WiFi LoRa V3** development boards
- USB cables for programming
- Antennas (must be attached before powering the boards)
- (Optional) Battery for autonomous operation

## Required Libraries
To compile and upload the firmware, ensure you have the following libraries installed:

1. **Heltec ESP32 Library**
   - Install via **Arduino Board Manager**  heltec board

   - Install via **Arduino Library Manager** heltec esp32
  

2. **LoRaWan_APP.h** (included in Heltec library)
   - Provides LoRa radio functions for ESP32.

## Board Configuration in Arduino IDE
1. Open Arduino IDE and go to **File** > **Preferences**.
2. Add the following URL to **Additional Board Manager URLs**:
   ```
   https://resource.heltec.cn/download/package_heltec_esp32_index.json
   ```
3. Go to **Tools** > **Board** > **Boards Manager**.
4. Search for **Heltec wifi lora32 v3** and install the packages if needed add the package **Lora By Sandeep Mistry**
5. Select the correct board:
   - **Tools** > **Board** > **Heltec Wifi LoRa V3**.
6. Select the correct port:
   - **Tools** > **Port** > (Select the port for your Heltec device).

## LoRa Configuration
### Frequency Setup
The project is configured for **915 MHz**, which is commonly used in the USA. Modify this if necessary:
```cpp
#define RF_FREQUENCY 915000000 // Hz
```

For other regions, use:
- EU: `868000000`
- ASIA: `923000000`

### Spreading Factor and Bandwidth
These parameters affect the range and speed of communication:
```cpp
#define LORA_SPREADING_FACTOR 7 // [SF7..SF12]
#define LORA_BANDWIDTH 0 // [0: 125 kHz, 1: 250 kHz, 2: 500 kHz]
```
Increasing the **spreading factor** increases range but slows transmission speed.

## Application Port Setup
If you need to specify an application port for structured LoRa data, you can define it within the payload structure. This is usually done at the LoRaWAN level, but for raw LoRa communication, it's handled in software.

## Uploading the Code
1. Connect your **Heltec WiFi LoRa V3** board to your computer.
2. Open **Arduino IDE**.
3. Select the correct board and port (as mentioned in board configuration).
4. Open the respective sketch (sender or receiver).
5. Click **Upload**.

## Running the Project
1. **Power up both boards**.
2. Open the **Serial Monitor** (`115200 baud` rate) for both sender and receiver.
3. The sender will start transmitting messages.
4. The receiver will display incoming messages with RSSI (signal strength).

## Troubleshooting
- Ensure antennas are connected **before powering the boards**.
- Check the correct frequency for your region.
- Verify that both devices use the same **spreading factor**, **bandwidth**, and **coding rate**.
- If messages are not received, try reducing the spreading factor or increasing the transmission power.
- If you face a license key troubleshoot add you license key from https://resource.heltec.cn/search   manually like **AT+CDKEY=0E32C3201D52F56B269F3F61090D19ED**

## Future Enhancements
- Implement **LoRaWAN** with TheThingsNetwork (TTN).
- Add **AES encryption** for secure data transmission.
- Optimize **power consumption** for longer battery life.

---
This project serves as a foundation for deploying LoRa-based communication networks with Heltec ESP32 boards. 🚀

