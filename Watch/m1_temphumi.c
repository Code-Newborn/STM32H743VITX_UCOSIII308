#include "common.h"
#include "main.h"

#if COMPILE_TEMPHUMI

typedef enum { STATE_STOPPED = 0, STATE_RUNING } temphumi_state_t;  // 温湿度采集状态
static temphumi_state_t state;

// 3个按键对应的功能
static bool btnReset( void );      // 重置
static bool btnStartStop( void );  // 开启/关闭测量
static bool btnExit( void );       // 退出

// 当前页面绘制函数
static display_t thisdraw( void );

void temphumi_open( void ) {
    display_setDrawFunc( thisdraw );                      // 设置绘图功能函数
    buttons_setFuncs( btnReset, btnStartStop, btnExit );  // 绑定按键功能，重置、启停、退出

    animation_start( NULL, ANIM_MOVE_ON );  // 打开动画动画过度
    DHT11_run = 1;                          // 进入界面开启温湿度采集
}

bool temphumi_IsActive( void ) {
    return ( DHT11_run == STATE_RUNING );
}

static bool btnReset() {}

static bool btnStartStop( void ) {
    // 切换开启/关闭状态
    if ( DHT11_run == STATE_RUNING ) {
        DHT11_run = STATE_STOPPED;
    } else if ( state == STATE_STOPPED ) {
        DHT11_run = STATE_RUNING;
    }
    return true;
}
static bool btnExit( void ) {
    animation_start( back, ANIM_MOVE_OFF );
    DHT11_run = 0;  // 关闭采集
    return true;
}

#define SHOW_POS_X 25
#define SHOW_POS_Y 20
static display_t thisdraw() {

    if ( DHT11_run )  // 状态指示
        draw_bitmap( 48, 0, close32x16, FONT_CLOSE_WIDTH, FONT_CLOSE_HEIGHT, NOINVERT, 0 );
    else
        draw_bitmap( 48, 0, open32x16, FONT_CLOSE_WIDTH, FONT_CLOSE_HEIGHT, NOINVERT, 0 );

    draw_bitmap( SHOW_POS_X + 1, SHOW_POS_Y, small2Font[ div10( temp ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( SHOW_POS_X + 13, SHOW_POS_Y, small2Font[ mod10( temp ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( SHOW_POS_X + 25, SHOW_POS_Y, dot_11x16, FONT_DOT_WIDTH, FONT_DOT_HEIGHT, NOINVERT, 0 );
    // 温度分辨率只有0.1，需要交换显示位置
    draw_bitmap( SHOW_POS_X + 37, SHOW_POS_Y, small2Font[ mod10( temp_decimal ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( SHOW_POS_X + 49, SHOW_POS_Y, small2Font[ div10( temp_decimal ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( SHOW_POS_X + 61, SHOW_POS_Y, celsius16x16, FONT_CELSIUS_WIDTH, FONT_CELSIUS_HEIGHT, NOINVERT, 0 );

    draw_bitmap( SHOW_POS_X + 1, SHOW_POS_Y + 20, small2Font[ div10( humi ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( SHOW_POS_X + 13, SHOW_POS_Y + 20, small2Font[ mod10( humi ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( SHOW_POS_X + 25, SHOW_POS_Y + 20, dot_11x16, FONT_DOT_WIDTH, FONT_DOT_HEIGHT, NOINVERT, 0 );
    draw_bitmap( SHOW_POS_X + 37, SHOW_POS_Y + 20, small2Font[ div10( humi_decimal ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( SHOW_POS_X + 49, SHOW_POS_Y + 20, small2Font[ mod10( humi_decimal ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( SHOW_POS_X + 61, SHOW_POS_Y + 20, humiUnit16x16, FONT_CELSIUS_WIDTH, FONT_CELSIUS_HEIGHT, NOINVERT, 0 );
    // 顶部显示实际时间
    // draw_string( time_timeStr(), NOINVERT, 48, 0 );

    return DISPLAY_BUSY;
}

#endif
