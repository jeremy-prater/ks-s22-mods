#ifndef _S22_MODEL_
#define _S22_MODEL_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

// Directly from BLE
void set_voltage(uint8_t voltage);
void set_speed(uint8_t speed);
void set_current(int8_t amps);
void set_temp(uint8_t temp);

// Synthesized values?
void set_pwm(uint8_t pwm);
void set_batt_percent(uint8_t batt_percent);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif