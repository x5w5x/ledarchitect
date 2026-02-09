/*
 * @Author: 轩
 * @Date: 2026-02-06 20:24:05
 * @LastEditTime: 2026-02-09 21:58:24
 * @FilePath: \led\led\core\led_core.h
 */
// led_core.h
#ifndef __LED_CORE_H
#define __LED_CORE_H

#include "config_led.h"
#include "led_driver.h"
#include <stdint.h>

/**
 * @brief  时间接口 V1.1
 */

 typedef struct{

    uint32_t (*get_tick_ms)(void);

 }TimeInsterface;

/**
 * @brief LED 工作模式
 */
typedef enum {
    LED_MODE_OFF = 0,        // 熄灭
    LED_MODE_ON,             // 常亮
    LED_MODE_BLINK_SLOW,     // 慢闪（1Hz）
    LED_MODE_BLINK_FAST,     // 快闪（5Hz）
    LED_MODE_BREATHING,      // 呼吸效果
    LED_MODE_FLASH_3X        // 快闪3次后熄灭（错误提示）
} led_mode_t;

/**
 * @brief 单个 LED 实例
 */
typedef struct {
    const led_config_t* config; // 配置
    led_driver_t* driver;       // 驱动
    led_mode_t mode;            // 当前模式
    uint32_t last_update_ms;    // 上次更新时间（ms）
    uint8_t blink_count;        // 用于 FLASH_3X 计数
    uint8_t breath_level;       // 呼吸当前亮度（0~100）
    int8_t breath_dir;          // 呼吸方向：+1 上升，-1 下降
    led_mode_t current_state; //当前状态 V1.1
} led_instance_t;

/**
 * @brief 公共 API
 */
int led_manager_init(const TimeInsterface* time_if);                     // 初始化所有 LED V1.1
void led_manager_update(void);                   // 主循环调用，推进状态机
void led_set_mode_by_config(const led_config_t* cfg, led_mode_t mode); // 设置模式

led_mode_t led_get_state_by_config(const led_config_t* cfg); //查询状态 V1.1

#endif