#ifndef  HAL_PWM_H
#define HAL_PWM_H

#include"stdint.h"
#include"stm32f10x.h"
typedef enum{
    HAL_PWM_CH_INVALID=-1,
    HAL_PWM_CH_TIM4_CH1, //PB6
    HAL_PWM_CH_TIM4_CH2, //PB7
    HAL_PWM_CH_TIM3_CH1, //PA6
    HAL_PWM_CH_TIM3_CH2,//PA7
}hal_pwm_channel_t;

//初始化指定通道
int hal_pwm_init(hal_pwm_channel_t channel);

//设置占空比
void hal_pwm_set_duty(hal_pwm_channel_t channel,uint16_t duty);

#endif // ! HAL_PWM_H
