#include "s22_model.h"
#include "ui/ui.h"
#include "esp_log.h"
#include "icons.h"
#include "gradient.h"

#define TAG "s22-model"

typedef struct
{
    uint16_t voltage;
    uint16_t speed;
    rgb_t *speed_color;
    rgb_t *power_color;
    int16_t current;
    int16_t power;
    uint16_t temp;
    uint16_t battery_percent;
    uint16_t pwm;
} s22_model;

static s22_model model;

void model_init()
{
    model.voltage = 0xFFFF;
    model.speed = 0xFFFF;
    model.speed_color = gradient_get_color(0);
    model.power_color = gradient_get_color(0);
    model.current = 0xFFFF;
    model.power = 0xFFFF;
    model.temp = 0xFFFF;
    model.battery_percent = 0xFFFF;
    model.pwm = 0xFFFF;
}

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

        lv_label_set_text(ui_VoltLabel, voltage_str);

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
        uint16_t speed_color = speed;
        if (speed_color > 44)
        {
            speed_color = 44;
        }
        uint8_t color_index = ((speed_color / 44.0f) * 0xFF);
        rgb_t *new_color = gradient_get_color(color_index);
        lv_color_t color = lv_color_make(new_color->red, new_color->green, new_color->blue);
        model.speed_color = new_color;
        lv_arc_set_value(ui_SpeedMeter, speed);
        lv_obj_set_style_arc_color(ui_SpeedMeter, color, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_label_set_text(ui_SpeedValue, speed_str);
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
        lv_label_set_text(ui_BatteryValue, batt_str);

        rgb_t *battery_color = gradient_get_color((100 - batt_percent) * 2.55F);
        lv_color_t color = lv_color_make(battery_color->red, battery_color->green, battery_color->blue);
        lv_obj_set_style_text_color(ui_BatteryIcon, color, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(ui_VoltIcon, color, LV_PART_MAIN | LV_STATE_DEFAULT);
        const char *level_icon;
        if (batt_percent < 20)
        {
            level_icon = ICON_BATTERY_EMPTY;
        }
        else if (batt_percent < 40)
        {
            level_icon = ICON_BATTERY_QUARTER;
        }
        else if (batt_percent < 60)
        {
            level_icon = ICON_BATTERY_HALF;
        }
        else if (batt_percent < 80)
        {
            level_icon = ICON_BATTERY_THREE_QUARTER;
        }
        else
        {
            level_icon = ICON_BATTERY_FULL;
        }
        lv_label_set_text(ui_BatteryIcon, level_icon);
    }
}

void update_power()
{
    int16_t new_power = model.voltage * model.current;

    if (model.power != new_power)
    {
        model.power = new_power;

        char power_str[5];
        itoa(model.power, power_str, 10);

        int16_t display_power = abs(model.power);
        if (display_power > 3300)
        {
            display_power = 3300;
        }
        uint8_t meter_range = display_power / 33.0f;
        rgb_t *meter_color = gradient_get_color(meter_range * 2.55f);
        lv_label_set_text(ui_PWMValue, power_str);
        lv_arc_set_value(ui_PWMMeter, 100 - meter_range);
        lv_color_t power_color = lv_color_make(meter_color->red, meter_color->green, meter_color->blue);
        model.power_color = meter_color;
        lv_obj_set_style_arc_color(ui_PWMMeter, power_color, LV_PART_MAIN | LV_STATE_DEFAULT);
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
rgb_t *get_speed_color()
{
    if (model.speed_color == NULL)
    {
        model.speed_color = gradient_get_color(0);
    }
    return model.speed_color;
}
rgb_t *get_power_color()
{
    if (model.power_color == NULL)
    {
        model.power_color = gradient_get_color(0);
    }
    return model.power_color;
}