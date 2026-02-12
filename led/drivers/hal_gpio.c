/*
 * @Author: 轩
 * @Date: 2026-02-12 20:46:53
 * @LastEditTime: 2026-02-12 20:49:34
 * @FilePath: \led\led\drivers\hal_gpio.c
 */
// hal_gpio.c
#include "hal_gpio.h"
#include "stm32f10x.h"  // ← 只在此文件中包含！

// 映射名字到端口（模拟 CMSIS 的 PORTA/PORTB）
hal_gpio_port_t hal_gpio_get_port_by_name(const char* name) {
    if (name == 0) return 0;
    if (name[4] == 'A') return GPIOA;
    if (name[4] == 'B') return GPIOB;
    if (name[4] == 'C') return GPIOC;
    return 0;
}

// 内部：启用时钟
static void hal_gpio_enable_clock(GPIO_TypeDef* port) {
    if (port == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if (port == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if (port == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

void hal_gpio_init(hal_gpio_port_t port, hal_gpio_pin_t pin, hal_gpio_mode_t mode) {
    GPIO_TypeDef* gpio = (GPIO_TypeDef*)port;
    if (!gpio) return;

    hal_gpio_enable_clock(gpio);

    GPIO_InitTypeDef init;
    init.GPIO_Pin = pin;
    init.GPIO_Speed = GPIO_Speed_50MHz;
    init.GPIO_Mode = (mode == HAL_GPIO_MODE_OUTPUT_PP) ? GPIO_Mode_Out_PP : GPIO_Mode_IPU;
    GPIO_Init(gpio, &init);
}

void hal_gpio_set(hal_gpio_port_t port, hal_gpio_pin_t pin, uint8_t high) {
    GPIO_TypeDef* gpio = (GPIO_TypeDef*)port;
    if (!gpio) return;
    if (high) {
        GPIO_SetBits(gpio, pin);
    } else {
        GPIO_ResetBits(gpio, pin);
    }
}

void hal_gpio_toggle(hal_gpio_port_t port, hal_gpio_pin_t pin) {
    GPIO_TypeDef* gpio = (GPIO_TypeDef*)port;
    if (!gpio) return;
    GPIO_WriteBit(gpio, pin, (BitAction)(1 - GPIO_ReadOutputDataBit(gpio, pin)));
}