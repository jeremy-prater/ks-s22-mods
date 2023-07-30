#ifndef _S22_PMIC_
#define _S22_PMIC_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool charging;
    uint16_t battery_mv;
    uint8_t battery_percent;
} m5_battery_info_t;

void pmic_init();
m5_battery_info_t * pmic_get_info();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif