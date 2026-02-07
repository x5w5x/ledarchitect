#include"led_driver.h"
#include"config_led.h"
#include"stm32f10x.h"


typedef struct{
    GPIO_TypeDef* port;
    uint16_t pin;
    uint8_t inverted;
} gpio_led_ctx_t;

static int gpio_init(led_driver_t* self){
    gpio_led_ctx_t* ctx=(gpio_led_ctx_t*) self->priv;

    uint32_t rcc_clk=0;
     if (ctx->port == GPIOA) rcc_clk = RCC_APB2Periph_GPIOA;
    else if (ctx->port == GPIOB) rcc_clk = RCC_APB2Periph_GPIOB;
    else if (ctx->port == GPIOC) rcc_clk = RCC_APB2Periph_GPIOC;
    RCC_APB2PeriphClockCmd(rcc_clk, ENABLE);

    GPIO_InitTypeDef gpio = {0};
    gpio.GPIO_Pin = ctx->pin;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(ctx->port, &gpio);

    return 0;
    
}

static int gpio_set_state(led_driver_t* self, uint8_t on) {
    gpio_led_ctx_t* ctx = (gpio_led_ctx_t*)self->priv;
    // 处理反相逻辑
    uint8_t val = ctx->inverted ? !on : on;
    if (val) {
        GPIO_SetBits(ctx->port, ctx->pin);
    } else {
        GPIO_ResetBits(ctx->port, ctx->pin);
    }
    return 0;
}

static int gpio_set_brightness(led_driver_t* self, uint8_t level) {
    return gpio_set_state(self, level > 50 ? 1 : 0);
}

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

#define MAX_GPIO_LEDS 4

static led_driver_t g_drv_pool[MAX_GPIO_LEDS] = {0};
static gpio_led_ctx_t g_ctx_pool[MAX_GPIO_LEDS] = {0};
static uint8_t g_index = 0;

led_driver_t* gpio_led_create(const led_config_t* cfg) {
    if (g_index >= MAX_GPIO_LEDS) {
        return 0; // 超出最大数量
    }

    led_driver_t* drv = &g_drv_pool[g_index];
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