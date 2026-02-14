/*
 * @Author: 轩
 * @Date: 2026-02-13 22:01:44
 * @LastEditTime: 2026-02-14 10:09:16
 * @FilePath: \led\led\drivers\led_pool.c
 */
// core/led_context.c
#include "led_pool.h"
#include <string.h>

#define MAX_CTX_INSTANCES 8  // 应 >= MAX_LED_INSTANCES

static led_driver_pool_t g_led_pool[MAX_CTX_INSTANCES];
static led_type_t      g_led_types[MAX_CTX_INSTANCES] = {LED_TYPE_NONE};

void* led_pool_alloc(led_type_t type) {
    for (int i = 0; i < MAX_CTX_INSTANCES; i++) {
        if (g_led_types[i] == LED_TYPE_NONE) {
            g_led_types[i] = type;
            memset(&g_led_pool[i], 0, sizeof(led_driver_pool_t));
            return &g_led_pool[i];
        }
    }
    return NULL;
}

// 释放函数
void led_pool_free(void* ptr) {
    if (!ptr) return;
    uintptr_t base = (uintptr_t)g_led_pool;
    uintptr_t addr = (uintptr_t)ptr;
    if (addr >= base && addr < base + sizeof(g_led_pool)) {
        int idx = (addr - base) / sizeof(led_driver_pool_t);
       g_led_types[idx] = LED_TYPE_NONE;
    }
}