#ifndef _S22_PMIC_
#define _S22_PMIC_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

    typedef enum
    {
        M5_BATTERY_LEVEL_UNKNOWN,
        M5_BATTERY_LEVEL_0,
        M5_BATTERY_LEVEL_25,
        M5_BATTERY_LEVEL_50,
        M5_BATTERY_LEVEL_75,
        M5_BATTERY_LEVEL_100,
    } m5_battery_level_t;

    typedef enum
    {
        M5_BATTERY_CHARGING_UNKNOWN,
        M5_BATTERY_CHARGING_NO,
        M5_BATTERY_CHARGING_YES,
    } m5_battery_charging_t;
    
    typedef enum
    {
        M5_BATTERY_FULL_UNKNOWN,
        M5_BATTERY_FULL_NO,
        M5_BATTERY_FULL_YES,
    } m5_battery_full_t;

    typedef struct
    {
        m5_battery_charging_t charging;
        m5_battery_full_t full;
        m5_battery_level_t level;
    } m5_battery_info_t;

    void pmic_init();
    void start_pmic_task();
    m5_battery_info_t *pmic_get_info();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif