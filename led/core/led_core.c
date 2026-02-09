// led_core.c
#include "led_core.h"
#include "gpio_led.h"   // 声明 gpio_led_create
#include"stdint.h"
#include "delay1.h"      // 提供 GetTickCount()

// 时间常量（毫秒）
#define BLINK_SLOW_HALF_PERIOD  500  // 慢闪半周期
#define BLINK_FAST_HALF_PERIOD  100  // 快闪半周期
#define BREATH_STEP_INTERVAL    30   // 呼吸步进间隔

// 全局 LED 实例数组
static led_instance_t g_leds[LED_COUNT];
static const TimeInsterface* g_time_if =0; //全局时间 V1.1 新增
/**
 * @brief 初始化所有 LED
 */
int led_manager_init(const TimeInsterface* time_if) {
    //初始化时间
    if(!time_if||!time_if->get_tick_ms) return -1;
    g_time_if=time_if;
    
    for (int i = 0; i < LED_COUNT; i++) {
        const led_config_t* cfg = &g_led_configs[i];
        //id范围检查 v1.2新增
        if(cfg->id>=LED_COUNT){
            return -2;
        }

        led_instance_t* led = &g_leds[cfg->id]; //v1.2修改

        // 保存配置
        led->config = cfg;
        led->mode = LED_MODE_OFF;
        led->last_update_ms = g_time_if->get_tick_ms(); //v1.1修改
        led->blink_count = 0;
        led->breath_level = 0;
        led->breath_dir = 1;

        // 根据配置选择驱动
        if (cfg->is_pwm) {
            // led->driver = pwm_led_create(cfg);
        } else {
            led->driver = gpio_led_create(cfg);
        }
        if(!led->driver) return -3;

        // 初始化硬件并关闭 LED
        led->driver->init(led->driver);
        led->driver->set_state(led->driver, 0);
    }
}

/**
 * @brief 更新单个 LED 状态
 */
static void led_update_single(led_instance_t* led) {
    uint32_t now = g_time_if->get_tick_ms(); //使用时间接口 V1.1修改
    led->current_state=led->mode; //同步状态 v1.1新增

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
    // for (int i = 0; i < LED_ID_MAX; i++) {
    //     led_update_single(&g_leds[i]);
    // }
    for (int i = 0; i < LED_COUNT; i++) {
        led_instance_t* led = &g_leds[g_led_configs[i].id];
        led_update_single(led);
    }
}

/**
 * @brief 通过配置指针设置 LED 模式
 */
void led_set_mode_by_config(const led_config_t* cfg, led_mode_t mode) {
    //
    if(!cfg||cfg->id>=LED_COUNT) return;
    led_instance_t* led=&g_leds[cfg->id];

    led->mode=mode;
    led->last_update_ms=g_time_if->get_tick_ms();
    if(mode==LED_MODE_FLASH_3X){
        led->blink_count=0;
    }
    if(mode==LED_MODE_BREATHING){
        led->breath_level=0;
        led->breath_dir=1;
    }

    // for (int i = 0; i < LED_ID_MAX; i++) {
    //     if (g_leds[i].config == cfg) {
    //         g_leds[i].mode = mode;
    //         g_leds[i].last_update_ms = g_time_if->get_tick_ms(); //v1.1修改
    //         // 重置特殊模式状态
    //         if (mode == LED_MODE_FLASH_3X) {
    //             g_leds[i].blink_count = 0;
    //         }
    //         if (mode == LED_MODE_BREATHING) {
    //             g_leds[i].breath_level = 0;
    //             g_leds[i].breath_dir = 1;
    //         }
    //         break;
    //     }
    // }
}

//查询状态 v1.1新增

led_mode_t led_get_state_by_config(const led_config_t* cfg){
    // for(int  i=0;i<LED_ID_MAX;i++){
    //     if(g_leds[i].config==cfg){
    //         return g_leds[i].current_state;
    //     }
    // }
    // return LED_MODE_OFF;

    if(!cfg||cfg->id>=LED_COUNT) return LED_MODE_OFF;
    return g_leds[cfg->id].current_state;
}