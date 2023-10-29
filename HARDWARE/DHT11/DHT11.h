#ifndef __DHT11_H
#define __DHT11_H

#include "stm32h743xx.h"
#include "core_cm7.h"
#include "stm32h7xx_hal.h"

// 采用定时器TIM2作为微秒级延时的来源

extern uint8_t humi;
extern uint8_t temp;

#define DHT11_READ_IO HAL_GPIO_ReadPin(DHT11_DQ_GPIO_Port, DHT11_DQ_Pin)

void DHT11(void);
void DHT11_Rst(void);          // 复位DHT11
uint8_t DHT11_Init(void);      // 初始化DHT11
uint8_t DHT11_Read_Data(void); // 读取温湿度
uint8_t DHT11_Read_Byte(void); // 读出一个字节
uint8_t DHT11_Read_Bit(void);  // 读出一个位
uint8_t DHT11_Check(void);     // 检测是否存在DHT11

#endif
