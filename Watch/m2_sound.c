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

static void        mSelect( void );
static void        itemLoader( byte );
static void        setVolumeUI( void );
static void        setVolumeAlarm( void );
static void        setVolumeHour( void );
static inline byte setVolume( byte );
static void        setMenuOptions( void );
static display_t   thisdraw( void );
//	static int volUI=0;
//	static int volAlarm=0;
//	static int volHour=0;

void mSoundOpen() {
    setMenuInfo( OPTION_COUNT, MENU_TYPE_ICON, PSTR( STR_SOUNDMENU ) );
    setMenuFuncs( MENUFUNC_NEXT, mSelect, MENUFUNC_PREV, itemLoader );

    menuData.func.draw = thisdraw;  // 绑定菜单画图函数
    setPrevMenuOpen( &prevMenuData, mSoundOpen );
    beginAnimation2( NULL );
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
    // 画出来了但是又被擦掉了，一直在画图
    //	char name[3];
    //	sprintf(name,"%d",appConfig.volUI);
    //	draw_string((char*)name, false, 64, FRAME_HEIGHT - 8);
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

static display_t thisdraw() {
    char name[ 3 ];

    switch ( menuData.selected ) {
    case 0:
        sprintf( name, "%d", 1 );
        break;
    case 1:
        sprintf( name, "%d", 2 );
        break;
    case 2:
        sprintf( name, "%d", 3 );
        break;
    }

    draw_string( ( char* )name, false, 122, 0 );  // 右上角绘制
    return DISPLAY_DONE;
}

static void setMenuOptions() {
    setMenuOption_P( 0, PSTR( STR_SOUND1 ), menu_volume[ 0 ], setVolumeUI );
    setMenuOption_P( 1, PSTR( STR_SOUND2 ), menu_volume[ 1 ], setVolumeAlarm );
    setMenuOption_P( 2, PSTR( STR_SOUND3 ), menu_volume[ 2 ], setVolumeHour );
}
