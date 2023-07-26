#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "esp_log.h"
#include "esp_spiffs.h"
#include "string.h"

#define TAG "S22_SPIFFS"

void spiffs_init(const char * mount_point) {
    if (mount_point == NULL) {
        ESP_LOGW(TAG, "No mount specified!");
        return;
    }

    size_t max_path_len = strlen(mount_point) + 2;
    char * path = malloc(max_path_len);
    memset(path, 0, max_path_len);
    path[0] = '/';
    strcat(path, mount_point);

    ESP_LOGI(TAG, "Mounting : %s", path);

    esp_vfs_spiffs_conf_t conf = {
      .base_path = path,
      .partition_label = mount_point,
      .max_files = 32,
      .format_if_mount_failed = false
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
}