// led_core.c
#include "led_core.h"
#include "gpio_led.h"   // 声明 gpio_led_create

#include "delay1.h"      // 提供 GetTickCount()

// 时间常量（毫秒）
#define BLINK_SLOW_HALF_PERIOD  500  // 慢闪半周期
#define BLINK_FAST_HALF_PERIOD  100  // 快闪半周期
#define BREATH_STEP_INTERVAL    30   // 呼吸步进间隔

// 全局 LED 实例数组
static led_instance_t g_leds[LED_COUNT];

/**
 * @brief 初始化所有 LED
 */
void led_manager_init(void) {
    for (int i = 0; i < LED_COUNT; i++) {
        const led_config_t* cfg = &g_led_configs[i];
        led_instance_t* led = &g_leds[i];

        // 保存配置
        led->config = cfg;
        led->mode = LED_MODE_OFF;
        led->last_update_ms = GetTickCount();
        led->blink_count = 0;
        led->breath_level = 0;
        led->breath_dir = 1;

        // 根据配置选择驱动
        if (cfg->is_pwm) {
            // led->driver = pwm_led_create(cfg);
        } else {
            led->driver = gpio_led_create(cfg);
        }

        // 初始化硬件并关闭 LED
        led->driver->init(led->driver);
        led->driver->set_state(led->driver, 0);
    }
}

/**
 * @brief 更新单个 LED 状态
 */
static void led_update_single(led_instance_t* led) {
    uint32_t now = GetTickCount();

    switch (led->mode) {
        case LED_MODE_OFF:
            led->driver->set_state(led->driver, 0);
            break;

        case LED_MODE_ON:
            led->driver->set_state(led->driver, 1);
            break;

        case LED_MODE_BLINK_SLOW:
        case LED_MODE_BLINK_FAST: {
            uint32_t half_period = (led->mode == LED_MODE_BLINK_SLOW) ?
                                   BLINK_SLOW_HALF_PERIOD : BLINK_FAST_HALF_PERIOD;
            // 每隔半周期翻转一次
            if (now - led->last_update_ms >= half_period) {
                uint8_t current_state = ((now / half_period) % 2);
                led->driver->set_state(led->driver, current_state);
                led->last_update_ms = now;
            }
            break;
        }

        case LED_MODE_BREATHING: {
            if (now - led->last_update_ms >= BREATH_STEP_INTERVAL) {
                // 更新呼吸亮度
                led->breath_level += led->breath_dir;
                // 边界检测：0~100
                if (led->breath_level >= 100) {
                    led->breath_level = 100;
                    led->breath_dir = -1;
                } else if (led->breath_level <= 0) {
                    led->breath_level = 0;
                    led->breath_dir = 1;
                }
                // 应用亮度
                led->driver->set_brightness(led->driver, led->breath_level);
                led->last_update_ms = now;
            }
            break;
        }

        case LED_MODE_FLASH_3X: {
            if (led->blink_count >= 6) {
                // 3 次闪烁完成（on/off ×3 = 6 次状态变化）
                led->mode = LED_MODE_OFF;
                led->driver->set_state(led->driver, 0);
                led->blink_count = 0;
            } else if (now - led->last_update_ms >= 100) {
                // 每 100ms 翻转一次
                uint8_t state = (led->blink_count % 2);
                led->driver->set_state(led->driver, state);
                led->last_update_ms = now;
                led->blink_count++;
            }
            break;
        }
    }
}

/**
 * @brief 主更新函数（必须在主循环中定期调用）
 */
void led_manager_update(void) {
    for (int i = 0; i < LED_COUNT; i++) {
        led_update_single(&g_leds[i]);
    }
}

/**
 * @brief 通过配置指针设置 LED 模式
 */
void led_set_mode_by_config(const led_config_t* cfg, led_mode_t mode) {
    for (int i = 0; i < LED_COUNT; i++) {
        if (g_leds[i].config == cfg) {
            g_leds[i].mode = mode;
            g_leds[i].last_update_ms = GetTickCount();
            // 重置特殊模式状态
            if (mode == LED_MODE_FLASH_3X) {
                g_leds[i].blink_count = 0;
            }
            if (mode == LED_MODE_BREATHING) {
                g_leds[i].breath_level = 0;
                g_leds[i].breath_dir = 1;
            }
            break;
        }
    }
}