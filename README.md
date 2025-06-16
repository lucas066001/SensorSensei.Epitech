# SensorSensei.Epitech

## Project Overview

SensorSensei.Epitech is an IoT project designed to collect and publish environmental data using two ESP32-based devices:

- **ESP32 Heltec (with LoRa module):** Acquires data from a sound sensor and a particulate matter (PM) sensor (air quality). It sends the data via LoRa.
- **ESP32 LilyGO (Gateway):** Acts as a gateway, receiving LoRa packets and publishing the data to [sensor.community](https://sensor.community/) via WiFi.

## Repository Structure

```
src/
  heltech_esp32/
    platformio.ini
    src/
      main.cpp
    lib/
      global/
      repositories/
      services/
    include/
    test/
  lilygo_gateway/
    platformio.ini
    src/
      main.cpp
    lib/
      global/
      services/
    include/
    test/
subject/
  (project documentation)
```

- Each device has its own PlatformIO project (`heltech_esp32` and `lilygo_gateway`).
- The `lib/` folders contain reusable code (services, repositories, global configs).
- The `src/` folders contain the main application logic.
- The `test/` folders are for unit tests (PlatformIO Test Runner).

## Platform & Dependencies

- **Platform:** [PlatformIO](https://platformio.org/) (recommended in VSCode)
- **Framework:** Arduino for ESP32
- **Boards:**
  - Heltec WiFi LoRa 32 V3
  - LilyGO T-Display

### Main Libraries

#### Heltec ESP32

- Heltec ESP32 Dev-Boards
- Adafruit SSD1306, GFX Library, BusIO
- Custom libraries for sensors and LoRa communication

#### LilyGO Gateway

- WiFi
- TinyGPSPlus
- Adafruit SSD1306, GFX Library, BusIO
- LoRa (sandeepmistry/LoRa)
- Custom libraries for WiFi, GPS, LoRa, and sensor data parsing

All dependencies are managed in each project's `platformio.ini` file.

## How It Works

1. **Heltec ESP32:**
   - Reads data from the sound and PM sensors.
   - Formats the data and sends it via LoRa.

2. **LilyGO Gateway:**
   - Receives LoRa packets.
   - Parses the sensor data.
   - Connects to WiFi and publishes the data to sensor.community using their API.

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) installed (VSCode recommended)
- ESP32 drivers installed ([Silicon Labs](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers))
- Both ESP32 boards (Heltec and LilyGO) with LoRa modules
- Sensors: sound sensor, particulate matter sensor (PM2.5/PM10)

### Setup

1. **Clone the repository:**
   ```sh
   git clone <repo-url>
   cd SensorSensei.Epitech
   ```

2. **Open in VSCode with PlatformIO extension.**

3. **Configure WiFi credentials:**
   - Edit `src/lilygo_gateway/src/main.cpp` and set your WiFi SSID and password.

4. **Build and upload firmware:**
   - Select the appropriate project (`heltech_esp32` or `lilygo_gateway`) in PlatformIO.
   - Click "Build" and "Upload" for each board.

5. **Connect sensors to the Heltec ESP32 as described in the code comments.**

6. **Power both devices.**
   - The Heltec board will periodically send sensor data via LoRa.
   - The LilyGO gateway will receive, parse, and publish the data to sensor.community.

## Documentation

- See the `subject/` folder for project documentation and requirements.
- Code is organized with clear separation between hardware abstraction (repositories), business logic (services), and configuration (global).

## License

MIT License. See [LICENSE](LICENSE) for details.

---

For any questions or contributions, please open an issue or submit a pull request.