#ifndef __LED_DRIVER_REGISTRY_H__
#define __LED_DRIVER_REGISTRY_H__
#include "led_core.h"

typedef enum{
    LED_DRIVER_TYPE_GPIO=0,
    LED_DRIVER_TYPE_PWM,
    LED_DRIVER_TYPE_WS2812,
    LED_DRIVER_TYPE_MAX,
}led_driver_type_t;

typedef led_driver_t* (*led_driver_create_fn)(const void* config);
//注册驱动
void led_driver_register(led_driver_type_t type,led_driver_create_fn create_fn);
//创建驱动实例
led_driver_t* led_driver_create_by_type(led_driver_type_t type,const void* config);

#endif // !__LED_DRIVER_REGISTRY_H__
