// /*
//  * @Author: 轩
//  * @Date: 2026-02-06 20:27:33
//  * @LastEditTime: 2026-02-08 16:48:26
//  * @FilePath: \led\led\core\led_event.c
//  */
// // led_event.c
// #include "led_event.h"
// #include "led_core.h"
// #include "config_led.h"

// /**
//  * @brief 系统就绪：状态灯灭，呼吸灯常亮
//  */
// void led_event_ready(void) {
//     led_set_mode_by_config(LED_STATUS, LED_MODE_OFF);
//     led_set_mode_by_config(LED_BREATH, LED_MODE_ON);
// }

// /**
//  * @brief 错误提示：状态灯快闪3次，呼吸灯灭
//  */
// void led_event_error(void) {
//     led_set_mode_by_config(LED_STATUS, LED_MODE_FLASH_3X);
//     led_set_mode_by_config(LED_BREATH, LED_MODE_OFF);
// }

// /**
//  * @brief 演示呼吸效果：状态灯灭，呼吸灯进入呼吸模式
//  */
// void led_event_breathing_demo(void) {
//     led_set_mode_by_config(LED_STATUS, LED_MODE_OFF);
//     led_set_mode_by_config(LED_BREATH, LED_MODE_BREATHING);
// }