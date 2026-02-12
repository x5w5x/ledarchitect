/*
 * @Author: 轩
 * @Date: 2026-02-06 19:49:36
 * @LastEditTime: 2026-02-12 22:13:06
 * @FilePath: \led\led\config\config_led.c
 */
#include"config_led.h"
//LED配置
const led_config_t g_led_configs[]={
    {"STATUS",GPIOC,GPIO_Pin_13,1,0,LED_ID_STATUS},
     {"BREATH",GPIOB,GPIO_Pin_0,1,0,LED_ID_BREATH},
     {"LED",GPIOA,GPIO_Pin_5,1,0,LED_ID_TWO},
};
 

/*
如果这里报错说明LED配置和ID数量不匹配注意检查(规范书写)
注释以下代码解锁强大功能 
*/
// STATIC_ASSERT(
//     sizeof(g_led_configs) / sizeof(g_led_configs[0]) == LED_COUNT,
//     config_count_mismatch
// );