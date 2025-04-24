#include "audio_sensor_service.h"

int16_t audio_buffer[AUDIO_BUFFER_LENGTH]; // Audio data buffer
float audio_mean_value = 0.0;

void audio_sensor_setup() {
    Serial.println("Setting up I2S audio input...");

    delay(1000);
    install_i2s_driver();
    configure_i2s_pins();
    i2s_start(I2S_PORT);

    delay(500);
    Serial.println("Audio sensor setup complete.");
}

void audio_sensor_loop() {
    size_t number_of_bytes_read = 0;
    esp_err_t result = i2s_read(I2S_PORT, &audio_buffer, AUDIO_BUFFER_LENGTH, &number_of_bytes_read, portMAX_DELAY);

    if (result == ESP_OK) {
        int number_of_samples = number_of_bytes_read / sizeof(int16_t);
        if (number_of_samples > 0) {
            float sum = 0;
            for (int i = 0; i < number_of_samples; ++i) {
                sum += static_cast<float>(audio_buffer[i]);
            }
            audio_mean_value = sum / number_of_samples;

            Serial.print("Audio Mean: ");
            Serial.println(audio_mean_value);
        }
    }
}

float get_audio_mean() {
    return audio_mean_value;
}

void install_i2s_driver() {
    const i2s_config_t i2s_configuration = {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = AUDIO_BUFFER_LENGTH,
        .use_apll = false
    };

    i2s_driver_install(I2S_PORT, &i2s_configuration, 0, NULL);
}

void configure_i2s_pins() {
    const i2s_pin_config_t pin_configuration = {
        .bck_io_num = I2S_SCK_PIN,
        .ws_io_num = I2S_WS_PIN,
        .data_out_num = -1,
        .data_in_num = I2S_SD_PIN
    };

    i2s_set_pin(I2S_PORT, &pin_configuration);
}
