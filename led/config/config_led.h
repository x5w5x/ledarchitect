#ifndef _CONFIG_LED_
#define _CONFIG_LED_

#include "stm32f10x.h"

typedef struct 
{
    const char* name;
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t inverted;
    uint8_t is_pwm;

}led_config_t;

//LED数量
#define LED_COUNT 2

extern const led_config_t g_led_configs[LED_COUNT];

#define LED_STATUS (&g_led_configs[0])
 #define LED_BREATH (&g_led_configs[1])

#endif // !1