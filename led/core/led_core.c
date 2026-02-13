// led_core.c
#include "led_core.h"
#include "gpio_led.h"   // 声明 gpio_led_create
#include"stdint.h"
#include "delay1.h"      // 提供 GetTickCount()
#include"string.h"
// #include"led_driver_registry.h"
#include"pwm_led.h"
// 时间常量（毫秒）
#define BLINK_SLOW_HALF_PERIOD  500  // 慢闪半周期
#define BLINK_FAST_HALF_PERIOD  100  // 快闪半周期
#define BREATH_STEP_INTERVAL    30   // 呼吸步进间隔

// 全局 LED 实例数组
// static led_instance_t g_leds[LED_COUNT];
static const TimeInsterface* g_time_if =0; //全局时间 V1.1 新增

//V2.2 驱动池
static led_instance_t g_led_pool[MAX_LED_INSTANCES];
static int g_led_used[MAX_LED_INSTANCES]={0};
static int g_instance_count=0;



/**
 * @brief 初始化所有 LED
 */
// led_err_t led_manager_init(const TimeInsterface* time_if) {
//     //初始化时间
//     if(!time_if||!time_if->get_tick_ms) return LED_ERR_INVALID_ARG;
//     g_time_if=time_if;

//     //清空内存池 V2.2新增
//     memset(g_led_pool,0,sizeof(g_led_pool));
//     memset(g_led_used,0,sizeof(g_led_used));
//     //V2.2修改
//     for (int i = 0; i < STATIC_LED_COUNT&&i<MAX_LED_INSTANCES; i++) {
//         led_instance_t* led=&g_led_pool[i];
//         led->driver=gpio_led_create(&g_led_configs[i]);
//         if(led->driver){
//             led->driver->init(led->driver);
//             led->driver->set_state(led->driver,0);
//             led->mode=LED_MODE_OFF;
//             led->last_update_ms=g_time_if->get_tick_ms();
//             led->blink_count=0;
//             led->breath_level = 0;
//             led->breath_dir = 1;
//             g_led_used[i]=1;
//             g_instance_count++;
//         }
        
//     }
//     return LED_OK;
// }


//
led_err_t led_manager_init(const TimeInsterface* time_if) {
    if (!time_if || !time_if->get_tick_ms) return LED_ERR_INVALID_ARG;
    g_time_if = time_if;

    memset(g_led_pool, 0, sizeof(g_led_pool));
    memset(g_led_used, 0, sizeof(g_led_used));

    for (int i = 0; i < STATIC_LED_COUNT && i < MAX_LED_INSTANCES; i++) {
        const led_config_t* cfg = &g_led_configs[i];
        
        // === 自动选择驱动类型 ===
        led_instance_t* led = &g_led_pool[i];
        if(!cfg->is_pwm){
                led->driver=gpio_led_create(cfg);
            }else{
                led->driver=pwm_led_create(cfg);
            }


        if (led->driver) {
            // 调用驱动方法（现在统一接口）
            if (led->driver->init(led->driver) != 0) {
                // 初始化失败？可选处理
                continue;
            }
            led->driver->set_state(led->driver, 0); // 关闭
            led->mode = LED_MODE_OFF;
            led->last_update_ms = g_time_if->get_tick_ms();
            led->blink_count = 0;
            led->breath_level = 0;
            led->breath_dir = 1;
            g_led_used[i] = 1;
            g_instance_count++;
        }
    }
    return LED_OK;
}



/**
 * @brief 更新单个 LED 状态
 */
static void led_update_single(led_instance_t* led) {
    if (!led || !led->driver || !g_time_if) return; //v2.3增加
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
           if (!led->driver || !led->driver->set_brightness) break;

                // 每 15ms 更新一次亮度（可调）
                if (now - led->last_update_ms >= 15) {
                    if (led->breath_dir) {
                        led->breath_level+=5;
                        if (led->breath_level >= 255) {
                            led->breath_level = 255;
                            led->breath_dir = 0; // 转为下降
                        }
                    } else {
                        if (led->breath_level > 0) {
                            led->breath_level-=5;
                        } else {
                            led->breath_dir = 1; // 转为上升
                        }
                    }
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
    //V2.2修改
    if(!g_time_if) return;

    for(int i=0;i<MAX_LED_INSTANCES;i++){
        if(g_led_used[i]){
            led_update_single(&g_led_pool[i]);
        }
    }
}

/**
 * @brief 通过配置指针设置 LED 模式
 */
led_err_t led_set_mode_by_id(led_id_t id, led_mode_t mode) {
    //V2.2修改
    if(id>=STATIC_LED_COUNT||!g_time_if) return LED_ERR_INVALID_ARG;

    led_instance_t* led=&g_led_pool[id];
    if(mode==LED_MODE_FLASH_3X){
        led->blink_count=0;
    }
    if (mode == LED_MODE_BREATHING) {
        led->breath_level = 0;
        led->breath_dir = 1; // 从暗到亮开始
        // led->last_update_ms = g_time_if->get_tick_ms();
    }
    led->mode=mode;
    led->last_update_ms=g_time_if->get_tick_ms();

    return LED_OK;
   
}

led_err_t led_set_mode(led_handle_t handle,led_mode_t mode){
    if(!handle||!g_time_if){
        return LED_ERR_INVALID_ARG;
    }
    led_instance_t* led=(led_instance_t*)handle;
    if(mode==LED_MODE_FLASH_3X){
        led->blink_count=0;
    }
    led->mode=mode;
    led->last_update_ms=g_time_if->get_tick_ms();
    return LED_OK;
}

//
// led_core.c —— 添加一个内部辅助函数
static led_handle_t led_create_internal(GPIO_TypeDef* port, uint16_t pin, uint8_t inverted, uint8_t is_pwm) {
    if (!g_time_if) return NULL; // 必须先调用 led_manager_init

    // 从 STATIC_LED_COUNT 开始找空闲槽（避免覆盖静态LED）
    for (int i = STATIC_LED_COUNT; i < MAX_LED_INSTANCES; i++) {
        if (!g_led_used[i]) {
            g_led_used[i] = 1;
            g_instance_count++;

            led_instance_t* led = &g_led_pool[i];

            // 构造临时配置
            led_config_t cfg = {0};
            cfg.port = port;
            cfg.pin = pin;
            cfg.inverted = inverted;
            cfg.is_pwm = is_pwm;

            // 自动选择驱动
            if(!is_pwm){
                led->driver=gpio_led_create(&cfg);
            }else{
                led->driver=pwm_led_create(&cfg);
            }

            if (!led->driver) {
                // 创建失败，回滚
                g_led_used[i] = 0;
                g_instance_count--;
                return NULL;
            }

            // 初始化驱动
            led->driver->init(led->driver);
            led->driver->set_state(led->driver, 0);

            // 初始化状态
            led->mode = LED_MODE_OFF;
            led->last_update_ms = g_time_if->get_tick_ms();
            led->blink_count = 0;
            led->breath_level = 0;
            led->breath_dir = 1;

           return (led_handle_t)led;

        }
    }
    return NULL; // 池满
}

// led_core.c —— 在文件末尾添加
led_handle_t led_create_gpio(const char* name, GPIO_TypeDef* port, uint16_t pin, uint8_t inverted) {
    (void)name; // 当前未使用 name，可忽略（未来可存入 log 或 debug）
    return led_create_internal(port, pin, inverted, 0);
}

led_handle_t led_create_pwm(const char* name, GPIO_TypeDef* port, uint16_t pin, uint8_t inverted) {
    (void)name;
    return led_create_internal(port, pin, inverted, 1);
}