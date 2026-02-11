
// #include "led_core.h"
// #include "config_led.h"
// #include "gpio_led.h"
// #include <string.h>

// // ========================
// // 配置（与你一致）
// // ========================
// #define MAX_LED_INSTANCES       8
// #define STATIC_LED_COUNT        LED_COUNT   // 来自 config_led.h

// // ========================
// // 全局变量
// // ========================
// static const TimeInsterface* g_time_if = NULL;
// static led_instance_t g_led_pool[MAX_LED_INSTANCES] = {0};
// static int g_led_used[MAX_LED_INSTANCES] = {0}; // 0=free, 1=used

// // ========================
// // 内部更新逻辑（无 get_state，用内部状态）
// // ========================
// static void led_update_single(led_instance_t* led) {
//     if (!led || !led->driver || !g_time_if) return;

//     uint32_t now = g_time_if->get_tick_ms();
//     uint32_t delta = now - led->last_update_ms;

//     switch (led->mode) {
//         case LED_MODE_OFF:
//             led->driver->set_state(led->driver, 0);
//             break;

//         case LED_MODE_ON:
//             led->driver->set_state(led->driver, 1);
//             break;

//         case LED_MODE_BLINK_SLOW: // 500ms on/off
//             if (delta >= 500) {
//                 // 切换状态：读取当前模式反向（因无 get_state，用 last_state 模拟）
//                 // 但更简单：直接用时间判断
//                 uint8_t is_on = ((now / 500) % 2 == 0) ? 1 : 0;
//                 led->driver->set_state(led->driver, is_on);
//                 led->last_update_ms = now;
//             }
//             break;

//         case LED_MODE_BLINK_FAST: // 100ms on/off
//             if (delta >= 100) {
//                 uint8_t is_on = ((now / 100) % 2 == 0) ? 1 : 0;
//                 led->driver->set_state(led->driver, is_on);
//                 led->last_update_ms = now;
//             }
//             break;

//         case LED_MODE_FLASH_3X: {
//             if (led->blink_count >= 6) {
//                 led->driver->set_state(led->driver, 0);
//                 break;
//             }
//             if (delta >= 100) {
//                 uint8_t is_on = (led->blink_count % 2 == 0) ? 1 : 0;
//                 led->driver->set_state(led->driver, is_on);
//                 led->blink_count++;
//                 led->last_update_ms = now;
//             }
//             break;
//         }

//         case LED_MODE_BREATHING: {
//             // 线性模拟呼吸：每 20ms 更新一次，周期 2000ms
//             if (delta >= 20) {
//                 static uint16_t step = 0; // 注意：多个呼吸灯会冲突！
//                 // 更好的方式：在 led_instance_t 中加字段，但你说没有
//                 // 临时方案：用时间计算（不完美但可用）
//                 uint32_t t = (now / 10) % 200; // 0~199 (200 * 10ms = 2000ms)
//                 uint8_t level;
//                 if (t < 100) {
//                     level = (uint8_t)(255 * t / 100);
//                 } else {
//                     level = (uint8_t)(255 * (199 - t) / 100);
//                 }
//                 // GPIO 无法调光，>50% 亮，否则灭
//                 led->driver->set_state(led->driver, (level > 127) ? 1 : 0);
//                 led->last_update_ms = now;
//             }
//             break;
//         }

//         default:
//             led->driver->set_state(led->driver, 0);
//             break;
//     }
// }

// // ========================
// // Public API
// // ========================

// led_err_t led_manager_init(const TimeInsterface* time_if) {
//     if (!time_if || !time_if->get_tick_ms) {
//         return LED_ERR_INVALID_ARG;
//     }

    
//     g_time_if = time_if;
//     memset(g_led_pool, 0, sizeof(g_led_pool));
//     memset(g_led_used, 0, sizeof(g_led_used));

//     // 初始化静态 LED（来自 config_led.c）
//     for (int i = 0; i < STATIC_LED_COUNT && i < MAX_LED_INSTANCES; i++) {
//         led_instance_t* led = &g_led_pool[i];
//         led->driver = gpio_led_create(&g_led_configs[i]);
//         if (led->driver) {
//             led->driver->init(led->driver);
//             led->driver->set_state(led->driver, 0);
//             led->mode = LED_MODE_OFF;
//             led->last_update_ms = g_time_if->get_tick_ms();
//             led->blink_count = 0;
//             g_led_used[i] = 1;
//         }
//     }
   

//     return LED_OK;
// }

// led_err_t led_set_mode_by_config(led_id_t id, led_mode_t mode) {
//     if (id >= STATIC_LED_COUNT || !g_time_if) {
//         return LED_ERR_INVALID_ARG;
//     }

    
//     led_instance_t* led = &g_led_pool[id];
//     if (mode == LED_MODE_FLASH_3X) {
//         led->blink_count = 0;
//     }
//     led->mode = mode;
//     led->last_update_ms = g_time_if->get_tick_ms();
   

//     return LED_OK;
// }

// // 新增：运行时动态创建 GPIO LED
// led_handle_t led_create_gpio(const char* name, GPIO_TypeDef* port, uint16_t pin, uint8_t inverted) {
//     if (!g_time_if) return NULL;

    
//     // 从 STATIC_LED_COUNT 开始分配（避免覆盖静态 LED）
//     for (int i = STATIC_LED_COUNT; i < MAX_LED_INSTANCES; i++) {
//         if (!g_led_used[i]) {
//             led_instance_t* led = &g_led_pool[i];
//             memset(led, 0, sizeof(led_instance_t));

//             // 构造临时 config（仅用于 gpio_led_create）
//             led_config_t cfg;
//             cfg.name = name ? name : "dynamic";
//             cfg.port = port;
//             cfg.pin = pin;
//             cfg.inverted = inverted;
//             cfg.is_pwm = 0;

//             led->driver = gpio_led_create(&cfg);
//             if (!led->driver) {
               
//                 return NULL;
//             }

//             led->driver->init(led->driver);
//             led->driver->set_state(led->driver, 0);
//             led->mode = LED_MODE_OFF;
//             led->last_update_ms = g_time_if->get_tick_ms();
//             led->blink_count = 0;

//             g_led_used[i] = 1;
           
//             return (led_handle_t)led;
//         }
//     }
   
//     return NULL; // 池满
// }

// // 新增：通过句柄设置模式
// led_err_t led_set_mode(led_handle_t handle, led_mode_t mode) {
//     if (!handle || !g_time_if) {
//         return LED_ERR_INVALID_ARG;
//     }

    
//     led_instance_t* led = (led_instance_t*)handle;
//     if (mode == LED_MODE_FLASH_3X) {
//         led->blink_count = 0;
//     }
//     led->mode = mode;
//     led->last_update_ms = g_time_if->get_tick_ms();
   

//     return LED_OK;
// }

// void led_manager_update(void) {
//     if (!g_time_if) return;

    
//     for (int i = 0; i < MAX_LED_INSTANCES; i++) {
//         if (g_led_used[i]) {
//             led_update_single(&g_led_pool[i]);
//         }
//     }
   
// }