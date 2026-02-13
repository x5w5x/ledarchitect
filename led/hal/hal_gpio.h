/*
 * @Author: 轩
 * @Date: 2026-02-12 20:38:00
 * @LastEditTime: 2026-02-13 15:18:12
 * @FilePath: \led\led\drivers\hal_gpio.h
 */
#ifndef _HAL_GPIO_H__
#define _HAL_GPIO_H__

#include"stdint.h"

//通用GPIO端口类型
typedef void* hal_gpio_port_t;

//引脚定义
typedef uint16_t hal_gpio_pin_t;

//方向
typedef enum{
    HAL_GPIO_MODE_OUTPUT_PP,
    HAL_GPIO_MODE_OUTPUT_OD,
    HAL_GPIO_MODE_INPUT,
}hal_gpio_mode_t;

//初始化
void hal_gpio_init(hal_gpio_port_t port, hal_gpio_pin_t pin, hal_gpio_mode_t mode);

//输出
void hal_gpio_set(hal_gpio_port_t port,hal_gpio_pin_t,uint8_t high);
void hal_gpio_toggle(hal_gpio_port_t port,hal_gpio_pin_t);

//获取端口

hal_gpio_port_t hal_gpio_get_port_by_name(const char* name);

#endif // !_HAL_GPIO_H__

