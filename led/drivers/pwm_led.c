/*
 * @Author: 轩
 * @Date: 2026-02-12 21:30:32
 * @LastEditTime: 2026-02-12 21:34:17
 * @FilePath: \led\led\drivers\pwm_led.c
 */
// pwm_led.c
// pwm_led.c
#include "pwm_led.h"
#include "stm32f10x.h"
#include "led_driver_registry.h"

// PWM 上下文
typedef struct {
    TIM_TypeDef* tim;
    uint8_t channel;
    uint16_t pin;
    GPIO_TypeDef* port;
    uint8_t inverted;
} pwm_led_ctx_t;

#define MAX_PWM_LEDS 2
static led_driver_t g_pwm_drv_pool[MAX_PWM_LEDS];
static pwm_led_ctx_t g_pwm_ctx_pool[MAX_PWM_LEDS];

// 初始化定时器和 GPIO
static void pwm_init_timer_channel(TIM_TypeDef* tim, uint8_t channel, uint16_t pin) {
    if (tim == TIM4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    if (pin == GPIO_Pin_6 || pin == GPIO_Pin_7) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    }

    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Pin = pin;
    gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_init);

    TIM_TimeBaseInitTypeDef tim_base;
    tim_base.TIM_Period = 999;
    tim_base.TIM_Prescaler = 71;
    tim_base.TIM_ClockDivision = 0;
    tim_base.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(tim, &tim_base);

    TIM_OCInitTypeDef oc_init;
    oc_init.TIM_OCMode = TIM_OCMode_PWM1;
    oc_init.TIM_OutputState = TIM_OutputState_Enable;
    oc_init.TIM_Pulse = 0;
    oc_init.TIM_OCPolarity = TIM_OCPolarity_High;

    if (channel == 1) TIM_OC1Init(tim, &oc_init);
    else if (channel == 2) TIM_OC2Init(tim, &oc_init);

    TIM_Cmd(tim, ENABLE);
    if (channel == 1) TIM_OC1PreloadConfig(tim, TIM_OCPreload_Enable);
    else if (channel == 2) TIM_OC2PreloadConfig(tim, TIM_OCPreload_Enable);
}

// === 驱动方法（全部返回 int）===
static int pwm_init(led_driver_t* self) {
    if (!self || !self->priv) return -1;
    pwm_led_ctx_t* ctx = (pwm_led_ctx_t*)self->priv;
    pwm_init_timer_channel(ctx->tim, ctx->channel, ctx->pin);
    return 0;
}

static int pwm_set_state(led_driver_t* self, uint8_t on) {
    if (!self || !self->priv) return -1;
    pwm_led_ctx_t* ctx = (pwm_led_ctx_t*)self->priv;
    uint16_t pulse = on ? 500 : 0; // 50% or 0%
    if (ctx->inverted) pulse = 1000 - pulse;

    if (ctx->channel == 1) {
        TIM_SetCompare1(ctx->tim, pulse);
    } else if (ctx->channel == 2) {
        TIM_SetCompare2(ctx->tim, pulse);
    }
    return 0;
}

static int pwm_set_brightness(led_driver_t* self, uint8_t level) {
    if (!self || !self->priv) return -1;
    pwm_led_ctx_t* ctx = (pwm_led_ctx_t*)self->priv;
    uint16_t pulse = (uint16_t)level * 1000 / 255; // 映射 0~255 → 0~1000
    if (ctx->inverted) pulse = 1000 - pulse;

    if (ctx->channel == 1) {
        TIM_SetCompare1(ctx->tim, pulse);
    } else if (ctx->channel == 2) {
        TIM_SetCompare2(ctx->tim, pulse);
    }
    return 0;
}

static int pwm_set_color(led_driver_t* self, const led_color_t* color) {
    // 单色 PWM 不支持颜色，可留空或返回错误
    return -1;
}

// === 创建函数 ===
led_driver_t* pwm_led_create(const led_config_t* cfg) {
    if (!cfg || !cfg->is_pwm) return 0;

    TIM_TypeDef* tim = 0;
    uint8_t channel = 0;
    if (cfg->port == GPIOB && cfg->pin == GPIO_Pin_6) {
        tim = TIM4; channel = 1;
    } else if (cfg->port == GPIOB && cfg->pin == GPIO_Pin_7) {
        tim = TIM4; channel = 2;
    } else {
        return 0;
    }

    for (int i = 0; i < MAX_PWM_LEDS; i++) {
        if (g_pwm_drv_pool[i].priv == 0) { // ← 使用 priv，不是 ctx！
            g_pwm_ctx_pool[i].tim = tim;
            g_pwm_ctx_pool[i].channel = channel;
            g_pwm_ctx_pool[i].pin = cfg->pin;
            g_pwm_ctx_pool[i].port = cfg->port;
            g_pwm_ctx_pool[i].inverted = cfg->inverted;

            g_pwm_drv_pool[i].priv = &g_pwm_ctx_pool[i]; // ← 关键：赋值给 priv
            g_pwm_drv_pool[i].init = pwm_init;
            g_pwm_drv_pool[i].set_state = pwm_set_state;
            g_pwm_drv_pool[i].set_brightness = pwm_set_brightness;
            g_pwm_drv_pool[i].set_color = pwm_set_color;

            return &g_pwm_drv_pool[i];
        }
    }
    return 0;
}

// === 注册 ===
static led_driver_t* pwm_led_create_wrapper(const void* config) {
    return pwm_led_create((const led_config_t*)config);
}

void pwm_led_register(void) {
    led_driver_register(LED_DRIVER_TYPE_PWM, pwm_led_create_wrapper);
}