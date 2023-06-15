#include "s22_model.h"
#include "ui/ui.h"
#include "esp_log.h"
#include ""

#define TAG "s22-model"


// Directly from BLE
void set_voltage(uint8_t voltage)
{
}

void set_speed(uint8_t speed)
{
    ESP_LOGI(TAG, "Setting Speed : %d", speed);

    char speed_str[4];
    itoa(speed, speed_str, 10);
    // Generate color gradient
    lv_color_t color = lv_color_hex(0xFFFFFF);
    lv_arc_set_value(ui_SpeedMeter, speed);
    lv_obj_set_style_arc_color(ui_SpeedMeter, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_label_set_text(ui_SpeedValue, speed_str);
}

void set_current(int8_t amps) {}
void set_temp(uint8_t temp) {}

// Synthesized values?
void set_pwm(uint8_t pwm)
{
    ESP_LOGI(TAG, "Setting pwm : %d", pwm);

    char pwm_str[4];
    itoa(pwm, pwm_str, 10);
    // Generate color gradient
    lv_color_t color = lv_color_hex(0xFFFFFF);
    lv_arc_set_value(ui_PWMMeter, 100 - pwm);
    lv_obj_set_style_arc_color(ui_PWMMeter, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(ui_PWMValue, pwm_str);
}
void set_batt_percent(uint8_t batt_percent) {}