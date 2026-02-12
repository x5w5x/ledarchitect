/*
 * @Author: 轩
 * @Date: 2026-02-06 19:12:36
 * @LastEditTime: 2026-02-12 22:16:03
 * @FilePath: \led\User\main.c
 */

#include "stm32f10x.h"                  // Device header
// #include "Delay.h"
#include"delay1.h"
#include "led_core.h"
#include "led_event.h"
#include"led_driver_registry.h"
static uint32_t get_tick(void){
	return GetTickCount();
}
extern void gpio_led_register(void);
extern void pwm_led_register(void);
int main(void)
{

	 Delay_Init();
	 gpio_led_register();//注册gpio_led
	 pwm_led_register();//注册pwm_led
	 TimeInsterface time_if={
		.get_tick_ms=get_tick //时间系统接口
	 };
	led_manager_init(&time_if);
	//静态LED
	  led_set_mode_by_id(LED_ID_BREATH, LED_MODE_BLINK_SLOW);
	  led_set_mode_by_id(LED_ID_STATUS,LED_MODE_BLINK_FAST);
	  led_set_mode_by_id(LED_ID_TWO,LED_MODE_BLINK_FAST);
	  //动态创建GPIO_led
	led_handle_t dynamic_led = led_create_gpio("dynamic", GPIOB, GPIO_Pin_5, 1);
	led_set_mode(dynamic_led, LED_MODE_BLINK_FAST);
	  //动态创建PWM_led
	led_handle_t pwm_led = led_create_pwm("dynamic_pwm", GPIOB, GPIO_Pin_6, 1);
	led_set_mode(pwm_led, LED_MODE_BREATHING);



	while (1)
	{
		 led_manager_update();

        // 短暂延时（避免 CPU 空转）
         Delay_ms(10);

	}
}


