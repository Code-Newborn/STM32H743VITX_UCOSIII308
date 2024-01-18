#include "DHT11.h"
#include "common.h"
#include "main.h"

#if COMPILE_TEMPHUMI

typedef enum { STATE_STOPPED = 0, STATE_RUNING } temphumi_state_t;  // 温湿度采集状态
static temphumi_state_t state;
static ulong            timer;
static millis_t         lastMS;

// 3个按键对应的功能
static bool btnReset( void );      // 重置
static bool btnStartStop( void );  // 开启/关闭测量
static bool btnExit( void );       // 退出

// 当前页面绘制函数
static display_t thisdraw( void );

void temphumi_open( void ) {
    display_setDrawFunc( thisdraw );                      // 设置绘图功能函数
    buttons_setFuncs( btnReset, btnStartStop, btnExit );  // 绑定按键功能，重置、启停、退出
    beginAnimation2( NULL );                              // 打开动画动画过度
}

bool temphumi_active( void ) {
    return ( state == STATE_RUNING );
}

static bool btnReset() {}

static bool btnStartStop( void ) {
    // 切换状态
    if ( state == STATE_RUNING ) {
        state = STATE_STOPPED;
    } else if ( state == STATE_STOPPED ) {
        state = STATE_RUNING;
    }
    lastMS = millis();
    return true;
}
static bool btnExit( void ) {
    animation_start( back, ANIM_MOVE_OFF );
    return true;
}

void temphumi_update( void ) {
    // 处于采集开启状态
    if ( state == STATE_RUNING ) {
        millis_t now = millis();
        timer += now - lastMS;
        lastMS = now;
        if ( timer > 359999999 )  // 99 hours, 59 mins, 59 secs, 999 ms
            timer = 359999999;
    }
}

#define SHOW_POS_X 1
#define SHOW_POS_Y 20
static display_t thisdraw() {

    byte  num1;
    byte  num2;
    byte  num3;
    ulong timer1 = timer / 1;  // 模拟一秒的时间
    ulong secs   = timer1 / 1000;

    if ( timer1 < 3600000 ) {
        num3 = ( timer1 % 1000 ) / 10;  // ms
        num2 = secs % 60;               // secs
        num1 = secs / 60;               // mins
    } else {
        num3 = secs % 60;           // secs
        num2 = ( secs / 60 ) % 60;  // mins
        num1 = ( secs / 3600 );     // hours
    }

    // draw_bitmap( 1, TIME_POS_Y, midFont[ div10( num1 ) ], MIDFONT_WIDTH, MIDFONT_HEIGHT, NOINVERT, 0 );
    // draw_bitmap( 24, TIME_POS_Y, midFont[ mod10( num1 ) ], MIDFONT_WIDTH, MIDFONT_HEIGHT, NOINVERT, 0 );
    // draw_bitmap( 60, TIME_POS_Y, midFont[ div10( num2 ) ], MIDFONT_WIDTH, MIDFONT_HEIGHT, NOINVERT, 0 );
    // draw_bitmap( 83, TIME_POS_Y, midFont[ mod10( num2 ) ], MIDFONT_WIDTH, MIDFONT_HEIGHT, NOINVERT, 0 );
    draw_bitmap( 1, SHOW_POS_Y, small2Font[ div10( num3 ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( 13, SHOW_POS_Y, small2Font[ mod10( num3 ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( 25, SHOW_POS_Y, small2Font[ div10( num3 ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( 37, SHOW_POS_Y, small2Font[ mod10( num3 ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( SHOW_POS_X + 46 + 2, SHOW_POS_Y, celsius, FONT_CELSIUS_WIDTH, FONT_CELSIUS_HEIGHT, NOINVERT, 0 );

    draw_bitmap( 1, SHOW_POS_Y + 18 + 2, small2Font[ div10( num3 ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( 13, SHOW_POS_Y + 18 + 2, small2Font[ mod10( num3 ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( 25, SHOW_POS_Y + 18 + 2, small2Font[ div10( num3 ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );
    draw_bitmap( 37, SHOW_POS_Y + 18 + 2, small2Font[ mod10( num3 ) ], FONT_SMALL2_WIDTH, FONT_SMALL2_HEIGHT, NOINVERT, 0 );

    draw_bitmap( SHOW_POS_X + 46 + 2, SHOW_POS_Y + 18 + 2, celsius, FONT_CELSIUS_WIDTH, FONT_CELSIUS_HEIGHT, NOINVERT, 0 );
    // 顶部显示实际时间
    draw_string( time_timeStr(), NOINVERT, 48, 0 );

    return DISPLAY_BUSY;
}

#endif
