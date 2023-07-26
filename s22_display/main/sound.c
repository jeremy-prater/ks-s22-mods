#include "sound.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "esp_log.h"
#include "driver/i2s.h"
#include "string.h"

#define TAG "sound"

static QueueHandle_t i2s_event_queue;

#define NUM_FRAMES_TO_SEND 1024 * 4

void sound_i2c_writer_task(void *param)
{
    int availableBytes = 0;
    int buffer_position = 0;
    uint8_t frames[NUM_FRAMES_TO_SEND];

    memset(frames, 0, sizeof(uint8_t) * NUM_FRAMES_TO_SEND);

    while (true)
    {
        // wait for some data to be requested
        i2s_event_t evt;
        if (xQueueReceive(i2s_event_queue, &evt, portMAX_DELAY))
        {
            if (evt.type == I2S_EVENT_TX_DONE)
            {
                // ESP_LOGI(TAG, "DMA empty, sending more frames");

                size_t bytesWritten = 0;
                do
                {
                    if (availableBytes == 0)
                    {
                        // output->m_sample_generator->getFrames(frames, NUM_FRAMES_TO_SEND);

                        availableBytes = NUM_FRAMES_TO_SEND * sizeof(uint8_t);
                        buffer_position = 0;
                    }

                    if (availableBytes > 0)
                    {
                        i2s_write(I2S_NUM_0, buffer_position + (uint8_t *)frames,
                                  availableBytes, &bytesWritten, portMAX_DELAY);
                        // ESP_LOGI(TAG, "Wrote : %d", bytesWritten);
                        availableBytes -= bytesWritten;
                        buffer_position += bytesWritten;
                        // ESP_LOGI(TAG, "Remaining : %d", availableBytes);

                        if (availableBytes == 0)
                        {
                            break;
                        }
                    }
                } while (bytesWritten > 0);
            }
        }
    }
}

void sound_init()
{
    i2s_config_t i2sConfig = {
        .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_8BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
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