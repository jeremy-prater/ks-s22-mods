#ifndef _S22_GRADIENT_
#define _S22_GRADIENT_

#include "color.h"

#ifdef __cplusplus
extern "C"
{
#endif

void gradient_init();
rgb_t * gradient_get_color(uint8_t index);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif