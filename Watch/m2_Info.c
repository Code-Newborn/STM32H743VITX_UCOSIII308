#include "common.h"


static byte idx = 0;

static prev_menu_s prevMenuData;

static bool      down( void );
static bool      up( void );
static bool      select( void );
static display_t draw( void );

// 信息文本行数
uint16_t rows = 0;
// 信息文本内容
char*    infos_str[] = {
    "version1.0", "User: ChenZ Zhao", "version3.0",  "version4.0",  "version5.0",  "version6.0",  "version7.0",  "version8.0",
    "version9.0", "version10.0",      "version11.0", "version12.0", "version13.0", "version14.0", "version15.0", "version16.0",
};


void mInfoOpen() {

    rows = sizeof( infos_str ) / sizeof( infos_str[ 0 ] );

    menuData.scroll      = 0;
    menuData.selected    = MAX_MENU_ITEMS - 1;
    menuData.optionCount = rows;
    menuData.title       = STR_INFOSMENU;

    display_setDrawFunc( draw );           // 屏幕绘制
    buttons_setFuncs( down, select, up );  // 按键函数注册
}

static bool down() {
    menuData.selected++;
    if ( menuData.selected >= menuData.optionCount )
        menuData.selected = MAX_MENU_ITEMS - 1;
}

static bool up() {
    menuData.selected--;
    if ( menuData.selected <= MAX_MENU_ITEMS - 2 )
        menuData.selected = menuData.optionCount - 1;
}

static display_t draw() {

    // 绘制标题
    char buff[ BUFFSIZE_STR_MENU ];
    memset( buff, ' ', sizeof( buff ) );
    strcpy( ( buff + ( 9 - ( strlen( STR_INFOSMENU ) / 2 ) ) ), STR_INFOSMENU );
    draw_string( buff, false, 0, 0 );


    // 绘制显示的文本行
    for ( size_t i = 0; i < MAX_MENU_ITEMS; i++ ) {
        draw_string( infos_str[ menuData.selected - i ], false, 0, 8 + ( MAX_MENU_ITEMS - 1 - i ) * 8 );  // 当前选择光标
    }

    return DISPLAY_BUSY;  // 返回屏幕刷新忙
}

static bool select() {
    animation_start( back, ANIM_MOVE_OFF );  // 返回
}
