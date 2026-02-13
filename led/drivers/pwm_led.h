// pwm_led.h
#ifndef __PWM_LED_H__
#define __PWM_LED_H__

#include "led_core.h"
#include"hal_pwm.h"
// PWM 上下文
typedef struct {
    hal_pwm_channel_t channel;
    uint8_t inverted;
} pwm_led_ctx_t;
// PWM LED 驱动创建函数
led_driver_t* pwm_led_create(const led_config_t* cfg);




#endif