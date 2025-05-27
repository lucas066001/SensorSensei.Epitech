#include <dust-sensor.h>

using namespace SensorSensei::Repositories;

DustSensorRepository::DustSensorRepository()
{
	/** User-defined variables **/
	voutPin = 2; // Use GPIO2 (VP) for ESP32 ADC input
	ledPin = 35; // GPIO35 for LED control (change if needed)

	/** System-defined variables **/
	samplingTime = 280;
	deltaTime = 40;
	sleepTime = 9680;
	voMeasured = 0;
	calcVoltage = 0;
	dustDensity = 0;
}

void DustSensorRepository::dustSensorSetup()
{
	pinMode(ledPin, OUTPUT);
}

std::tuple<float, float> DustSensorRepository::dustRead()
{
	digitalWrite(ledPin, LOW); // Power on the LED
	delayMicroseconds(samplingTime);

	int voMeasured = analogRead(voutPin); // Read the dust value

	delayMicroseconds(deltaTime);
	digitalWrite(ledPin, HIGH); // Turn the LED off
	delayMicroseconds(sleepTime);

	float calcVoltage = (float)voMeasured * (3.3 / 4095.0);

	// Compute dust density (in mg/m3)
	float dustDensity = (calcVoltage >= 0.6) ? (0.17 * calcVoltage - 0.1) : 0.0;

	// Convert to PM2.5 and PM10 (basic heuristic, to adapt as needed)
	float pm25 = dustDensity;		// Assume direct mapping to PM2.5
	float pm10 = dustDensity * 1.8; // Heuristic: PM10 often ~1.5–2× PM2.5

	Serial.print("Raw Signal Value (0-4095): ");
	Serial.print(voMeasured);
	Serial.print(" - Voltage: ");
	Serial.print(calcVoltage);
	Serial.print("V - PM2.5: ");
	Serial.print(pm25);
	Serial.print(" µg/m3 - PM10: ");
	Serial.print(pm10);
	Serial.println(" µg/m3");

	return std::make_tuple(pm25, pm10);
}
