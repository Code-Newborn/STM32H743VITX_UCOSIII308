/*
 * Project: Lightweight millisecond tracking library
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/millisecond-tracking-library-for-avr/
 */

#include "common.h"
#include "sys.h"
#include "led.h"
// #include "misc.h"

// 时基心跳获取

volatile millis_t milliseconds = 0;

// Get current milliseconds
millis_t millis_get()
{
	millis_t ms;
	ms = milliseconds;
	return ms;
}
