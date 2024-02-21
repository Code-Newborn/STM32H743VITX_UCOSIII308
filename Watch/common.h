/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "debug.h"
#include "typedefs.h"
#include "util.h"

// #include "wdt.h"
// #include "spi.h"
// #include "i2c.h"
// #include "m_rtc.h"
// #include "adc.h"
// #include "oled.h"
#include "buttons.h"
// #include "battery.h"
// #include "buzzer.h"
// #include "led.h"
// #include "ds3231.h"
#include "functions.h"
#include "m1_alarms.h"
#include "m1_games.h"
#include "m1_settings.h"
#include "m1_stopwatch.h"
#include "m1_temphumi.h"
#include "m1_torch.h"
#include "m1_weather.h"
#include "m2_diag.h"
#include "m2_display.h"
#include "m2_game1.h"
#include "m2_game2.h"
#include "m2_game3.h"
#include "m2_sleep.h"
#include "m2_sound.h"
#include "m2_timedate.h"
#include "m_main.h"
#include "millis.h"
#include "normal.h"
// #include "lowbatt.h"
#include "m1_tunemaker.h"
#include "ui.h"
// #include "screenshot.h"    //输出oledbuff里面的数据

// #include "system.h"
#include "time.h"
#include "alarm.h"
#include "appconfig.h"
#include "display.h"
#include "global.h"
#include "pwrmgr.h"
// #include "disco.h"
#include "animation.h"
#include "draw.h"
#include "english.h"
#include "lang.h"
#include "menu.h"
#include "tune.h"
#include "tunes.h"
// #include "discos.h"
#include "DHT11/DHT11.h"
#include "LED/led.h"
#include "OLED/OLED_SSD1306.h"
#include "mpu/eMPL/inv_mpu.h"
#include "mpu/eMPL/inv_mpu_dmp_motion_driver.h"
#include "mpu/mpu6050.h"
#include "mpu/mpu_task.h"
#include "mpu/mpuiic.h"
#include "oledlib/draw_api.h"
#include "oledlib/oled_driver.h"
#include "resources.h"

extern byte OLED_GRAM[ 512 ];

// #include "wait_data_clock.h"
// #include "wifi_task.h"
// #include "wifi.h"
// #include "usart2.h"
// #include "gui_log_console.h"
// #include "wifi_cmd_task.h"

#define u32 uint32_t
#define u16 uint16_t
#define u8  uint8_t

#endif /* COMMON_H_ */
