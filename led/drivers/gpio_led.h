/*
 * @Author: 轩
 * @Date: 2026-02-06 20:36:58
 * @LastEditTime: 2026-02-08 20:12:32
 * @FilePath: \led\led\drivers\gpio_led.h
 */
// drivers/gpio_led.h
#ifndef __GPIO_LED_H
#define __GPIO_LED_H

#include "led_driver.h"
#include "config_led.h"
#include "hal_gpio.h"
typedef struct{
    GPIO_TypeDef* port; //端口
    uint16_t pin; //引脚
    uint8_t inverted; //是否反向控制
} gpio_led_ctx_t;
//创建接口
led_driver_t* gpio_led_create(const led_config_t* cfg);

#endif