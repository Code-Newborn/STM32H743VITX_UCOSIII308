/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#define OPTION_COUNT getItemCount()

static prev_menu_s prevMenuData;

// 主菜单配置函数
static void mOpen( void );
static void mSelect( void );
static void itemLoader( byte );

static uint8_t getItemCount() {
    uint8_t cnt = 2;
#if COMPILE_GAME1 || COMPILE_GAME2 || COMPILE_GAME3
    ++cnt;
#endif
#if COMPILE_STOPWATCH
    ++cnt;
#endif
#if COMPILE_TORCH
    ++cnt;
#endif
#if COMPILE_BTRCCAR
    ++cnt;
#endif
#if COMPILE_TUNEMAKER
    ++cnt;
#endif
    return cnt;
}

void mMainOpen() {
    buttons_setFuncs( NULL, menu_select, NULL );  // 为按键注册函数
    beginAnimation( mOpen );                      // 关闭动画开始，并执行打开主菜单
}

// 打开主菜单界面
static void mOpen() {
    display_setDrawFunc( menu_draw );                     // 注册绘制函数 menu_draw
    buttons_setFuncs( menu_up, menu_select, menu_down );  // 注册按键功能函数

    setMenuInfo( OPTION_COUNT, MENU_TYPE_ICON, PSTR( STR_MAINMENU ) );  // 获取当前菜单项的信息（选项个数，菜单显示模式是文字还是图标）
    setMenuFuncs( MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader );  // 绑定当前菜单项的函数，如前进、后退、选择确认
    setPrevMenuOpen( &prevMenuData, mOpen );                            // 储存上级菜单
    beginAnimation2( NULL );                                            // 开启过度动画
}

// 选择确认函数
static void mSelect() {
    setPrevMenuExit( &prevMenuData );  // 储存上一次菜单的选项
    doAction( true );
}

// 主菜单加载项
static void itemLoader( byte num ) {
    num = 0;
    setMenuOption_P( num++, PSTR( STR_ALARMS ), menu_alarm, mAlarmsOpen );  // 打开闹钟
#if COMPILE_TORCH
    setMenuOption_P( num++, PSTR( STR_FLASHLIGHT ), menu_torch, torch_open );  // 打开手电
#endif
#if COMPILE_STOPWATCH
    setMenuOption_P( num++, PSTR( STR_STOPWATCH ), menu_stopwatch, stopwatch_open );  // 打开秒表
#endif
#if COMPILE_BTRCCAR
    setMenuOption_P( num++, PSTR( STR_BTRCCAR ), menu_stopwatch, btrccar_open );
#endif
// 音乐选项
#if COMPILE_TUNEMAKER
    setMenuOption_P( num++, PSTR( STR_TUNEMAKER ), menu_tunemaker, tunemakerOpen );
    setMenuOption_P( num++, PSTR( STR_TUNEMAKER ), menu_tunemaker, tunemakerOpen );  // 打开音乐
#endif
// 游戏选项
#if COMPILE_GAME1 || COMPILE_GAME2 || COMPILE_GAME3
    setMenuOption_P( num++, PSTR( STR_GAMES ), menu_games, mGamesOpen );
    setMenuOption_P( num++, PSTR( STR_GAMES ), menu_games, mGamesOpen );  // 打开游戏
#endif
    // setMenuOption_P(num++, PSTR(STR_CALCULATORS), menu_calc, calcOpen);
    setMenuOption_P( num++, PSTR( STR_SETTINGS ), menu_settings, mSettingsOpen );
    setMenuOption_P( OPTION_COUNT, PSTR( STR_EXIT ), menu_exit, menu_close );
}
