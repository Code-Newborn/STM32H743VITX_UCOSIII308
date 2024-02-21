
#include "common.h"

// 界面绘制
static display_t draw( void );
// 3按键功能
static bool down( void );
static bool up( void );
static bool select( void );

int frame_cnt;

void weatherOpen() {
    display_setDrawFunc( draw );
    buttons_setFuncs( up, select, down );

    animation_start( NULL, ANIM_MOVE_ON );
}

static display_t draw() {

    frame_cnt = frame_cnt % Animation1_FrameNum;
    draw_bitmap( 0, 0, ( const byte* )Animation1_Frames[ frame_cnt ], 64, 64, NOINVERT, 0 );  // 绘制区域
    draw_string_P( PSTR( "22:10:16" ), false, 64, 4 );
    draw_string_P( PSTR( "Harbin" ), false, 64, 16 );
    draw_string_P( PSTR( "Sunuy" ), false, 64, 28 );
    draw_string_P( PSTR( "-23C" ), false, 64, 40 );
    draw_string_P( PSTR( "24-02-20" ), false, 64, 52 );

    frame_cnt++;

    return DISPLAY_DONE;
}

static bool select( void ) {
    animation_start( back, ANIM_MOVE_OFF );
    return true;
}

static bool down() {}

static bool up() {}