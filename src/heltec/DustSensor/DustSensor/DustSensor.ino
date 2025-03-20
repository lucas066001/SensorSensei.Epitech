/**user define**/
int voutPin = 2;  // Use GPIO2 (VP) for ESP32 ADC input
int ledPin = 35;    // GPIO35 for LED control (change if needed)

/**system define**/
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
int voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  digitalWrite(ledPin, LOW); // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(voutPin); // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(ledPin, HIGH); // turn the LED off
  delayMicroseconds(sleepTime);

  // Adjusted for ESP32 (0 - 3.3V mapped to 0 - 4095)
  calcVoltage = (float)voMeasured * (3.3 / 4095.0);

  // Adjusted dust density formula
  if (calcVoltage >= 0.6) {
      dustDensity = 0.17 * calcVoltage - 0.1;
  } else {
      dustDensity = 0;
  }

  Serial.print("Raw Signal Value (0-4095): ");
  Serial.print(voMeasured);
  Serial.print(" - Voltage: ");
  Serial.print(calcVoltage);
  Serial.print("V");
  Serial.print(" - Dust Density: ");

  if (calcVoltage > 3.0) {
     Serial.print(">");  // unit: mg/m3
  }

  Serial.print(dustDensity);
  Serial.println(" mg/m3");

  delay(1000);
}
