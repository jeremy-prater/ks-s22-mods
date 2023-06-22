#include "s22_model.h"
#include "ui/ui.h"
#include "esp_log.h"

#define TAG "s22-model"

// Directly from BLE
void set_voltage(uint16_t voltage)
{
    char voltage_str[5];
    itoa(voltage, voltage_str, 10);
    strcat(voltage_str, "V");
    // Generate color gradient
    lv_color_t color = lv_color_hex(0xFFFFFF);
    lv_label_set_text(ui_VoltLabel, voltage_str);

    float icv = (voltage / 30.0) - 3.0;
    uint8_t percent = 0;
    if (icv > 0.0)
    {
        percent = (icv / 1.2) * 100.0;
    }
    set_batt_percent(percent);
}

void set_speed(uint16_t speed)
{
    // ESP_LOGI(TAG, "Setting Speed : %d", speed);

    char speed_str[4];
    itoa(speed, speed_str, 10);
    // Generate color gradient
    lv_color_t color = lv_color_hex(0xFFFFFF);
    lv_arc_set_value(ui_SpeedMeter, speed);
    lv_obj_set_style_arc_color(ui_SpeedMeter, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_label_set_text(ui_SpeedValue, speed_str);
}

void set_current(int16_t amps)
{
    char current_str[8];
    itoa(amps, current_str, 10);
    strcat(current_str, "A");
    // Generate color gradient
    lv_color_t color = lv_color_hex(0xFFFFFF);
    lv_label_set_text(ui_CurrentValue, current_str);
}
void set_temp(uint16_t temp)
{
    char temp_str[4];
    int8_t temp_f = (temp * (9.0 / 5.0)) + 32.0;
    itoa(temp_f, temp_str, 10);
    // strcat(temp_str, "C");
    // Generate color gradient
    lv_color_t color = lv_color_hex(0xFFFFFF);
    lv_label_set_text(ui_TempValue, temp_str);
}

// Synthesized values?
void set_pwm(uint16_t pwm)
{
    // ESP_LOGI(TAG, "Setting pwm : %d", pwm);

    char pwm_str[4];
    itoa(pwm, pwm_str, 10);
    // Generate color gradient
    lv_color_t color = lv_color_hex(0xFFFFFF);
    lv_arc_set_value(ui_PWMMeter, 100 - pwm);
    lv_obj_set_style_arc_color(ui_PWMMeter, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(ui_PWMValue, pwm_str);
}
void set_batt_percent(uint16_t batt_percent)
{
    char batt_str[5];
    itoa(batt_percent, batt_str, 10);
    strcat(batt_str, "%");
    // Generate color gradient
    lv_color_t color = lv_color_hex(0xFFFFFF);
    lv_label_set_text(ui_BatteryValue, batt_str);
}