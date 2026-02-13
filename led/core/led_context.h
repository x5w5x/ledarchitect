/*
 * @Author: 轩
 * @Date: 2026-02-13 22:01:18
 * @LastEditTime: 2026-02-13 22:21:58
 * @FilePath: \led\led\core\led_context.h
 */

// core/led_context.h
#ifndef LED_CONTEXT_H
#define LED_CONTEXT_H

#include "gpio_led.h"  // 包含 gpio_led_ctx_t
#include "pwm_led.h"   // 包含 pwm_led_ctx_t
typedef union {
    gpio_led_ctx_t gpio;
    pwm_led_ctx_t  pwm;
    // 未来可加: rgb_led_ctx_t rgb;
} led_context_union_t;

// 上下文类型枚举
typedef enum {
    CTX_TYPE_NONE = 0,
    CTX_TYPE_GPIO,
    CTX_TYPE_PWM,
} ctx_type_t;
typedef struct {
    led_driver_t drv;      // 驱动接口
    gpio_led_ctx_t ctx;    // GPIO 特有数据
} gpio_led_obj_t;

typedef struct {
    led_driver_t drv;      // 驱动接口
    pwm_led_ctx_t ctx;     // PWM 特有数据
} pwm_led_obj_t;

typedef union {
    gpio_led_obj_t gpio_obj;
    pwm_led_obj_t  pwm_obj;
} led_obj_union_t;
void led_obj_free(void* ptr);
void* led_obj_alloc(ctx_type_t type);
#endif