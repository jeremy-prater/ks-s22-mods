#ifndef _S22_MODEL_
#define _S22_MODEL_

#include "stdint.h"
#include "battery.h"
#include "rgb.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void model_init();

    // M5 Hardware battery status
    void set_m5_charging(m5_battery_charging_t charging);
    void set_m5_charging_full(m5_battery_full_t full);
    void set_m5_battery_percent(m5_battery_level_t level);

    // Directly from BLE
    void set_voltage(uint16_t voltage);
    void set_speed(uint16_t speed);
    void set_current(int16_t amps);
    void set_temp(uint16_t temp);
    void set_batt_percent(uint16_t batt_percent);

    // Synthesized values?
    void set_pwm(uint16_t pwm);
    void set_batt_percent(uint16_t batt_percent);

    // Getters
    uint16_t get_voltage();
    uint16_t get_speed();
    int16_t get_current();
    uint16_t get_temp();
    uint16_t get_pwm();
    uint16_t get_batt_percent();

    rgb_t * get_speed_color();
    rgb_t * get_power_color();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif