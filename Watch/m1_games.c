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

static void mSelect( void );
static void itemLoader( byte );

static uint8_t getItemCount() {
    uint8_t cnt = 0;
#if COMPILE_GAME1
    ++cnt;
#endif
#if COMPILE_GAME2
    ++cnt;
#endif
#if COMPILE_GAME3
    ++cnt;
#endif
    return cnt;
}

void mGamesOpen() {
    display_setDrawFunc( menu_draw );                     // 注册绘制函数 menu_draw
    buttons_setFuncs( menu_up, menu_select, menu_down );  // 注册按键功能函数

    setMenuInfo( OPTION_COUNT, MENU_TYPE_ICON, PSTR( STR_GAMESMENU ) );
    setMenuFuncs( MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader );
    setPrevMenuOpen( &prevMenuData, mGamesOpen );  // 保存当前菜单打开函数

    animation_start( NULL, ANIM_MOVE_ON );
}

static void mSelect() {
    setPrevMenuExit( &prevMenuData );
    doAction( true );
}

static void itemLoader( byte num ) {
    num = 0;
#if COMPILE_GAME1
    setMenuOption_P( num++, PSTR( GAME1_NAME ), menu_game1, game1_start );
#endif
#if COMPILE_GAME2
    setMenuOption_P( num++, PSTR( GAME2_NAME ), menu_game2, game2_start );
#endif
#if COMPILE_GAME3
    setMenuOption_P( num++, PSTR( GAME3_NAME ), menu_game3, game3_start );
#endif
    setMenuOption_P( menuData.optionCount - 1, menuBack, menu_exit, back );  // 返回上级选项
}
