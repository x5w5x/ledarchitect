/*
 * @Author: 轩
 * @Date: 2026-02-06 19:49:26
 * @LastEditTime: 2026-02-11 21:47:07
 * @FilePath: \led\led\config\config_led.h
 */
#ifndef _CONFIG_LED_
#define _CONFIG_LED_

#include "stm32f10x.h"
#include "compiler.h"
//LED ID枚举 v1.2新增 优化速率
typedef enum{
    LED_ID_STATUS=0,
    LED_ID_BREATH,
    LED_ID_TWO,
    // LED_ID_ONE,
    LED_COUNT //必须有,不能使用自动计数器

}led_id_t;

typedef struct 
{
    const char* name; //名称
    GPIO_TypeDef* port; //端口
    uint16_t pin; //引脚
    uint8_t inverted; //是否反转电平
    uint8_t is_pwm;  //是否pwm控制
    led_id_t id; //v1.1新增

}led_config_t;




extern const led_config_t g_led_configs[]; //v1.1修改
//快捷宏
#define LED_STATUS (&g_led_configs[LED_ID_STATUS])
#define LED_BREATH (&g_led_configs[LED_ID_BREATH])
#define LED_TWO (&g_led_configs[LED_ID_TWO])

#endif // !_CONFIG_LED_

