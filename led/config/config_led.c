#include"config_led.h"

const led_config_t g_led_configs[LED_COUNT]={
    {"STATUS",GPIOC,GPIO_Pin_13,1,0},
     {"BREATH",GPIOB,GPIO_Pin_0,1,0}
};