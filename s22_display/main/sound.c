#include "sound.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "esp_log.h"
#include "driver/i2s.h"
#include "string.h"

#define TAG "sound"
#define NUM_FRAMES_TO_SEND 1024

static QueueHandle_t i2s_event_queue;

static SemaphoreHandle_t audio_data_lock = NULL;
static uint8_t *audio_data = NULL;
static size_t audio_data_len = 0;
static size_t audio_data_pos = 0;
static bool audio_play = false;

void sound_play(const char *filename)
{
    xSemaphoreTake(audio_data_lock, portMAX_DELAY);

    if (audio_data != NULL)
    {
        ESP_LOGW(TAG, "Restarting audio with new stream : %s", filename);
    }
    FILE *audio_file = fopen(filename, "r");

    if (audio_file == NULL)
    {
        ESP_LOGE(TAG, "Failed to open audio file %s", filename);
        xSemaphoreGive(audio_data_lock);
        return;
    }

    fseek(audio_file, 0, SEEK_END);
    size_t audio_file_size = ftell(audio_file);
    fseek(audio_file, 0, SEEK_SET);

    audio_data = realloc(audio_data, audio_file_size);
    if (audio_data == NULL)
    {
        ESP_LOGW(TAG, "failed to allocate memory for audio file : %d", audio_file_size);
        xSemaphoreGive(audio_data_lock);
        return;
    }

    audio_data_len = audio_file_size;
    audio_data_pos = 0;
    audio_play = true;

    ESP_LOGI(TAG, "Playing audio file : %s with size %d", filename, audio_data_len);

    if (fread(audio_data, audio_file_size, 1, audio_file) != 1)
    {
        ESP_LOGW(TAG, "failed to read audio file");
        xSemaphoreGive(audio_data_lock);
        return;
    }


    fclose(audio_file);

    xSemaphoreGive(audio_data_lock);

    i2s_event_t evt;
    evt.type = I2S_EVENT_TX_DONE;
    xQueueSend(i2s_event_queue, (void *)&evt, portMAX_DELAY);
}

void sound_i2c_writer_task(void *param)
{
    int availableBytes = 0;
    int buffer_position = 0;
    uint8_t frames[NUM_FRAMES_TO_SEND];
    const size_t frame_size = sizeof(uint8_t) * NUM_FRAMES_TO_SEND;

    memset(frames, 0, frame_size);

    while (true)
    {
        // wait for some data to be requested
        i2s_event_t evt;
        if (audio_play && xQueueReceive(i2s_event_queue, &evt, portMAX_DELAY))
        {
            if (evt.type == I2S_EVENT_TX_DONE)
            {
                ESP_LOGI(TAG, "DMA empty, sending more frames");

                size_t bytesWritten = 0;
                do
                {
                    xSemaphoreTake(audio_data_lock, portMAX_DELAY);
                    if (availableBytes == 0)
                    {
                        if (audio_data != NULL)
                        {
                            availableBytes = sizeof(uint8_t) * NUM_FRAMES_TO_SEND;
                            memcpy(frames, audio_data + buffer_position, frame_size);
                            ESP_LOGI(TAG, "Read %d bytes from audio ram", availableBytes);
                        }
                    }

                    if (availableBytes > 0)
                    {
                        i2s_write(I2S_NUM_0, buffer_position + (uint8_t *)frames,
                                  availableBytes, &bytesWritten, portMAX_DELAY);
                        ESP_LOGI(TAG, "Wrote : %d", bytesWritten);
                        availableBytes -= bytesWritten;
                        buffer_position += bytesWritten;
                        ESP_LOGI(TAG, "Remaining : %d", audio_data_len - buffer_position);

                        if (buffer_position >= audio_data_len)
                        {
                            ESP_LOGI(TAG, "End of audio steam!");
                            buffer_position = 0;
                            memset(frames, 0, frame_size);
                            free(audio_data);
                            audio_data = NULL;
                            availableBytes = frame_size;
                            audio_data_len = frame_size;
                            audio_play = false;
                            xSemaphoreGive(audio_data_lock);
                            break;
                        }
                    }
                    xSemaphoreGive(audio_data_lock);
                } while (bytesWritten > 0);
            }
        }
    }
}

void sound_init()
{
    audio_data_lock = xSemaphoreCreateMutex();
    i2s_config_t i2sConfig = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_8BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S_LSB,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 4,
        .dma_buf_len = 64,
    };

    i2s_driver_install(I2S_NUM_0, &i2sConfig, 4, &i2s_event_queue);
    i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);
    i2s_zero_dma_buffer(I2S_NUM_0);
    xTaskCreatePinnedToCore(sound_i2c_writer_task, "i2s_writer", 4096 * 2, NULL, 0, NULL, 1);

    ESP_LOGI(TAG, "Sound system init : I2S / DAC ready");
}