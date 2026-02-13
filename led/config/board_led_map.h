#ifndef BOARD_LED_MAP_H
#define BOARD_LED_MAP_H
#include"hal_pwm.h"

static const struct{
    GPIO_TypeDef* port;
    uint16_t pin;
    hal_pwm_channel_t channel;
}g_board_pwm_map[]={
    {GPIOB,GPIO_Pin_6,HAL_PWM_CH_TIM4_CH1},
    {GPIOB,GPIO_Pin_7,HAL_PWM_CH_TIM4_CH2},
};

#endif // !BOARD_LED_MAP_h

