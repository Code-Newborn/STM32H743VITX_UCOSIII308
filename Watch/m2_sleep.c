/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#define OPTION_COUNT 1  // 定时休眠界面 菜单项数

static prev_menu_s prevMenuData;


// ==================== 1 【休眠设置菜单界面】 按键函数 ====================
static void mSelect( void );

// ==================== 2 【休眠设置菜单界面】 内容构建 ====================
static void itemLoader( byte );

static display_t mDraw( void );

static void setTimeout( void );
static void setMenuOptions( void );

// ==================== 3 【休眠设置菜单界面】 打开加载 ====================
void mSleepOpen() {
    setMenuInfo( OPTION_COUNT, MENU_TYPE_ICON, PSTR( STR_SLEEPMENU ) );
    setMenuFuncs( MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader );

    menuData.func.draw = mDraw;
    setPrevMenuOpen( &prevMenuData, mSleepOpen );

    animation_start( NULL, ANIM_MOVE_ON );
}

static void mSelect() {
    bool isExiting = exitSelected();

    if ( isExiting )
        appconfig_save();

    setPrevMenuExit( &prevMenuData );

    doAction( isExiting );
}

static void itemLoader( byte num ) {
    UNUSED( num );
    setMenuOptions();
    //	setMenuOption_P(1, menuBack, menu_exit, back);
    addBackOption();
}

static void setTimeout() {
    byte timeout = appConfig.sleepTimeout;
    timeout++;
    if ( timeout > 12 )
        timeout = 0;
    appConfig.sleepTimeout = timeout;
}

static void setMenuOptions() {
    setMenuOption_P( 0, PSTR( STR_TIMEOUT ), menu_sleeptimeout, setTimeout );
}

static display_t mDraw() {
    if ( menuData.selected == 0 ) {
        char buff[ 4 ];
        sprintf_P( buff, PSTR( "%hhuS" ), ( unsigned char )( appConfig.sleepTimeout * 5 ) );  // INFO 设置自动息屏定时 每次增加5秒
        draw_string( buff, NOINVERT, 56, 40 );
    }
    return DISPLAY_DONE;
}
