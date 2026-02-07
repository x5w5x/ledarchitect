// drivers/gpio_led.h
#ifndef __GPIO_LED_H
#define __GPIO_LED_H

#include "led_driver.h"
#include "config_led.h"

// 👇关键：返回类型是 led_driver_t*
led_driver_t* gpio_led_create(const led_config_t* cfg);

#endif