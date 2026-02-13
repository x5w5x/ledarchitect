// pwm_led.h
#ifndef __PWM_LED_H__
#define __PWM_LED_H__

#include "led_core.h"

// PWM LED 驱动创建函数
led_driver_t* pwm_led_create(const led_config_t* cfg);




#endif