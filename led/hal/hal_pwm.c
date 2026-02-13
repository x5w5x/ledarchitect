#include"hal_pwm.h"



static TIM_TypeDef* get_tim_by_channel(hal_pwm_channel_t ch){
    switch(ch){
    case HAL_PWM_CH_TIM4_CH1:
    case HAL_PWM_CH_TIM4_CH2:
    return TIM4;

    case HAL_PWM_CH_TIM3_CH1:
    case HAL_PWM_CH_TIM3_CH2:
    return TIM3;

    default:
    return 0;
    }
}



static uint8_t get_channel_index(hal_pwm_channel_t ch) {
    switch (ch) {
        case HAL_PWM_CH_TIM4_CH1:
        case HAL_PWM_CH_TIM3_CH1:
            return 1;
        case HAL_PWM_CH_TIM4_CH2:
        case HAL_PWM_CH_TIM3_CH2:
            return 2;
        default:
            return 0;
    }
}

static void config_gpio_for_pwm(hal_pwm_channel_t ch) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽

    switch (ch) {
        case HAL_PWM_CH_TIM4_CH1: // PB6
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
            GPIO_Init(GPIOB, &GPIO_InitStruct);
            break;
        case HAL_PWM_CH_TIM4_CH2: // PB7
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
            GPIO_Init(GPIOB, &GPIO_InitStruct);
            break;
        case HAL_PWM_CH_TIM3_CH1: // PA6
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
            GPIO_Init(GPIOA, &GPIO_InitStruct);
            break;
        case HAL_PWM_CH_TIM3_CH2: // PA7
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
            GPIO_Init(GPIOA, &GPIO_InitStruct);
            break;
        default:
            break;
    }
}

static void enable_timer_clock(TIM_TypeDef* tim) {
    if (tim == TIM3) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    } else if (tim == TIM4) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    }
}

int hal_pwm_init(hal_pwm_channel_t channel) {
    if (channel <= HAL_PWM_CH_INVALID) return -1;

    TIM_TypeDef* tim = get_tim_by_channel(channel);
    if (!tim) return -1;

    // 1. 配置 GPIO 为复用 PWM
    config_gpio_for_pwm(channel);

    // 2. 使能定时器时钟
    enable_timer_clock(tim);

    // 3. 配置定时器（1kHz PWM，ARR=999 → 1ms 周期）
    TIM_TimeBaseInitTypeDef TIM_BaseStruct = {0};
    TIM_BaseStruct.TIM_Prescaler = 72 - 1;      // 72MHz / 72 = 1MHz
    TIM_BaseStruct.TIM_Period = 1000 - 1;       // 1MHz / 1000 = 1kHz
    TIM_BaseStruct.TIM_ClockDivision = 0;
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(tim, &TIM_BaseStruct);

    // 4. 配置 PWM 模式
    TIM_OCInitTypeDef TIM_OCStruct = {0};
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_Pulse = 0;                 // 初始占空比 0%
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;

    uint8_t ch_idx = get_channel_index(channel);
    if (ch_idx == 1) {
        TIM_OC1Init(tim, &TIM_OCStruct);
        TIM_OC1PreloadConfig(tim, TIM_OCPreload_Enable);
    } else if (ch_idx == 2) {
        TIM_OC2Init(tim, &TIM_OCStruct);
        TIM_OC2PreloadConfig(tim, TIM_OCPreload_Enable);
    }

    // 5. 启动定时器
    TIM_Cmd(tim, ENABLE);
    TIM_CtrlPWMOutputs(tim, ENABLE); // 高级定时器需要，但 TIM3/4 不需要，可省略

    return 0;
}

void hal_pwm_set_duty(hal_pwm_channel_t channel, uint16_t duty) {
    if (duty > 1000) duty = 1000;

    TIM_TypeDef* tim = get_tim_by_channel(channel);
    if (!tim) return;

    uint16_t pulse = duty; // 因为 ARR=999，所以 Pulse = duty (0~999)

    uint8_t ch_idx = get_channel_index(channel);
    if (ch_idx == 1) {
        TIM_SetCompare1(tim, pulse);
    } else if (ch_idx == 2) {
        TIM_SetCompare2(tim, pulse);
    }
}