#include "s22_model.h"
#include "ui/ui.h"
#include "esp_log.h"
#include "icons.h"

#define TAG "s22-model"

typedef struct
{
    uint16_t voltage;
    uint16_t speed;
    int16_t current;
    uint16_t temp;
    uint16_t battery_percent;
    uint16_t pwm;
} s22_model;

static s22_model model;

// M5 Hardware battery status
void set_m5_charging(m5_battery_charging_t charging)
{
    if (pmic_get_info()->charging != charging)
    {
        ESP_LOGI(TAG, "Update PMIC data : charging %d", charging);
        pmic_get_info()->charging = charging;

        if (charging == M5_BATTERY_CHARGING_YES)
        {
            lv_obj_clear_flag(ui_M5Battery0Charging, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_M5Battery1Charging, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(ui_M5Battery2Charging, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_add_flag(ui_M5Battery0Charging, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_M5Battery1Charging, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(ui_M5Battery2Charging, LV_OBJ_FLAG_HIDDEN);
        }
        lv_event_send(ui_M5Battery0Charging, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_M5Battery1Charging, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_M5Battery2Charging, LV_EVENT_REFRESH, NULL);
    }
}

void set_m5_charging_full(m5_battery_full_t full)
{
    if (pmic_get_info()->full != full)
    {
        ESP_LOGI(TAG, "Update PMIC data : full %d", full);
        pmic_get_info()->full = full;
        if (full == M5_BATTERY_FULL_YES)
        {
            lv_obj_set_style_text_color(ui_M5Battery0Charging, lv_color_hex(0x0008FF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(ui_M5Battery1Charging, lv_color_hex(0x0008FF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(ui_M5Battery2Charging, lv_color_hex(0x0008FF), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_text_color(ui_M5Battery0Charging, lv_color_hex(0x00FF08), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(ui_M5Battery1Charging, lv_color_hex(0x00FF08), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(ui_M5Battery2Charging, lv_color_hex(0x00FF08), LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        lv_event_send(ui_M5Battery0Charging, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_M5Battery1Charging, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_M5Battery2Charging, LV_EVENT_REFRESH, NULL);
    }
}

void set_m5_battery_percent(m5_battery_level_t level)
{
    if (pmic_get_info()->level != level)
    {
        pmic_get_info()->level = level;
        const char *level_icon;
        switch (level)
        {
        case M5_BATTERY_LEVEL_100:
            level_icon = ICON_BATTERY_FULL;
            break;
        case M5_BATTERY_LEVEL_75:
            level_icon = ICON_BATTERY_THREE_QUARTER;
            break;
        case M5_BATTERY_LEVEL_50:
            level_icon = ICON_BATTERY_HALF;
            break;
        case M5_BATTERY_LEVEL_25:
            level_icon = ICON_BATTERY_QUARTER;
            break;
        default:
            level_icon = ICON_BATTERY_EMPTY;
        }

        ESP_LOGI(TAG, "Update PMIC data : level %d", level);

        lv_label_set_text(ui_M5Battery0, level_icon);
        lv_label_set_text(ui_M5Battery1, level_icon);
        lv_label_set_text(ui_M5Battery2, level_icon);

        lv_event_send(ui_M5Battery0, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_M5Battery1, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_M5Battery2, LV_EVENT_REFRESH, NULL);
    }
}

// Directly from BLE
void set_voltage(uint16_t voltage)
{
    if (model.voltage != voltage)
    {
        model.voltage = voltage;
        char voltage_str[5];
        itoa(voltage, voltage_str, 10);
        strcat(voltage_str, "V");
        // Generate color gradient
        lv_color_t color = lv_color_hex(0xFFFFFF);
        lv_label_set_text(ui_VoltLabel, voltage_str);

        lv_event_send(ui_VoltLabel, LV_EVENT_REFRESH, NULL);

        float icv = (voltage / 30.0) - 3.0;
        uint8_t percent = 0;
        if (icv > 0.0)
        {
            percent = (icv / 1.2) * 100.0;
        }
        set_batt_percent(percent);
    }
}

void set_speed(uint16_t speed)
{
    if (model.speed != speed)
    {
        model.speed = speed;
        char speed_str[4];
        itoa(speed, speed_str, 10);
        // Generate color gradient
        lv_color_t color = lv_color_hex(0xFFFFFF);
        lv_arc_set_value(ui_SpeedMeter, speed);
        lv_obj_set_style_arc_color(ui_SpeedMeter, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_label_set_text(ui_SpeedValue, speed_str);

        lv_event_send(ui_SpeedMeter, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_SpeedValue, LV_EVENT_REFRESH, NULL);
    }
}

void set_current(int16_t amps)
{
    if (model.current != amps)
    {
        model.current = amps;
        char current_str[8];
        itoa(amps, current_str, 10);
        strcat(current_str, "A");
        // Generate color gradient
        lv_color_t color = lv_color_hex(0xFFFFFF);
        lv_label_set_text(ui_CurrentValue, current_str);

        lv_event_send(ui_CurrentValue, LV_EVENT_REFRESH, NULL);
    }
}

void set_temp(uint16_t temp)
{
    if (model.temp != temp)
    {
        model.temp = temp;
        char temp_str[4];
        int8_t temp_f = (temp * (9.0 / 5.0)) + 32.0;
        itoa(temp_f, temp_str, 10);
        // strcat(temp_str, "C");
        // Generate color gradient
        lv_color_t color = lv_color_hex(0xFFFFFF);
        lv_label_set_text(ui_TempValue, temp_str);

        lv_event_send(ui_TempValue, LV_EVENT_REFRESH, NULL);
    }
}

// Synthesized values?
void set_pwm(uint16_t pwm)
{
    if (model.pwm != pwm)
    {
        model.pwm = pwm;
        char pwm_str[4];
        itoa(pwm, pwm_str, 10);
        // Generate color gradient
        lv_color_t color = lv_color_hex(0xFFFFFF);
        lv_arc_set_value(ui_PWMMeter, 100 - pwm);
        lv_obj_set_style_arc_color(ui_PWMMeter, color, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(ui_PWMValue, pwm_str);

        lv_event_send(ui_PWMValue, LV_EVENT_REFRESH, NULL);
    }
}

void set_batt_percent(uint16_t batt_percent)
{
    if (model.battery_percent != batt_percent)
    {
        model.battery_percent = batt_percent;
        char batt_str[5];
        itoa(batt_percent, batt_str, 10);
        strcat(batt_str, "%");
        // Generate color gradient
        lv_color_t color = lv_color_hex(0xFFFFFF);
        lv_label_set_text(ui_BatteryValue, batt_str);

        lv_event_send(ui_BatteryValue, LV_EVENT_REFRESH, NULL);
    }
}

// Getters

uint16_t get_voltage()
{
    return model.voltage;
}
uint16_t get_speed()
{
    return model.speed;
}
int16_t get_current()
{
    return model.current;
}
uint16_t get_temp()
{
    return model.temp;
}
uint16_t get_pwm()
{
    return model.pwm;
}
uint16_t get_battery_percent()
{
    return model.battery_percent;
}
