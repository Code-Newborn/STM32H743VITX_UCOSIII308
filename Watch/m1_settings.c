/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#define OPTION_COUNT 5  // 当前选项数

static prev_menu_s prevMenuData;  // 上一次菜单数据

static void mSelect( void );
static void itemLoader( byte );

/**
 * @brief     : 设置菜单打开
 * @msg       : 一级菜单
 * @return     {*}
 */
void mSettingsOpen() {
    setMenuInfo( OPTION_COUNT, MENU_TYPE_ICON, PSTR( STR_SETTINGSMENU ) );  // 设置当前菜单界面信息
    setMenuFuncs( MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader );      // 设定按键切换，当前界面加载项
    setPrevMenuOpen( &prevMenuData, mSettingsOpen );                        // 保存上一层菜单的打开功能函数

    beginAnimation2( NULL );  // 设置动画初始状态
}

static void mSelect() {
    setPrevMenuExit( &prevMenuData );
    doAction( true );
}

static void itemLoader( byte num ) {
    UNUSED( num );
    setMenuOption_P( 0, PSTR( STR_TIMEDATE ), menu_timedate, mTimeDateOpen );   // 打开日历
    setMenuOption_P( 1, PSTR( STR_SLEEP ), menu_sleep, mSleepOpen );            // 打开睡眠
    setMenuOption_P( 2, PSTR( STR_SOUND ), menu_sound, mSoundOpen );            // 打开音量
    setMenuOption_P( 3, PSTR( STR_DISPLAY ), menu_display, mDisplayOpen );      // 打开显示
    setMenuOption_P( 4, PSTR( STR_DIAGNOSTICS ), menu_diagnostic, mDiagOpen );  // 打开分析

    setMenuOption_P( menuData.optionCount - 1, menuBack, menu_exit, back );  // 最后项添加返回

    // 添加选项 ...

    // setMenuOption_P( 5, PSTR( STR_UI ), NULL, NULL );
    // setMenuOption_P( 6, PSTR( STR_RCSETTINGS ), NULL, NULL );
    // setMenuOption_P( 5, PSTR( STR_EXIT ), menu_exit, menu_close );
}
