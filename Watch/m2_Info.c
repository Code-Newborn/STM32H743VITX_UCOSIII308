#include "common.h"

#define OPTION_COUNT 10

#define IDX_MAX 20  // 文本行数量

static byte idx = 0;

static prev_menu_s prevMenuData;

static bool      down( void );
static bool      up( void );
static bool      select( void );
static display_t draw( void );


void mInfoOpen() {
    display_setDrawFunc( draw );
    buttons_setFuncs( up, select, down );
    setMenuInfo( OPTION_COUNT, MENU_TYPE_STR, PSTR( STR_INFOSMENU ) );
    setPrevMenuOpen( &prevMenuData, mInfoOpen );

    animation_start( NULL, ANIM_MOVE_ON );
}

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
    draw_bitmap( 25, 20, dot_11x16, FONT_DOT_WIDTH, FONT_DOT_HEIGHT, NOINVERT, 0 );

    return DISPLAY_BUSY;
}

static bool select() {
    setPrevMenuExit( &prevMenuData );
    doAction( true );
}
