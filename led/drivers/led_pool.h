/*
 * @Author: 轩
 * @Date: 2026-02-13 22:01:18
 * @LastEditTime: 2026-02-14 10:16:41
 * @FilePath: \led\led\drivers\led_pool.h
 */


#ifndef LED_POOL_H
#define LED_POOL_H

#include "gpio_led.h"  // 包含 gpio_led_ctx_t
#include "pwm_led.h"   // 包含 pwm_led_ctx_t

typedef struct {
    led_driver_t drv;      // 驱动接口
    gpio_led_ctx_t ctx;    // GPIO 特有数据
} gpio_led_pool_t;

typedef struct {
    led_driver_t drv;      // 驱动接口
    pwm_led_ctx_t ctx;     // PWM 特有数据
} pwm_led_pool_t;

typedef union {
    gpio_led_pool_t gpio_obj;
    pwm_led_pool_t  pwm_obj;
} led_driver_pool_t;
typedef enum{
    LED_TYPE_NONE=0,
    LED_TYPE_GPIO,
    LED_TYPE_PWM
}led_type_t;

// typedef union{
//     struct {
//         led_driver_t drv;
//         gpio_led_ctx_t ctx;
//     } gpio;
    
//     struct {
//         led_driver_t drv;
//         pwm_led_ctx_t ctx;
//     } pwm;
// }led_driver_pool_t;

void led_pool_free(void* ptr);
void* led_pool_alloc(led_type_t type);
#endif