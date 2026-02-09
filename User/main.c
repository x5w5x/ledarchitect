/*
 * @Author: 轩
 * @Date: 2026-02-06 19:12:36
 * @LastEditTime: 2026-02-09 22:42:01
 * @FilePath: \led\User\main.c
 */

#include "stm32f10x.h"                  // Device header
// #include "Delay.h"
#include"delay1.h"
#include "led_core.h"
#include "led_event.h"
static uint32_t get_tick(void){
	return GetTickCount();
}
int main(void)
{

	 Delay_Init();
	 TimeInsterface time_if={
		.get_tick_ms=get_tick
	 };
	led_manager_init(&time_if);
	  led_set_mode_by_config(LED_BREATH, LED_MODE_BLINK_SLOW);
	  led_set_mode_by_config(LED_STATUS,LED_MODE_BLINK_FAST);
	  led_set_mode_by_config(LED_TWO,LED_MODE_BLINK_FAST);
	while (1)
	{
		 led_manager_update();

        // 短暂延时（避免 CPU 空转）
         Delay_ms(10);

        // // 每 1秒触发一次错误事件（模拟故障）
        static uint32_t last_error_time = 0;
        if (GetTickCount() - last_error_time > 1000) {
            // led_event_error();
            last_error_time = GetTickCount();
        }
	}
}


