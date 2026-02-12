#include"led_driver.h"
#include"config_led.h"
#include"stm32f10x.h"
#include "hal_gpio.h"

//LED控制结构体
typedef struct{
    GPIO_TypeDef* port; //端口
    uint16_t pin; //引脚
    uint8_t inverted; //是否反向控制
} gpio_led_ctx_t;
//初始化引脚
static int gpio_init(led_driver_t* self){
    gpio_led_ctx_t* ctx=(gpio_led_ctx_t*) self->priv; //强制转换私有指针
    if (!ctx) return -1;

    // 使用 HAL 初始化
    hal_gpio_init(ctx->port, ctx->pin, HAL_GPIO_MODE_OUTPUT_PP);
    hal_gpio_set(ctx->port, ctx->pin, ctx->inverted ? 1 : 0); // 默认灭
    return 0;
    // uint32_t rcc_clk=0; //时钟变量
    //  if (ctx->port == GPIOA) rcc_clk = RCC_APB2Periph_GPIOA;
    // else if (ctx->port == GPIOB) rcc_clk = RCC_APB2Periph_GPIOB;
    // else if (ctx->port == GPIOC) rcc_clk = RCC_APB2Periph_GPIOC;
    // RCC_APB2PeriphClockCmd(rcc_clk, ENABLE);

    // GPIO_InitTypeDef gpio = {0};
    // gpio.GPIO_Pin = ctx->pin;
    // gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    // gpio.GPIO_Speed = GPIO_Speed_2MHz;
    // GPIO_Init(ctx->port, &gpio);

    // return 0;
    
}
//设置led状态
static int gpio_set_state(led_driver_t* self, uint8_t on) {
    gpio_led_ctx_t* ctx = (gpio_led_ctx_t*)self->priv;
    // 处理反相逻辑
    // uint8_t val = ctx->inverted ? !on : on;
    // if (val) {
    //     GPIO_SetBits(ctx->port, ctx->pin);
    // } else {
    //     GPIO_ResetBits(ctx->port, ctx->pin);
    // }
    // return 0;

    uint8_t value = on ? 1 : 0;
    if (ctx->inverted) value = !value;
    hal_gpio_set(ctx->port, ctx->pin, value);
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


// led_driver_t* gpio_led_create(const led_config_t* cfg){
//     static led_driver_t drv={0};
//     static gpio_led_ctx_t ctx={0};
//     ctx.port=cfg->port;
//     ctx.pin=cfg->pin;
//     ctx.inverted=cfg->inverted;

//     //绑定函数指针
//     drv.priv=&ctx;
//     drv.init=gpio_init;
//     drv.set_state=gpio_set_state;
//     drv.set_brightness=gpio_set_brightness;
//     drv.set_color=gpio_set_color;
//     return &drv;

// }

//最大GPIO LED数量
#define MAX_GPIO_LEDS 4

static led_driver_t g_drv_pool[MAX_GPIO_LEDS] = {0};  //静态LED驱动数组
static gpio_led_ctx_t g_ctx_pool[MAX_GPIO_LEDS] = {0}; //LED驱动池
static uint8_t g_index = 0; //LED数量计数器

//创建LED实例
led_driver_t* gpio_led_create(const led_config_t* cfg) {
    if (g_index >= MAX_GPIO_LEDS) {
        return 0; // 超出最大数量
    }

    led_driver_t* drv = &g_drv_pool[g_index]; //  获取驱动程序和上下文结构的指针
    gpio_led_ctx_t* ctx = &g_ctx_pool[g_index];

    // 初始化上下文
    ctx->port = cfg->port;
    ctx->pin = cfg->pin;
    ctx->inverted = cfg->inverted;

    // 绑定接口函数
    drv->priv = ctx;
    drv->init = gpio_init;
    drv->set_state = gpio_set_state;
    drv->set_brightness = gpio_set_brightness;
    drv->set_color = gpio_set_color;

    g_index++;
    return drv;
}