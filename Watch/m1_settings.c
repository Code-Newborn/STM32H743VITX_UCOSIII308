/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#define OPTION_COUNT 6  // 当前选项数

static prev_menu_s prevMenuData;  // 上一次菜单数据

// ==================== 1 【设置菜单界面】 按键函数 ====================
static void mSelect( void );

// ==================== 2 【设置菜单界面】 内容构建 ====================
static void itemLoader( byte );

// ==================== 3 【设置菜单界面】 打开加载 ====================
void mSettingsOpen() {
    display_setDrawFunc( menu_draw );                     // 注册绘制函数 menu_draw
    buttons_setFuncs( menu_up, menu_select, menu_down );  // 注册按键功能函数

    setMenuInfo( OPTION_COUNT, MENU_TYPE_ICON, PSTR( STR_SETTINGSMENU ) );  // 设置当前菜单界面信息
    setMenuFuncs( MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader );      // 设定按键切换，当前界面加载项
    setPrevMenuOpen( &prevMenuData, mSettingsOpen );                        // 保存上一层菜单的打开功能函数

    animation_start( NULL, ANIM_MOVE_ON );
}

static void mSelect() {
    setPrevMenuExit( &prevMenuData );
    doAction( true );
}

static void itemLoader( byte num ) {
    num = 0;
    setMenuOption_P( num++, PSTR( STR_TIMEDATE ), menu_timedate, mTimeDateOpen );   // 打开日历
    setMenuOption_P( num++, PSTR( STR_SLEEP ), menu_sleep, mSleepOpen );            // 打开睡眠
    setMenuOption_P( num++, PSTR( STR_SOUND ), menu_sound, mSoundOpen );            // 打开音量
    setMenuOption_P( num++, PSTR( STR_DISPLAY ), menu_display, mDisplayOpen );      // 打开显示
    setMenuOption_P( num++, PSTR( STR_DIAGNOSTICS ), menu_diagnostic, mDiagOpen );  // 打开分析
    setMenuOption_P( num++, PSTR( STR_INFO ), menu_info, mInfoOpen );               // 打开信息

    setMenuOption_P( menuData.optionCount - 1, menuBack, menu_exit, back );  // 最后项添加返回
}
