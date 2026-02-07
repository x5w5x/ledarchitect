#ifndef _LED_DRIVER_H
#define _LED_DRIVER_H

#include<stdint.h>

typedef struct { 
    uint8_t r,g,b;
}led_color_t;

typedef struct led_driver{
 void* priv;
 int (*init)(struct led_driver* self);
 int (*set_state)(struct led_driver* self,uint8_t on);
 int (*set_brightness)(struct led_driver* self, uint8_t level);
 int (*set_color)(struct led_driver* self, const led_color_t* color);
}led_driver_t;

#endif // !_LED_DRIVER_H

