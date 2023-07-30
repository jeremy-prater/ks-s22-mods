#include "sound.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "esp_log.h"
#include "driver/i2s.h"
#include "string.h"

#define TAG "sound"
#define NUM_FRAMES_TO_SEND 44100

static QueueHandle_t i2s_event_queue;

static SemaphoreHandle_t audio_data_lock = NULL;
static uint8_t *audio_data = NULL;
static size_t audio_data_len = 0;
static size_t audio_data_pos = 0;
static TickType_t start_time;

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
    evt.size = NUM_FRAMES_TO_SEND;
    xQueueSend(i2s_event_queue, (void *)&evt, portMAX_DELAY);
    start_time = xTaskGetTickCount();
}

static void sound_i2s_writer_task(void *param)
{
    while (true)
    {
        i2s_event_t evt;
        if (xQueueReceive(i2s_event_queue, &evt, portMAX_DELAY))
        {
            xSemaphoreTake(audio_data_lock, portMAX_DELAY);
            if (evt.type == I2S_EVENT_TX_DONE && audio_data_len > 0)
            {
                ESP_LOGI(TAG, "DMA needs data");
                size_t bytes_sent = 0;
                size_t send_size = evt.size;
                size_t data_left = audio_data_len - audio_data_pos;
                if (send_size > data_left)
                {
                    send_size = data_left;
                }

                ESP_LOGI(TAG, "Sending : %d", send_size);
                i2s_write_expand(I2S_NUM_0, audio_data, audio_data_len, I2S_BITS_PER_SAMPLE_8BIT, I2S_BITS_PER_SAMPLE_16BIT, &bytes_sent, portMAX_DELAY);
                ESP_LOGI(TAG, "Wrote : %d", bytes_sent);
                audio_data_pos += bytes_sent;
                ESP_LOGI(TAG, "Remaining : %d", audio_data_len - audio_data_pos);
                if (audio_data_len - audio_data_pos == 0)
                {
                    audio_data_pos = 0;
                    audio_data_len = 0;
                    i2s_zero_dma_buffer(I2S_NUM_0);
                    uint32_t duration = (xTaskGetTickCount() - start_time) * 10;
                    ESP_LOGI(TAG, "End of audio steam : %lu ms", duration);

                }
            }
            xSemaphoreGive(audio_data_lock);
        }
    }
}

void sound_init()
{
    audio_data_lock = xSemaphoreCreateMutex();
    i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_PCM_SHORT,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_desc_num = 5,
        //.dma_frame_num = 128, // 6260
        .dma_frame_num = 125, // 6410
        //.dma_frame_num = 124, // 6460
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 4, &i2s_event_queue);
    i2s_set_dac_mode(I2S_DAC_CHANNEL_RIGHT_EN);
    i2s_zero_dma_buffer(I2S_NUM_0);
    xTaskCreatePinnedToCore(sound_i2s_writer_task, "i2s_writer", 4096, NULL, 0, NULL, 1);

    ESP_LOGI(TAG, "Sound system init : I2S / DAC ready");
}