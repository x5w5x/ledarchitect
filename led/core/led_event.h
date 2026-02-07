/*
 * @Author: 轩
 * @Date: 2026-02-06 20:27:22
 * @LastEditTime: 2026-02-06 20:27:26
 * @FilePath: \led\led\core\led_event.h
 */
// led_event.h
#ifndef __LED_EVENT_H
#define __LED_EVENT_H

/**
 * @brief 语义化 LED 事件（封装常用行为）
 */
void led_event_ready(void);          // 系统就绪：呼吸灯常亮
void led_event_error(void);          // 错误发生：状态灯快闪3次
void led_event_breathing_demo(void); // 演示呼吸效果

#endif