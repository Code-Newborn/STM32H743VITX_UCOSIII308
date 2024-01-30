/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef RESOURCES_H_
#define RESOURCES_H_
#include "english.h"
#include "typedefs.h"

// 默认
extern const byte menu_default[];

// 一级菜单 闹钟
extern const byte menu_alarm[];

// 一级菜单 手电筒
extern const byte menu_torch[];

// 一级菜单 秒表
extern const byte menu_stopwatch[];

// 一级菜单 音乐
extern const byte menu_tunemaker[];

// 一级菜单 游戏
extern const byte menu_games[];
// 二级菜单 游戏1
extern const byte menu_game1[];
// 二级菜单 游戏2
extern const byte menu_game2[];
// 二级菜单 游戏3
extern const byte menu_game3[];

// 一级菜单 设置
extern const byte menu_settings[];
// 二级菜单 时间日期
extern const byte menu_timedate[];
// 二级菜单 睡眠
extern const byte menu_sleep[];
// 二级菜单 音量
extern const byte menu_sound[];
// 二级菜单 显示
extern const byte menu_display[];
// 三级菜单 亮度
extern const byte menu_brightness[][ 128 ];
// 三级菜单 翻转
extern const byte menu_invert[];
// 三级菜单 旋转
extern const byte menu_rotate[];
// 三级菜单 动画
extern const byte menu_anim[][ 128 ];
// 三级菜单 帧率
extern const byte menu_setfps[];
// 三级菜单 LED
extern const byte menu_LEDs[][ 128 ];
// 二级菜单 分析
extern const byte menu_diagnostic[];

// 一级菜单 退出
extern const byte menu_exit[];

extern const byte selectbar_bottom[];
extern const byte selectbar_top[];

extern const char dowChars[];

extern const char days[ 7 ][ BUFFSIZE_STR_DAYS ];
extern const char months[ 12 ][ BUFFSIZE_STR_MONTHS ];

extern const byte livesImg[];
extern const byte stopwatch[];

extern const byte dowImg[ 7 ][ 8 ];

extern const byte menu_TandH[];

// 二级菜单
extern const byte menu_volume[][ 128 ];

extern const byte menu_sleeptimeout[];

extern const byte usbIcon[];
extern const byte chargeIcon[];
extern const byte signalIcon[];
extern const byte noSignalIcon[];

extern const byte battIconEmpty[];
extern const byte battIconLow[];
extern const byte battIconHigh[];
extern const byte battIconFull[];

// Alarm icon
// extern const byte smallFontAlarm[];

#define SMALLFONT_WIDTH  5
#define SMALLFONT_HEIGHT 8
extern const byte smallFont[][ 5 ];

#define MIDFONT_WIDTH  19
#define MIDFONT_HEIGHT 24
extern const byte midFont[][ 57 ];

#define SEGFONT_WIDTH  19
#define SEGFONT_HEIGHT 24
extern const byte segFont[][ 57 ];

#define FONT_SMALL2_WIDTH  11
#define FONT_SMALL2_HEIGHT 16
extern const byte small2Font[][ 22 ];

#define FONT_COLON_WIDTH  6
#define FONT_COLON_HEIGHT 24
extern const byte colon[];

#define FONT_CELSIUS_WIDTH  16
#define FONT_CELSIUS_HEIGHT 16
extern const byte celsius16x16[];

#define FONT_HUMIUNIT_WIDTH  16
#define FONT_HUMIUNIT_HEIGHT 16
extern const byte humiUnit16x16[];

#define FONT_CLOSE_WIDTH  32
#define FONT_CLOSE_HEIGHT 16
extern const byte close32x16[];

#define FONT_OPEN_WIDTH  32
#define FONT_OPEN_HEIGHT 16
extern const byte open32x16[];

#define FONT_DOT_WIDTH  11
#define FONT_DOT_HEIGHT 16
extern const byte dot_11x16[];

#endif /* RESOURCES_H_ */
