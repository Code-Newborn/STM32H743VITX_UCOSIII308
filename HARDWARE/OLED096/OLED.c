#include "OLED096/OLED.h"
#include "OLED096/OLED_FONT.h"

#include "common.h"
#include "typedefs.h"
#include "usart.h"
#include "util.h"




byte oledBuffer[ FRAME_BUFFER_SIZE ];  // 图形显示数据

void loadFlip( void ) {
    if ( !appConfig.display180 ) {
        OLED_FlipNormal();  // 正常配置
    }
    else {
        OLED_FlipEnable();  // 翻转配置
    }
}


// 配置写入数据的起始位置
void OLED_WR_BP( uint8_t x, uint8_t y ) {
    WriteCmd( 0xb0 + y );                      // 设置行起始地址
    WriteCmd( ( ( x & 0xf0 ) >> 4 ) | 0x10 );  // 设置低列起始地址
    WriteCmd( ( x & 0x0f ) | 0x01 );           // 设置高列起始地址
}

// x0,y0：起点坐标
// x1,y1：终点坐标
// BMP[]：要写入的图片数组
void OLED_ShowPicture( uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[] ) {
    uint32_t j = 0;
    uint8_t  x = 0, y = 0;
    if ( y % 8 == 0 )
        y = 0;
    else
        y += 1;
    for ( y = y0; y < y1; y++ ) {
        OLED_WR_BP( x0, y );
        for ( x = x0; x < x1; x++ ) {
            WriteDat( BMP[ j ] );
            j++;
        }
    }
}


// 画线
// x1,y1:起点坐标
// x2,y2:终点坐标
void OLED_DrawLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 ) {
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1;  // 计算坐标增量
    delta_y = y2 - y1;
    uRow    = x1;
    uCol    = y1;
    if ( delta_x > 0 )
        incx = 1;
    else if ( delta_x == 0 )  // 垂直线
        incx = 0;
    else {
        incx    = -1;
        delta_x = -delta_x;
    }

    if ( delta_y > 0 )
        incy = 1;
    else if ( delta_y == 0 )
        incy = 0;  // 水平线
    else {
        incy    = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y )
        distance = delta_x;  // 选取坐标差值最大方向
    else
        distance = delta_y;
    for ( t = 0; t <= distance + 1; t++ )  // 画线输出
    {
        SetPointBuffer( uRow, uCol, GetDrawColor() );  // 画点
        xerr += delta_x;
        yerr += delta_y;
        if ( xerr > distance ) {
            xerr -= distance;
            uRow += incx;
        }
        if ( yerr > distance ) {
            yerr -= distance;
            uCol += incy;
        }
    }
}

static Type_color Draw_Color = pix_white;

void SetDrawColor( Type_color value ) {
    Draw_Color = value;
}

Type_color GetDrawColor( void ) {
    return Draw_Color;
}

// 定义缓冲，
unsigned char ScreenBuffer[ SCREEN_PAGE_NUM ][ SCREEN_COLUMN ] = { 0 };
unsigned char TempBuffer[ SCREEN_PAGE_NUM ][ SCREEN_COLUMN ]   = { 0 };

static _Bool _SelectedBuffer = 1;

void SetPointBuffer( int x, int y, int value ) {
    if ( x > SCREEN_COLUMN - 1 || y > SCREEN_ROW - 1 )  // 超出范围
        return;
    if ( _SelectedBuffer ) {
        if ( value )
            OLED_GRAM[ ( y / SCREEN_PAGE_NUM ) * 64 + x ] |= 1 << ( y % SCREEN_PAGE_NUM );
        else
            OLED_GRAM[ ( y / SCREEN_PAGE_NUM ) * 64 + x ] &= ~( 1 << ( y % SCREEN_PAGE_NUM ) );
    }
    else {
        if ( value )
            TempBuffer[ y / SCREEN_PAGE_NUM ][ x ] |= 1 << ( y % SCREEN_PAGE_NUM );
        else
            TempBuffer[ y / SCREEN_PAGE_NUM ][ x ] &= ~( 1 << ( y % SCREEN_PAGE_NUM ) );
    }
}


// 清屏函数
// color:要清屏的填充色
void OLED_FillAll( Type_color color ) {
    if ( color == pix_black ) {
        memset( ScreenBuffer, 0x00, sizeof( ScreenBuffer ) );
    }
    else {
        memset( ScreenBuffer, 0xFF, sizeof( ScreenBuffer ) );
    }
}

void OLED_Flush( void ) {
    uint8_t* p;
    p = oledBuffer;

    for ( uint8_t i = 0; i < 8; i++ ) {

        WriteCmd( 0xb0 + i );  // page0-page1
        WriteCmd( 0x00 );      // low column start address
        WriteCmd( 0x10 );

        for ( uint8_t j = 0; j < 128; j++ ) {
            if ( appConfig.invert ) {
                WriteDat( ~( *p++ ) );
            }
            else {
                WriteDat( *p++ );
            }
        }
    }
}
