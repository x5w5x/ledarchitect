/*
 * @Author: 轩
 * @Date: 2026-02-12 21:04:28
 * @LastEditTime: 2026-02-12 21:16:49
 * @FilePath: \led\led\drivers\led_driver_registry.c
 */

#include "led_driver_registry.h"

#define MAX_DRIVER_TYPES 5

// static led_driver_create_fn g_driver_create_fns[LED_DRIVER_TYPE_MAX];
static led_driver_create_fn g_driver_create_fns[MAX_DRIVER_TYPES];
void led_driver_register(led_driver_type_t type,led_driver_create_fn create_fn){
    if(type<MAX_DRIVER_TYPES){
        g_driver_create_fns[type]=create_fn;
    }

}

led_driver_t* led_driver_create_by_type(led_driver_type_t type,const void* config){

    if(type<MAX_DRIVER_TYPES&&g_driver_create_fns[type]!=0){
        return g_driver_create_fns[type](config);
    }
    return 0;
}