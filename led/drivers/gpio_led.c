#include"led_driver.h"
#include"config_led.h"
#include"gpio_led.h"
#include "led_core.h" 
#include"led_pool.h"
//LED控制结构体

//初始化引脚
static int gpio_init(led_driver_t* self){
    gpio_led_ctx_t* ctx=(gpio_led_ctx_t*) self->priv; //强制转换私有指针
    if (!ctx) return -1;

    // 使用 HAL 初始化
    hal_gpio_init(ctx->port, ctx->pin, HAL_GPIO_MODE_OUTPUT_PP);
    hal_gpio_set(ctx->port, ctx->pin, ctx->inverted ? 1 : 0); // 默认灭
    return 0;

    
}
//设置led状态
static int gpio_set_state(led_driver_t* self, uint8_t on) {
    gpio_led_ctx_t* ctx = (gpio_led_ctx_t*)self->priv;
    // 处理反相逻辑
    uint8_t value = on ? 1 : 0;
    if (ctx->inverted) value = !value;
    hal_gpio_set(ctx->port, ctx->pin, value);
    return 1;
}

//设置亮度
static int gpio_set_brightness(led_driver_t* self, uint8_t level) {
    return gpio_set_state(self, level > 50 ? 1 : 0);
}
//设置颜色
static int gpio_set_color(led_driver_t* self, const led_color_t* color) {
    uint8_t on = (color->r || color->g || color->b);
    return gpio_set_state(self, on);
}


//创建LED实例
led_driver_t* gpio_led_create(const led_config_t* cfg) {
     gpio_led_pool_t* obj = (gpio_led_pool_t*)led_pool_alloc(LED_TYPE_GPIO);
    if (!obj) return 0;

    // 初始化 driver
    obj->drv.priv = &obj->ctx;
    obj->drv.init = gpio_init;
    obj->drv.set_state = gpio_set_state;
    obj->drv.set_brightness = gpio_set_brightness;

    // 初始化 ctx
    obj->ctx.port = cfg->port;
    obj->ctx.pin = cfg->pin;
    obj->ctx.inverted = cfg->inverted;

    return &obj->drv; // 返回 driver 地址
}


