/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#if COMPILE_ANIMATIONS

static anim_s animationStatus;

// http://javascript.info/tutorial/animation

void animation_init() {
    animationStatus.active         = false;
    animationStatus.animOnComplete = NULL;
}

// 与菜单切换动画状态相关
void animation_update() {
    if ( animationStatus.active ) {  // 切换动画被激活
        byte offsetY = animationStatus.offsetY;

        // 由上往下↓↓↓
        if ( animationStatus.goingOffScreen ) {
            if ( offsetY < 4 )
                offsetY += 1;
            else if ( offsetY < 8 )
                offsetY += 3;
            else if ( offsetY < 16 )
                offsetY += 5;
            else
                offsetY += 8;

            if ( offsetY >= FRAME_HEIGHT ) {
                animationStatus.active = false;
                offsetY                = 0;
            }
        } else {
            if ( offsetY > 255 - 4 )
                offsetY += 1;
            else if ( offsetY > 255 - 8 )
                offsetY += 3;
            else if ( offsetY > 255 - 16 )
                offsetY += 5;
            else
                offsetY += 8;

            if ( offsetY < 10 ) {  // 超限循环 256==0
                animationStatus.active = false;
                offsetY                = 0;
            }
        }

        animationStatus.offsetY = offsetY;
        if ( !animationStatus.active && animationStatus.animOnComplete != NULL ) {
            animationStatus.animOnComplete();  // 执行结尾函数后清除
            animationStatus.animOnComplete = NULL;
        }
    }
}

// 开始动画函数
// 参数：animOnComplete ，函数指针指向函数，动画过程中要执行的函数
// goingOffScreen  未知？？？？
void animation_start( void ( *animOnComplete )( void ), bool goingOffScreen ) {
    if ( appConfig.animations ) {                                   // 设置有动画
        animationStatus.active         = true;                      // 动画状态
        animationStatus.offsetY        = goingOffScreen ? 0 : 192;  // 竖直偏移 64*3=192
        animationStatus.animOnComplete = animOnComplete;            // 结束后显示函数
        animationStatus.goingOffScreen = goingOffScreen;            // 移出画面
    } else {
        if ( animOnComplete != NULL )
            animOnComplete();
    }
}

bool animation_active() {
    return animationStatus.active;
}

bool animation_movingOn() {
    return !animationStatus.goingOffScreen;
}

byte animation_offsetY() {
    return animationStatus.offsetY;
}

#else

void animation_start( void ( *animOnComplete )( void ), bool goingOffScreen ) {
    ( void )( goingOffScreen );
    if ( animOnComplete != NULL )
        animOnComplete();
}

#endif
