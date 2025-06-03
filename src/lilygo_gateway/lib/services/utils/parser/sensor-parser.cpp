#include <utils/parser/sensor-parser.h>

using namespace SensorSensei::Services::Utils;

std::tuple<float, float, float> SensorParserService::extractSensorData(const String &input)
{
    float meanSoundLevel = 0.0f;
    float pm2_5 = 0.0f;
    float pm10 = 0.0f;

    int firstOpen = input.indexOf('<');
    int firstClose = input.indexOf('>', firstOpen);
    int secondOpen = input.indexOf('<', firstClose);
    int secondClose = input.indexOf('>', secondOpen);
    int thirdOpen = input.indexOf('<', secondClose);
    int thirdClose = input.indexOf('>', thirdOpen);

    if (firstOpen != -1 && firstClose != -1 &&
        secondOpen != -1 && secondClose != -1 &&
        thirdOpen != -1 && thirdClose != -1)
    {
        String audioStr = input.substring(firstOpen + 1, firstClose);
        String pm2_5Str = input.substring(secondOpen + 1, secondClose);
        String pm10Str = input.substring(thirdOpen + 1, thirdClose);

        meanSoundLevel = audioStr.toFloat();
        pm2_5 = pm2_5Str.toFloat();
        pm10 = pm10Str.toFloat();

        Serial.print("Parsed meanSoundLevel: ");
        Serial.println(meanSoundLevel);
        Serial.print("Parsed pm2.5: ");
        Serial.println(pm2_5);
        Serial.print("Parsed pm10: ");
        Serial.println(pm10);
    }
    else
    {
        Serial.println("SensorParserService: Failed to parse input string.");
    }

    return std::make_tuple(meanSoundLevel, pm2_5, pm10);
}
