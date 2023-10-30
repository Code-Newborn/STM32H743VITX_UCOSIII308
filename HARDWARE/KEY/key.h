#ifndef _KEY_H
#define _KEY_H

#include "sys.h"

#define KEY1 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) // KEY1按键PE3
#define KEY2 HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) // KEY2按键PC5

#define KEY1_PRES 1 // KEY1按下后返回值
#define KEY2_PRES 2 // KEY2按下后返回值

uint8_t KEY_Scan(uint8_t mode); // 按键扫描函数
#endif
