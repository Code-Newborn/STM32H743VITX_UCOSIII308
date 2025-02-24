/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#define OPTION_COUNT 3

static prev_menu_s prevMenuData;
// ==================== 1 【音量菜单界面】 按键函数 ====================
static void        mSelect( void );
// ==================== 2 【音量菜单界面】 内容构建 ====================
static void        itemLoader( byte );

static void        setVolumeUI( void );
static void        setVolumeAlarm( void );
static void        setVolumeHour( void );
static inline byte setVolume( byte );
static void        setMenuOptions( void );
//	static int volUI=0;
//	static int volAlarm=0;
//	static int volHour=0;

// ==================== 3 【音量菜单界面】 打开加载 ====================
void mSoundOpen() {
    display_setDrawFunc( menu_draw );  // 注册绘制函数 menu_draw

    setMenuInfo( OPTION_COUNT, MENU_TYPE_ICON, PSTR( STR_SOUNDMENU ) );
    setMenuFuncs( MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader );
    setPrevMenuOpen( &prevMenuData, mSoundOpen );

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
    addBackOption();
}

static void setVolumeUI() {
    // volUI = setVolume( volUI );

    appConfig.volUI = setVolume( appConfig.volUI );
}

static void setVolumeAlarm() {
    // volAlarm = setVolume( volAlarm );

    appConfig.volAlarm = setVolume( appConfig.volAlarm );
}

static void setVolumeHour() {
    // volHour = setVolume( volHour );

    appConfig.volHour = setVolume( appConfig.volHour );
}

static byte setVolume( byte vol ) {
    vol++;
    if ( vol > 3 )
        vol = 0;
    return vol;
}

static void setMenuOptions() {
    setMenuOption_P( 0, PSTR( STR_SOUND1 ), menu_volume[ 0 ], setVolumeUI );
    setMenuOption_P( 1, PSTR( STR_SOUND2 ), menu_volume[ 1 ], setVolumeAlarm );
    setMenuOption_P( 2, PSTR( STR_SOUND3 ), menu_volume[ 2 ], setVolumeHour );
}
