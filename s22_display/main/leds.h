#ifndef _S22_LEDS_
#define _S22_LEDS_

#include "led_strip.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NUM_M5_LEDS 10
#define M5_LED_GPIO GPIO_NUM_15

    typedef enum
    {
        LED_SCENE_BLE_SCANNING,
        LED_SCENE_RIDING,
        LED_SCENE_WARNING,
        LED_SCENE_DANGER,
    } led_scene_t;

    void leds_init();
    void leds_set_colors(rgb_t *colors);
    void leds_set_color(uint8_t led, rgb_t color);
    void leds_set_scene(led_scene_t scene);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif