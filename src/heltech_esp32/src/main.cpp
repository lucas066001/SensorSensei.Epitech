#include <dust-sensor.h>
#include <audio-sensor.h>
#include <board-config.h>
#include <lora-sender.h>
#include <WiFi.h>
#include <esp_sleep.h>
#include <esp_bt.h>

using namespace SensorSensei;

Services::LoraSenderService loraSenderService;
Repositories::DustSensorRepository dustSensorRepository;
Repositories::AudioSensorRepository audioSensorRepository;

void setup()
{
  // Energy saving setup
  WiFi.mode(WIFI_OFF);
  btStop(); // Arrêt Bluetooth
  esp_bt_controller_disable();

  Serial.begin(115200);
  delay(100);

  // Init sensors
  dustSensorRepository.dustSensorSetup();
  audioSensorRepository.audioSensorSetup();
  loraSenderService.loraSenderSetup();

  // Sensor read
  float meanAudio = audioSensorRepository.audioRead();

  std::tuple<float, float> dustValues = dustSensorRepository.dustRead();
  float pm25 = std::get<0>(dustValues);
  float pm10 = std::get<1>(dustValues);

  // LoRa send
  loraSenderService.loraSend(meanAudio, pm25, pm10);

  // Deep sleep
  esp_sleep_enable_timer_wakeup(Constants::BoardConfig::SleepTime);
  esp_deep_sleep_start();
}

void loop()
{
  // Never reached thanks to deep sleep
}
