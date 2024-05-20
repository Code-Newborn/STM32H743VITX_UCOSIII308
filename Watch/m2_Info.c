#include "common.h"

#define OPTION_COUNT 10

#define IDX_MAX 20  // 文本行数量

static byte idx = 0;

static prev_menu_s prevMenuData;

static bool      down( void );
static bool      up( void );
static bool      select( void );
static display_t draw( void );
// static void      itemLoader( byte );


void mInfoOpen() {
    // menuData.func.draw = draw;
    display_setDrawFunc( draw );           // 设置绘图功能函数
    buttons_setFuncs( down, select, up );  // 绑定按键功能，重置、启停、退出

    setPrevMenuOpen( &prevMenuData, mInfoOpen );

    animation_start( NULL, ANIM_MOVE_ON );  // 打开动画动画过度
}

// static void itemLoader( byte num ) {
//     UNUSED( num );
//     showAlarmStr( num, &alarm2 );  // 遍历打印
//     addBackOption();
// }

static bool down() {
    idx--;
    if ( idx == 255 )
        idx = 1;
}

static bool up() {
    idx++;
    if ( idx == IDX_MAX )
        idx = 0;
}

static display_t draw() {
    draw_bitmap( 48, 20, close32x16, FONT_CLOSE_WIDTH, FONT_CLOSE_HEIGHT, NOINVERT, 0 );

    return DISPLAY_DONE;
}

static bool select() {
    animation_start( back, ANIM_MOVE_OFF );
    prevMenuData.lastSelected = 0;                  // Reset selected item
    menuData.prevMenu         = prevMenuData.last;  //
    return true;
}
