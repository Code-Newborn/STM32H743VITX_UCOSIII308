/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

// Frame rate when stuff is happening
// If this is too low then animations will be jerky
// Animations are also frame rate based instead of time based, adjusting frame rate will also effect animation speed
#define FRAME_RATE 60

// Frame rate when nothing is going on
// If this is too low then the interface will seem a bit slow to respond
#define FRAME_RATE_LOW 25

#define FRAME_RATE_MS     ( ( byte )( 1000 / FRAME_RATE ) )
#define FRAME_RATE_LOW_MS ( ( byte )( 1000 / FRAME_RATE_LOW ) )

static draw_f    drawFunc;
static display_f func;

byte MY_FPS = FRAME_RATE;  // 全局动画帧率控制
// #if COMPILE_GAME1
// extern display_t game1_draw();
// #endif

#if COMPILE_ANIMATIONS
typedef struct {
    byte height;
    bool closing;
    bool doingLine;
    byte lineWidth;
    byte lineClosing;
    bool active;
} crt_anim_s;

static crt_anim_s crt_anim;

static void crt_animation( void );
#endif

void display_set( display_f faceFunc ) {
    func = faceFunc;
}
// 显示 表盘
void display_load() {
    if ( func != NULL )
        func();
}

// 存储
draw_f display_setDrawFunc( draw_f func ) {
    draw_f old = drawFunc;
    drawFunc   = func;
    return old;
}

void display_update() {
    static millis8_t lastDraw;      // Time of last draw
    static byte      fpsMs, fpsms;  // Milliseconds to next draw

    // 限制帧率，过快则直接取消此次更新显示
    millis8_t now = millis();
    if ( ( millis8_t )( now - lastDraw ) < fpsMs ) {
        // pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_IDLE);
        return;
    }
    fpsms    = now - lastDraw;  // 帧率计算
    lastDraw = now;

    // debugPin_draw(HIGH);

    display_t busy = DISPLAY_DONE;

#if COMPILE_ANIMATIONS
    // Update animations
    animation_update();  // 动画更新

    // Draw stuff
    if ( drawFunc != NULL && ( crt_anim.active || ( !crt_anim.active && !crt_anim.closing ) ) )
        busy = drawFunc();

    // 关屏动画
    if ( crt_anim.active )
        crt_animation();

    if ( crt_anim.active || animation_active() )
        busy = DISPLAY_BUSY;
#else
    if ( drawFunc != NULL )
        busy = drawFunc();
#endif

    if ( appConfig.showFPS ) {
        // Work out & draw FPS, add 2ms (actually 2.31ms) for time it takes to send to OLED, clear buffer etc
        // This is only approximate
        // millis8_t end = millis() + 1;
        char buff[ 5 ];
        sprintf_P( buff, PSTR( "%u" ), ( uint )( 1000 / fpsms ) );  // 帧率显示
        draw_string( buff, false, 100, 56 );
    }
    // 结束绘制，发送至OLED
    draw_end();

    // Decide framerate
    if ( busy == DISPLAY_DONE ) {
        //	pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_NONE);
        fpsMs = FRAME_RATE_LOW_MS;
    } else {
        //	pwrmgr_setState(PWR_ACTIVE_DISPLAY, PWR_STATE_IDLE);
        fpsMs = ( byte )( 1000 / MY_FPS );
        // #if COMPILE_GAME1
        //     if (drawFunc == game1_draw)
        //       fpsMs <<= 1;
        // #endif
    }
}

#if COMPILE_ANIMATIONS
void display_startCRTAnim( crtAnim_t open ) {
    if ( !appConfig.animations ) {
        crt_anim.active = false;
        return;
    }

    if ( open == CRTANIM_OPEN )  // 开屏
    {
        crt_anim.closing     = false;
        crt_anim.doingLine   = true;
        crt_anim.height      = FRAME_HEIGHT / 2;
        crt_anim.lineClosing = false;
        crt_anim.lineWidth   = 0;
    } else  // 关屏
    {
        crt_anim.closing     = true;
        crt_anim.doingLine   = false;
        crt_anim.height      = 0;
        crt_anim.lineClosing = true;
        crt_anim.lineWidth   = FRAME_WIDTH;
    }
    crt_anim.active = true;
}

static void crt_animation() {
    byte height    = crt_anim.height;
    byte lineWidth = crt_anim.lineWidth;

    if ( !crt_anim.doingLine )  // 非线条关闭阶段，纵向关闭
    {
        if ( crt_anim.closing )  // 正在关屏
            height += 3;
        else
            height -= 3;

        if ( height >= FRAME_HEIGHT / 2 )  // 高度到位
        {
            // 未完全关闭，还有线条关闭阶段
            if ( crt_anim.closing ) {
                height             = FRAME_HEIGHT / 2;
                crt_anim.doingLine = true;
            }
            // 完全关闭
            else {
                height          = 0;
                crt_anim.active = false;
            }
            // crt_anim.closing = !crt_anim.closing;
        }
    } else  // 线条关闭阶段中
    {
        if ( crt_anim.lineClosing )  // 线条关闭中
            lineWidth -= 6;
        else
            lineWidth += 10;

        if ( lineWidth >= FRAME_WIDTH )  // 线条宽度完全关闭
        {
            // 线条首次完全关闭
            if ( crt_anim.lineClosing )
                lineWidth = 0;
            else  // 已完全关闭
                lineWidth = FRAME_WIDTH;

            if ( !crt_anim.lineClosing )  // 线条开屏
                crt_anim.doingLine = false;

            if ( crt_anim.lineClosing && crt_anim.closing )  // 处于关屏，且线条关闭阶段
                crt_anim.active = false;                     // 完全关屏

            crt_anim.lineClosing = !crt_anim.lineClosing;
        }
    }

    // 按行清除显示
    byte rows = height / 8;
    LOOP( rows, i ) {
        // 上下对称
        memset( &oledBuffer[ i * FRAME_WIDTH ], 0, FRAME_WIDTH );
        memset( &oledBuffer[ FRAME_BUFFER_SIZE - ( i + 1 ) * FRAME_WIDTH ], 0, FRAME_WIDTH );
    }

    // 8余数行 & 最后线条
    byte prows = height % 8;
    if ( prows ) {
        uint idxStart = rows * FRAME_WIDTH;
        uint idxEnd   = ( ( FRAME_BUFFER_SIZE - 1 ) - idxStart );
        byte a        = ( 255 << prows );
        byte b        = ( 255 >> prows );
        byte c        = ( 1 << prows );
        byte d        = ( 128 >> prows );
        LOOP( FRAME_WIDTH, i ) {
            oledBuffer[ idxStart ] = ( oledBuffer[ idxStart ] & a ) | c;
            idxStart++;

            oledBuffer[ idxEnd ] = ( oledBuffer[ idxEnd ] & b ) | d;
            idxEnd--;
        }
    } else if ( height )  // Edge line
    {
        uint pos = ( ( byte )( FRAME_WIDTH - lineWidth ) / 2 ) + ( ( byte )( FRAME_HEIGHT - height ) / 8 ) * FRAME_WIDTH;
        memset( &oledBuffer[ pos ], 0x01, lineWidth );

        if ( height != FRAME_HEIGHT / 2 ) {
            pos = ( height / 8 ) * FRAME_WIDTH;
            LOOPR( FRAME_WIDTH, x )
            oledBuffer[ pos + x ] |= 0x01;
        }
    }

    crt_anim.height    = height;
    crt_anim.lineWidth = lineWidth;

    //	if(crt_anim.doingLine && crt_anim.closing)
    //		draw_bitmap_s2(&crtdotImage);
}
#endif
