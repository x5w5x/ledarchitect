/*
 * @Author: 轩
 * @Date: 2026-02-06 20:31:55
 * @LastEditTime: 2026-02-07 22:58:59
 * @FilePath: \led\led\utils\delay1.c
 */
#include "delay1.h"
#include "stm32f10x.h"

static volatile uint32_t s_tick = 0;

void SysTick_Handler(void) {
    s_tick++;
}

void Delay_Init(void) {
    SysTick_Config(SystemCoreClock / 1000);
}

void Delay_ms(uint32_t ms) {
    uint32_t start = GetTickCount();
    while ((GetTickCount() - start) < ms);
}

uint32_t GetTickCount(void) {
    return s_tick;
}