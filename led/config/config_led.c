/*
 * @Author: 轩
 * @Date: 2026-02-06 19:49:36
 * @LastEditTime: 2026-02-08 19:56:36
 * @FilePath: \led\led\config\config_led.c
 */
#include"config_led.h"
//LED配置
const led_config_t g_led_configs[LED_COUNT]={
    {"STATUS",GPIOC,GPIO_Pin_13,1,0},
     {"BREATH",GPIOB,GPIO_Pin_0,1,0},
     {"LED",GPIOA,GPIO_Pin_5,1,0}
};