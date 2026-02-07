/*
 * @Author: 轩
 * @Date: 2026-02-06 20:31:41
 * @LastEditTime: 2026-02-06 20:31:50
 * @FilePath: \led\led\utils\delay.h
 */
#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>

void Delay_Init(void);
void Delay_ms(uint32_t ms);
uint32_t GetTickCount(void);

#endif