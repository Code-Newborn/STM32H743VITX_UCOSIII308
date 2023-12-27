#ifndef __DHT11_H
#define __DHT11_H

#include "stm32h7xx_hal.h"

// 采用定时器TIM2作为微秒级延时的来源

extern uint8_t humi;
extern uint8_t temp;
extern uint8_t temp_decimal;
extern uint8_t humi_decimal;

#define DHT11_READ_IO HAL_GPIO_ReadPin( DHT11_DQ_GPIO_Port, DHT11_DQ_Pin )

void    DHT11( void );
void    DHT11_Rst( void );        // 复位DHT11
uint8_t DHT11_Init( void );       // 初始化DHT11
uint8_t DHT11_Read_Data( void );  // 读取温湿度
uint8_t DHT11_Read_Byte( void );  // 读出一个字节
uint8_t DHT11_Read_Bit( void );   // 读出一个位
uint8_t DHT11_Check( void );      // 检测是否存在DHT11

/**显示屏OLED 显示示例
OLED_ShowPicture(0, 0, 128, 8, BMP1);
HAL_Delay(1000);
OLED_ShowPicture(0, 0, 128, 8, BMP2);
OLED_Clear();
OLED_ShowChinese(0, 0, 0, 16);  // 中
OLED_ShowChinese(18, 0, 1, 16); // 景
OLED_ShowChinese(36, 0, 2, 16); // 园
OLED_ShowChinese(54, 0, 3, 16); // 电
OLED_ShowChinese(72, 0, 4, 16); // 子
OLED_ShowChinese(90, 0, 5, 16); // 科
OLED_Refresh();
HAL_Delay(1000);
OLED_ShowChinese(0, 0, 0, 16);   // 中
OLED_ShowChinese(18, 0, 1, 16);  // 景
OLED_ShowChinese(36, 0, 2, 16);  // 园
OLED_ShowChinese(54, 0, 3, 16);  // 电
OLED_ShowChinese(72, 0, 4, 16);  // 子
OLED_ShowChinese(90, 0, 5, 16);  // 科
OLED_ShowChinese(108, 0, 6, 16); // 技
OLED_ShowString(8, 16, "ZHONGJINGYUAN", 16);
OLED_ShowString(20, 32, "2014/05/01", 16);
OLED_ShowString(0, 48, "ASCII:", 16);
OLED_ShowString(63, 48, "CODE:", 16);
OLED_ShowChar(48, 48, t, 16); // 显示ASCII字符
t++;
if (t > '~')
    t = ' ';
OLED_ShowNum(103, 48, t, 3, 16);
OLED_Refresh();
HAL_Delay(2000);
OLED_Clear();
OLED_ShowChinese(0, 0, 0, 16);   // 16*16 中
OLED_ShowChinese(16, 0, 0, 24);  // 24*24 中
OLED_ShowChinese(24, 20, 0, 32); // 32*32 中
OLED_ShowChinese(64, 0, 0, 64);  // 64*64 中
OLED_Refresh();
HAL_Delay(2000);
OLED_Clear();
OLED_ShowString(0, 0, "ABC", 12);  // 6*12 “ABC”
OLED_ShowString(0, 12, "ABC", 16); // 8*16 “ABC”
OLED_ShowString(0, 28, "ABC", 24); // 12*24 “ABC”
OLED_Refresh();
HAL_Delay(2000);
OLED_ScrollDisplay(11, 4);
*/
#endif
