#ifndef _LED_H
#define _LED_H

#include "sys.h"

extern uint8_t key;
extern uint8_t LED2_sta;

#define LED2(n) (n ? HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)) // 设置LED状态
#define LED2_Toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1))                                                                     // LED1输出电平翻转

#endif
