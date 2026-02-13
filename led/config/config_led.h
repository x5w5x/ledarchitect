/*
 * @Author: 轩
 * @Date: 2026-02-06 19:49:26
 * @LastEditTime: 2026-02-13 22:24:09
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

#endif // !_CONFIG_LED_

