// core/led_context.c
#include "led_context.h"
#include <string.h>

#define MAX_CTX_INSTANCES 8  // 应 >= MAX_LED_INSTANCES

static led_obj_union_t g_obj_pool[MAX_CTX_INSTANCES];
static ctx_type_t      g_obj_types[MAX_CTX_INSTANCES] = {CTX_TYPE_NONE};

void* led_obj_alloc(ctx_type_t type) {
    for (int i = 0; i < MAX_CTX_INSTANCES; i++) {
        if (g_obj_types[i] == CTX_TYPE_NONE) {
            g_obj_types[i] = type;
            memset(&g_obj_pool[i], 0, sizeof(led_obj_union_t));
            return &g_obj_pool[i];
        }
    }
    return NULL;
}

// 释放函数
void led_obj_free(void* ptr) {
    if (!ptr) return;
    uintptr_t base = (uintptr_t)g_obj_pool;
    uintptr_t addr = (uintptr_t)ptr;
    if (addr >= base && addr < base + sizeof(g_obj_pool)) {
        int idx = (addr - base) / sizeof(led_context_union_t);
       g_obj_types[idx] = CTX_TYPE_NONE;
    }
}