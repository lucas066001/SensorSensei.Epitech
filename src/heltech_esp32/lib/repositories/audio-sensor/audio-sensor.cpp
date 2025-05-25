#include <audio-sensor.h>

using namespace SensorSensei::Repositories;

AudioSensorRepository::AudioSensorRepository() : sBuffer{} // zero-initialized
{
}

void AudioSensorRepository::audioSensorSetup()
{
	Serial.begin(115200);
	Serial.println("Setup I2S ...");

	delay(1000);
	i2sInstall();
	i2sSetpin();
	i2s_start(I2S_PORT);
	delay(500);
}

float AudioSensorRepository::audioRead()
{
	const int durationSeconds = 15;
	const unsigned long startMillis = millis();

	double sumSquare = 0.0;
	size_t totalSamples = 0;

	while ((millis() - startMillis) < durationSeconds * 1000)
	{
		size_t bytesIn = 0;
		esp_err_t result = i2s_read(I2S_PORT, &sBuffer, sizeof(sBuffer), &bytesIn, portMAX_DELAY);
		if (result == ESP_OK)
		{
			int samplesRead = bytesIn / sizeof(int16_t);
			for (int i = 0; i < samplesRead; ++i)
			{
				float sample = static_cast<float>(sBuffer[i]);
				sumSquare += sample * sample;
			}
			totalSamples += samplesRead;
		}
		delay(50);
	}

	float rms = (totalSamples > 0) ? sqrt(sumSquare / totalSamples) : 0.0f;

	// Calcul du niveau en dBFS (full scale = 32768 pour 16-bit)
	float db = (rms > 0) ? 20.0f * log10(rms / 32768.0f) : -100.0f;

	Serial.print("Mean dB over ");
	Serial.print(durationSeconds);
	Serial.print("s: ");
	Serial.println(db);

	return db;
}

void AudioSensorRepository::i2sInstall()
{
	const i2s_config_t i2s_config = {
		.mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
		.sample_rate = 44100,
		.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
		.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
		.communication_format = I2S_COMM_FORMAT_STAND_I2S,
		.intr_alloc_flags = 0,
		.dma_buf_count = 8,
		.dma_buf_len = bufferLen,
		.use_apll = false,
		.tx_desc_auto_clear = false,
		.fixed_mclk = 0};

	i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void AudioSensorRepository::i2sSetpin()
{
	const i2s_pin_config_t pin_config = {
		.bck_io_num = I2S_SCK,
		.ws_io_num = I2S_WS,
		.data_out_num = -1,
		.data_in_num = I2S_SD};

	i2s_set_pin(I2S_PORT, &pin_config);
}
