#include <gps/gps.h>
#include <wifi/wifi.h>
#include <lora/lora-receiver.h>
#include <utils/parser/sensor-parser.h>
#include <external/sensor-community/sensor-community.h>

using namespace SensorSensei::Services;

GpsService gpsService;
WifiService wifiService("YOUR_WIFI_SSID", "YOUR_WIFI_PASSWORD");
LoraReceiverService loraReceiverService;
External::SensorCommunityService sensorCommunityService(&wifiService);
Utils::SensorParserService sensorParserService;

void setup()
{
	Serial.begin(115200);
	delay(1000);
	wifiService.connect();
	gpsService.setup_gps();
	loraReceiverService.setup_lora();
}

void loop()
{
	String receivedPacket = loraReceiverService.handle();
	if (!receivedPacket.isEmpty())
	{
		Serial.println("Received Packet: " + receivedPacket);

		// Basically useless as sensorCommunity doesn't allow GPS data update through API
		gpsService.update_spatio_data();
		std::tuple<float, float, float> sensorDatas = sensorParserService.extractSensorData(receivedPacket);

		float meanSoundLevel = std::get<0>(sensorDatas);
		float meanDustDensity25 = std::get<1>(sensorDatas);
		float meanDustDensity10 = std::get<2>(sensorDatas);

		Serial.printf("Parsed meanSoundLevel: %d\n", meanSoundLevel);
		Serial.printf("Parsed meanDustDensity25: %d\n", meanDustDensity25);
		Serial.printf("Parsed meanDustDensity10: %d\n", meanDustDensity10);

		if (meanSoundLevel >= 0.0f)
		{
			sensorCommunityService.sendSoundData(meanSoundLevel);
		}
		else
		{
			// Low sound level so we round to 0.0f
			sensorCommunityService.sendSoundData(0.0f);
			Serial.println("Extremly low sound data received. sending 0.0f");
		}

		if (meanDustDensity10 >= 0.0f && meanDustDensity25 >= 0.0f)
		{
			sensorCommunityService.sendDustData(meanDustDensity25, meanDustDensity10);
		}
		else
		{
			Serial.println("Invalid dust data received. Ignoring");
		}
	}
	else
	{
		// Serial.println("No packet received");
	}

	// delay(1000);
}
