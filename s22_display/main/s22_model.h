#ifndef _S22_MODEL_
#define _S22_MODEL_

#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // Directly from BLE
    void set_voltage(uint16_t voltage);
    void set_speed(uint16_t speed);
    void set_current(int16_t amps);
    void set_temp(uint16_t temp);

    // Synthesized values?
    void set_pwm(uint16_t pwm);
    void set_batt_percent(uint16_t batt_percent);

    // Getters

    uint16_t get_voltage();
    uint16_t get_speed();
    int16_t get_current();
    uint16_t get_temp();
    uint16_t get_pwm();
    uint16_t get_battery_percent();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif