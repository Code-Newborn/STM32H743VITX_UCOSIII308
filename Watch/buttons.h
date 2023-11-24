/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_
#include "typedefs.h"
#include "gpio.h"

/*下面的方式是通过直接操作库函数方式读取IO*/
// #define KEY_UP      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)   //back
#define KEY0 HAL_GPIO_ReadPin(KEY_RST_GPIO_Port, KEY_RST_Pin) // right
#define KEY1 HAL_GPIO_ReadPin(KEY_MID_GPIO_Port, KEY_MID_Pin) // ok
#define KEY2 HAL_GPIO_ReadPin(KEY_SET_GPIO_Port, KEY_SET_Pin) // left

/*下面方式是通过位带操作方式读取IO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3
#define KEY2 		PEin(2)		//P32
#define WK_UP 	PAin(0)		//PA0
*/

typedef enum
{
	BTN_1 = 0,
	BTN_2 = 1,
	BTN_3 = 2,
	BTN_4 = 3,
	BTN_5 = 4,
	BTN_6 = 5,
	BTN_7 = 6,
	BTN_COUNT = 7 // 按键总数，必须放置在最后
} btn_t;

void buttons_init(void);
void buttons_update(void);
void buttons_startup(void);
void buttons_shutdown(void);
button_f buttons_setFunc(btn_t, button_f);
void buttons_setFuncs(button_f, button_f, button_f);
// millis_t buttons_pressTime(btn_t);
bool buttons_isActive(void);
void buttons_wake(void);

#endif /* BUTTONS_H_ */
