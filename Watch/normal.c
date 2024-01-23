/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

#define TIME_POS_X 1
#define TIME_POS_Y 20
#define TICKER_GAP 4  // 数字切换时间隔距离

typedef struct {
    byte        x;
    byte        y;
    const byte* bitmap;
    byte        w;
    byte        h;
    byte        offsetY;
    byte        val;
    byte        maxVal;
    bool        moving;
} tickerData_t;

static display_t draw( void );
static void      drawDate( void );
#if COMPILE_ANIMATIONS
static bool animateIcon( bool, byte* );
#endif
static display_t ticker( void );
static void      drawTickerNum( tickerData_t* );

#include "mpu_task.h"
// 在主界面进入mpu界面
static bool mpu_show( void ) {
    // beginAnimation(mpu_open);
    return true;
}
static bool wifi_show( void ) {
    // beginAnimation(wifi_test_open);
    return true;
}

// 表盘界面
void watchface_normal() {
    display_setDrawFunc( draw );                           // 注册绘制函数
    buttons_setFuncs( mpu_show, menu_select, wifi_show );  // 注册进入菜单函数 不同按键进入不同主菜单，已注释其中两个
    animation_start( NULL, ANIM_MOVE_ON );                 // 设置打开过渡动画，（不执行函数）
}

static display_t draw() {
#if COMPILE_ANIMATIONS
    static byte usbImagePos_y    = FRAME_HEIGHT;
    static byte chargeImagePos_y = FRAME_HEIGHT;
#endif

    drawDate();  // 绘制日期

    display_t busy;   // 绘制忙 标志
    busy = ticker();  // 秒钟滴答刷新绘制

    drawBattery();  // 绘制电源图标

    byte x = 20;

#if COMPILE_ANIMATIONS
    //	if(animateIcon(UDADDR != 0, &usbImagePos))
    if ( animateIcon( 1, &usbImagePos_y ) ) {
        draw_bitmap( x, usbImagePos_y, usbIcon, 16, 8, NOINVERT, 0 );
        x += 20;
    }
#else
    //	if(UDADDR != 0())
    //	{
    draw_bitmap( x, FRAME_HEIGHT - 9, usbIcon, 16, 8, NOINVERT, 0 );
    x += 20;
//	}
#endif

    // Draw charging icon
#if COMPILE_ANIMATIONS
    //	if(animateIcon(CHARGING(), &chargeImagePos))
    if ( animateIcon( 1, &chargeImagePos_y ) ) {
        draw_bitmap( x, chargeImagePos_y, chargeIcon, 8, 8, NOINVERT, 0 );
        x += 12;
    }
#else
    //	if(CHARGING())
    {
        draw_bitmap( x, FRAME_HEIGHT - 9, chargeIcon, 8, 8, NOINVERT, 0 );
        x += 12;
    }
#endif

#if COMPILE_STOPWATCH
    // Stopwatch icon
    if ( stopwatch_active() ) {
        draw_bitmap( x, FRAME_HEIGHT - 8, stopwatch, 8, 8, NOINVERT, 0 );
        x += 12;
    }
#endif

    // Draw next alarm
    //	alarm_s nextAlarm;
    //	if(alarm_getNext(&nextAlarm))
    //	{
    //		time_s alarmTime;
    //		alarmTime.hour = nextAlarm.hour;
    //		alarmTime.mins = nextAlarm.min;
    //		alarmTime.ampm = CHAR_24;
    ////		time_timeMode(&alarmTime, appConfig.timeMode);TIMEMODE_24HR
    //		time_timeMode(&alarmTime, TIMEMODE_24HR);

    //
    //		char buff[9];
    //		sprintf_P(buff, PSTR("%02hhu:%02hhu%c"), alarmTime.hour, alarmTime.mins, alarmTime.ampm);
    //		draw_string(buff, false, x, FRAME_HEIGHT - 8);

    //		x += (alarmTime.ampm == CHAR_24) ? 35 : 42;
    //	//	draw_bitmap(x, FRAME_HEIGHT - 8, dowImg[alarm_getNextDay()], 8, 8, NOINVERT, 0);
    //				draw_bitmap(x, FRAME_HEIGHT - 8, dowImg[1], 8, 8, NOINVERT, 0);

    ////		x += 9;
    //	}

    return busy;
}

static void drawDate() {
    // 获取 星期 字符串
    char day[ BUFFSIZE_STR_DAYS ];
    strcpy( day, days[ timeDate.date.day ] );

    // 获取 月份 字符串
    char month[ BUFFSIZE_STR_MONTHS ];
    strcpy( month, months[ timeDate.date.month ] );

    // 获取 星期、天数、月份、年份 字符串
    char buff[ BUFFSIZE_DATE_FORMAT ];
    sprintf_P( buff, PSTR( DATE_FORMAT ), day, timeDate.date.date, month, timeDate.date.year );
    draw_string( buff, false, 12, 0 );
}

#if COMPILE_ANIMATIONS
static bool animateIcon( bool active, byte* pos ) {
    byte y = *pos;
    if ( active || ( !active && y < FRAME_HEIGHT ) ) {
        if ( active && y > FRAME_HEIGHT - 8 )
            y -= 1;
        else if ( !active && y < FRAME_HEIGHT )
            y += 1;
        *pos = y;
        return true;
    }
    return false;
}
#endif

// 手表滴答显示
static display_t ticker() {
    static byte yPos;       // 时、分数字刷新相对位置
    static byte yPos_secs;  // 秒数字刷新相对位置
    static bool moving = false;
    static bool moving2[ 5 ];

#if COMPILE_ANIMATIONS
    static byte hour2;
    static byte mins;
    static byte secs;

    if ( appConfig.animations )  // 若开启了动画
    {
        if ( timeDate.time.secs != secs )  // 秒钟发生变换
        {
            yPos      = 0;
            yPos_secs = 0;
            moving    = true;

            moving2[ 0 ] = div10( timeDate.time.hour ) != div10( hour2 );  // 时钟进10
            moving2[ 1 ] = mod10( timeDate.time.hour ) != mod10( hour2 );  // 时钟进1
            moving2[ 2 ] = div10( timeDate.time.mins ) != div10( mins );   // 分钟进10
            moving2[ 3 ] = mod10( timeDate.time.mins ) != mod10( mins );   // 分钟进1
            moving2[ 4 ] = div10( timeDate.time.secs ) != div10( secs );   // 秒钟进10

            // memcpy(&timeDateLast, &timeDate, sizeof(timeDate_s));
            // 记录上一时基时刻
            hour2 = timeDate.time.hour;
            mins  = timeDate.time.mins;
            secs  = timeDate.time.secs;
        }

        if ( moving )  // 仍处于动画切换过程
        {
            if ( yPos <= 3 )
                yPos++;
            else if ( yPos <= 6 )
                yPos += 3;
            else if ( yPos <= 16 )
                yPos += 5;
            else if ( yPos <= 22 )
                yPos += 3;
            else if ( yPos <= 24 + TICKER_GAP )
                yPos++;

            // 时、分数字移动到位
            if ( yPos >= MIDFONT_HEIGHT + TICKER_GAP )
                yPos = 255;

            if ( yPos_secs <= 1 )
                yPos_secs++;
            else if ( yPos_secs <= 13 )
                yPos_secs += 3;
            else if ( yPos_secs <= 16 + TICKER_GAP )
                yPos_secs++;

            // 时、分、秒数字 是否 都移动到位，若否则重复上述移动规律
            if ( yPos_secs > FONT_SMALL2_HEIGHT + TICKER_GAP && yPos > MIDFONT_HEIGHT + TICKER_GAP ) {
                yPos      = 0;
                yPos_secs = 0;
                moving    = false;
                memset( moving2, 0x00, sizeof( moving2 ) );  // 清空时分秒变化记录
            }
        }
    } else
#endif
    {
        yPos      = 0;
        yPos_secs = 0;
        moving    = false;                           // 无移动过程
        memset( moving2, 0x00, sizeof( moving2 ) );  // 清除移动步骤
    }

    // 绘制过程
    tickerData_t data;
    // 秒钟 显示字体、大小、位置
    data.x       = 104;  //  秒钟 显示位置 横坐标
    data.y       = 28;   //  秒钟 显示位置 纵坐标
    data.w       = FONT_SMALL2_WIDTH;
    data.h       = FONT_SMALL2_HEIGHT;
    data.bitmap  = ( const byte* )&small2Font;
    data.offsetY = yPos_secs;

    // 秒钟 十位
    data.val    = div10( timeDate.time.secs );  // 秒钟十位值
    data.maxVal = 5;
    data.moving = moving2[ 4 ];
    drawTickerNum( &data );

    // 秒钟 个位
    data.x      = 116;
    data.val    = mod10( timeDate.time.secs );  // 秒钟个位值
    data.maxVal = 9;
    data.moving = moving;
    drawTickerNum( &data );

    // 分钟、时钟 显示字体、大小、位置
    data.y       = TIME_POS_Y;
    data.w       = MIDFONT_WIDTH;
    data.h       = MIDFONT_HEIGHT;
    data.bitmap  = ( const byte* )&midFont;
    data.offsetY = yPos;

    // 分钟 十位
    data.x      = 60;
    data.val    = div10( timeDate.time.mins );
    data.maxVal = 5;
    data.moving = moving2[ 2 ];
    drawTickerNum( &data );

    // 分钟 个位
    data.x      = 83;
    data.val    = mod10( timeDate.time.mins );
    data.maxVal = 9;
    data.moving = moving2[ 3 ];
    drawTickerNum( &data );

    // 时钟 十位
    data.x      = 1;
    data.val    = div10( timeDate.time.hour );
    data.maxVal = 5;
    data.moving = moving2[ 0 ];
    drawTickerNum( &data );

    // 时钟 个位
    data.x      = 24;
    data.val    = mod10( timeDate.time.hour );
    data.maxVal = 9;
    data.moving = moving2[ 1 ];
    drawTickerNum( &data );

    if ( milliseconds % 1000 > 500 )  // 绘制闪烁的时间冒号
        draw_bitmap( TIME_POS_X + 46 + 2, TIME_POS_Y, colon, FONT_COLON_WIDTH, FONT_COLON_HEIGHT, NOINVERT, 0 );

    // Draw AM/PM character
    char tmp[ 2 ];
    tmp[ 0 ] = timeDate.time.ampm;
    tmp[ 1 ] = 0x00;
    draw_string( tmp, false, 104, 20 );

    return ( moving ? DISPLAY_BUSY : DISPLAY_DONE );
}

static void drawTickerNum( tickerData_t* data ) {
    byte        arraySize = ( data->w * data->h ) / 8;  // 每个数字显示所需字节数大小
    byte        yPos      = data->offsetY;
    const byte* bitmap    = &data->bitmap[ data->val * arraySize ];
    byte        x         = data->x;
    byte        y         = data->y;

    if ( !data->moving || yPos == 0 || yPos == 255 )
        draw_bitmap( x, y, bitmap, data->w, data->h, NOINVERT, 0 );
    else {
        byte prev = data->val - 1;
        if ( prev == 255 )  // 前一数字为0
            prev = data->maxVal;

        // 分两部分显示，上一数字与本数字混合
        draw_bitmap( x, y, bitmap, data->w, data->h, NOINVERT, yPos - data->h - TICKER_GAP );
        draw_bitmap( x, y, &data->bitmap[ prev * arraySize ], data->w, data->h, NOINVERT, yPos );
    }
}
/*
static void drawTickerNum(byte x, byte y, byte val, byte maxValue, bool moving, const byte* font, byte w, byte h, byte yPos)
{
    byte arraySize = (w * h) / 8;
    if(yPos == 255)
        yPos = 0;

    s_image img = newImage(x, y, &font[val * arraySize], w, h, WHITE, false, 0);
    draw_bitmap_set(&img);

    if(!moving || yPos == 0)
    {
        draw_bitmap_s2(&img);
        return;
    }

    byte prev = val - 1;
    if(prev == 255)
        prev = maxValue;

    img.offsetY = yPos - h - TICKER_GAP;
    draw_bitmap_s2(&img);

    img.offsetY = yPos;
    img.bitmap = &font[prev * arraySize];
    draw_bitmap_s2(&img);
}*/
