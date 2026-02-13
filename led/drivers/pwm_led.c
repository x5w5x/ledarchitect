/*
 * @Author: 轩
 * @Date: 2026-02-12 21:30:32
 * @LastEditTime: 2026-02-13 22:15:00
 * @FilePath: \led\led\drivers\pwm_led.c
 */
// pwm_led.c
#include "pwm_led.h"
// #include "stm32f10x.h"
#include"hal_pwm.h"
#include"board_led_map.h"
#include"led_context.h"

// 初始化定时器和 GPIO
static hal_pwm_channel_t pwm_init_timer_channel(GPIO_TypeDef* port, uint16_t pin) {
     for (int i = 0; i < (sizeof(g_board_pwm_map)/sizeof(g_board_pwm_map[0])); i++) {
        if (g_board_pwm_map[i].port == port && g_board_pwm_map[i].pin == pin)
            return g_board_pwm_map[i].channel;
    }
    return HAL_PWM_CH_INVALID;
    
}

// === 驱动方法（全部返回 int）===
static int pwm_init(led_driver_t* self) {
    if (!self || !self->priv) return -1;
    pwm_led_ctx_t* ctx = (pwm_led_ctx_t*)self->priv;
    // pwm_init_timer_channel(ctx->tim, ctx->channel, ctx->pin);
    // return 0;
    int ret = hal_pwm_init(ctx->channel);
    if (ret != 0) {
        return -1; // 初始化失败
    }

    // 初始关闭
    hal_pwm_set_duty(ctx->channel, ctx->inverted ? 1000 : 0);
    return 0;
}

static int pwm_set_state(led_driver_t* self, uint8_t on) {
    if (!self || !self->priv) return -1;
    pwm_led_ctx_t* ctx = (pwm_led_ctx_t*)self->priv;
    uint16_t pulse = on ? 500 : 0; // 50% or 0%
    if (ctx->inverted) pulse = 1000 - pulse;

    hal_pwm_set_duty(ctx->channel, pulse);
    return 0;

    
}

static int pwm_set_brightness(led_driver_t* self, uint8_t level) {
    if (!self || !self->priv) return -1;
    pwm_led_ctx_t* ctx = (pwm_led_ctx_t*)self->priv;

     uint16_t duty = (uint16_t)level * 1000 / 255;
    if (ctx->inverted) {
        duty = 1000 - duty;
    }

    hal_pwm_set_duty(ctx->channel, duty);
    return 0;
}

static int pwm_set_color(led_driver_t* self, const led_color_t* color) {
    // 单色 PWM 不支持颜色，可留空或返回错误
    return -1;
}

// === 创建函数 ===
led_driver_t* pwm_led_create(const led_config_t* cfg) {
    if (!cfg || !cfg->is_pwm) {
        return 0;
    }

    // 1. 映射 GPIO 到抽象 PWM 通道
    hal_pwm_channel_t ch = pwm_init_timer_channel(cfg->port, cfg->pin);
    if (ch == HAL_PWM_CH_INVALID) {
        return 0; // 不支持的引脚
    }
    pwm_led_obj_t* obj = (pwm_led_obj_t*)led_obj_alloc(CTX_TYPE_PWM);
    if (!obj) {
        return 0;
    }

    // 3. 初始化 driver 接口
    obj->drv.priv = &obj->ctx;
    obj->drv.init = pwm_init;
    obj->drv.set_state = pwm_set_state;
    obj->drv.set_brightness = pwm_set_brightness;
    obj->drv.set_color = 0; // 不支持 RGB

    // 4. 初始化上下文
    obj->ctx.channel = ch;
    obj->ctx.inverted = cfg->inverted;

    // 5. 返回 driver 地址（即对象起始地址）
    return &obj->drv;
}
