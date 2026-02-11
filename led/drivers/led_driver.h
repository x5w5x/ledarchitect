/*
 * @Author: 轩
 * @Date: 2026-02-06 20:00:14
 * @LastEditTime: 2026-02-11 19:35:58
 * @FilePath: \led\led\drivers\led_driver.h
 */
#ifndef _LED_DRIVER_H
#define _LED_DRIVER_H

#include<stdint.h>

//颜色配置
typedef struct { 
    uint8_t r,g,b;
}led_color_t;

//LED驱动
typedef struct led_driver{
 void* priv; //私有数据指针
 int (*init)(struct led_driver* self);   //初始化
 int (*set_state)(struct led_driver* self,uint8_t on);  //设置状态
 int (*set_brightness)(struct led_driver* self, uint8_t level); //设置亮度
 int (*set_color)(struct led_driver* self, const led_color_t* color); //设置颜色为
}led_driver_t;

//


#endif // !_LED_DRIVER_H

