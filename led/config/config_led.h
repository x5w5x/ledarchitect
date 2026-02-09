#ifndef _CONFIG_LED_
#define _CONFIG_LED_

#include "stm32f10x.h"

typedef struct 
{
    const char* name; //名称
    GPIO_TypeDef* port; //端口
    uint16_t pin; //引脚
    uint8_t inverted; //是否反转电平
    uint8_t is_pwm;  //是否pwm控制

}led_config_t;

//LED数量
#define LED_COUNT 3

//LED配置数组
extern const led_config_t g_led_configs[LED_COUNT];

#define LED_STATUS (&g_led_configs[0])
 #define LED_BREATH (&g_led_configs[1])
 #define LED_ID (&g_led_configs[2])

#endif // !_CONFIG_LED_