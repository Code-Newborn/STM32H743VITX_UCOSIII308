/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef UTIL_H_
#define UTIL_H_

// #define BLACK		0
// #define WHITE		1
// #define INVERT	2
#define NOINVERT false
#define INVERT true

#define F_CPU 168000000
#define sprintf_P sprintf
#define PSTR(a) a

#define max(a, b) ({typeof(a) _a = (a);\
					typeof(b) _b = (b);\
					(void)(&_a == &_b);\
					_a > _b ? _a : _b; })

#define M_PI 3.14

// Quick and easy macro for a for loop
#define LOOP(count, var) for (byte var = 0; var < count; var++)

// Reverse loop sometimes produces smaller/faster code
#define LOOPR(count, var) for (byte var = count; var--;)

#define FRAME_WIDTH 128										 // OLED 分辨率宽度
#define FRAME_HEIGHT 64										 // OLED 分辨率高度
#define FRAME_BUFFER_SIZE ((FRAME_WIDTH * FRAME_HEIGHT) / 8) // OLED 显存字节数

#endif /* UTIL_H_ */
