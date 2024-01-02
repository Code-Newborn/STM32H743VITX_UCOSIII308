#include "Ink_oled.h"
#include <stdlib.h>
#include "Ink_oledfont.h"
#include "delay.h"
#include "spi.h"

// 模拟SPI时序
void OLED_WR_Bus( uint8_t dat ) {
    // uint8_t i;
    OLED_CS_Clr();
    // for ( i = 0; i < 8; i++ ) {
    //     OLED_SCL_Clr();
    //     if ( dat & 0x80 ) {
    //         OLED_SDA_Set();
    //     } else {
    //         OLED_SDA_Clr();
    //     }
    //     OLED_SCL_Set();
    //     dat <<= 1;
    // }
    HAL_SPI_Transmit( &hspi1, &dat, 1, 100 );
    OLED_CS_Set();
}

// 写入一个命令
void OLED_WR_REG( uint8_t reg ) {
    OLED_DC_Clr();
    OLED_WR_Bus( reg );
    OLED_DC_Set();
}

// 写入一个字节
void OLED_WR_DATA8( uint8_t dat ) {
    OLED_WR_Bus( dat );
}

PAINT Paint;

void Epaper_READBUSY() {
    while ( 1 ) {
        if ( OLED_BUSY() == 0 ) {
            break;
        }
    }
}

void EPD_Update( void ) {
    OLED_WR_REG( 0x22 );  // Display Update Control
    OLED_WR_DATA8( 0xF7 );
    // OLED_WR_DATA8(0xFF);
    OLED_WR_REG( 0x20 );  // Activate Display Update Sequence
    Epaper_READBUSY();
}

// 初始化屏幕
void OLED_GUIInit( void ) {
    // OLED_GPIOInit();
    OLED_RES_Clr();  // Module reset
    delay_ms( 20 );  // At least 10ms delay
    OLED_RES_Set();
    delay_ms( 20 );  // At least 10ms delay

    Epaper_READBUSY();
    OLED_WR_REG( 0x12 );  // SWRESET
    Epaper_READBUSY();

    OLED_WR_REG( 0x01 );  // Driver output control
    OLED_WR_DATA8( 0x27 );
    OLED_WR_DATA8( 0x01 );
    OLED_WR_DATA8( 0x01 );

    OLED_WR_REG( 0x11 );  // data entry mode
    OLED_WR_DATA8( 0x01 );

    OLED_WR_REG( 0x44 );  // set Ram-X address start/end position
    OLED_WR_DATA8( 0x00 );
    OLED_WR_DATA8( 0x0F );  // 0x0F-->(15+1)*8=128

    OLED_WR_REG( 0x45 );    // set Ram-Y address start/end position
    OLED_WR_DATA8( 0x27 );  // 0xF9-->(249+1)=250
    OLED_WR_DATA8( 0x01 );
    OLED_WR_DATA8( 0x00 );
    OLED_WR_DATA8( 0x00 );

    OLED_WR_REG( 0x3C );  // BorderWavefrom
    OLED_WR_DATA8( 0x05 );

    OLED_WR_REG( 0x21 );  //  Display update control
    OLED_WR_DATA8( 0x00 );
    OLED_WR_DATA8( 0x80 );

    OLED_WR_REG( 0x18 );  // Read built-in temperature sensor
    OLED_WR_DATA8( 0x80 );

    OLED_WR_REG( 0x4E );  // set RAM x address count to 0;
    OLED_WR_DATA8( 0x00 );
    OLED_WR_REG( 0x4F );  // set RAM y address count to 0X199;
    OLED_WR_DATA8( 0x27 );
    OLED_WR_DATA8( 0x01 );

    Epaper_READBUSY();
}

void Paint_NewImage( uint8_t* image, uint16_t Width, uint16_t Height, uint16_t Rotate, uint16_t Color ) {
    Paint.Image = 0x00;
    Paint.Image = image;

    Paint.WidthMemory  = Width;
    Paint.HeightMemory = Height;
    Paint.Color        = Color;
    Paint.WidthByte    = ( Width % 8 == 0 ) ? ( Width / 8 ) : ( Width / 8 + 1 );
    Paint.HeightByte   = Height;
    Paint.Rotate       = Rotate;
    if ( Rotate == ROTATE_0 || Rotate == ROTATE_180 ) {
        Paint.Width  = Width;
        Paint.Height = Height;
    } else {
        Paint.Width  = Height;
        Paint.Height = Width;
    }
}

void Paint_SetPixel( uint16_t Xpoint, uint16_t Ypoint, uint16_t Color ) {
    uint16_t X, Y;
    uint32_t Addr;
    uint8_t  Rdata;
    switch ( Paint.Rotate ) {
    case 0:

        X = Paint.WidthMemory - Ypoint - 1;
        Y = Xpoint;
        break;
    case 90:
        X = Paint.WidthMemory - Xpoint - 1;
        Y = Paint.HeightMemory - Ypoint - 1;
        break;
    case 180:
        X = Ypoint;
        Y = Paint.HeightMemory - Xpoint - 1;
        break;
    case 270:
        X = Xpoint;
        Y = Ypoint;
        break;
    default:
        return;
    }
    Addr  = X / 8 + Y * Paint.WidthByte;
    Rdata = Paint.Image[ Addr ];
    if ( Color == BLACK ) {
        Paint.Image[ Addr ] = Rdata & ~( 0x80 >> ( X % 8 ) );  // 将对应数据位置0
    } else
        Paint.Image[ Addr ] = Rdata | ( 0x80 >> ( X % 8 ) );  // 将对应数据位置1
}

// 清屏函数
void OLED_Clear( uint16_t Color ) {
    uint16_t X, Y;
    uint32_t Addr;
    for ( Y = 0; Y < Paint.HeightByte; Y++ ) {
        for ( X = 0; X < Paint.WidthByte; X++ ) {  // 8 pixel =  1 byte
            Addr                = X + Y * Paint.WidthByte;
            Paint.Image[ Addr ] = Color;
        }
    }
}

// 画点函数
void OLED_DrawPoint( uint16_t Xpoint, uint16_t Ypoint, uint16_t Color ) {
    Paint_SetPixel( Xpoint - 1, Ypoint - 1, Color );
}

// 更新到显存
void OLED_Display( unsigned char* Image ) {
    unsigned int Width, Height, i, j;
    uint32_t     k = 0;
    Width          = 296;
    Height         = 16;
    OLED_WR_REG( 0x24 );
    for ( j = 0; j < Height; j++ ) {
        for ( i = 0; i < Width; i++ ) {
            OLED_WR_DATA8( Image[ k ] );
            k++;
        }
    }
    EPD_Update();
}

// 画直线
void OLED_DrawLine( uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color ) {
    uint16_t Xpoint, Ypoint;
    int      dx, dy;
    int      XAddway, YAddway;
    int      Esp;
    char     Dotted_Len;
    Xpoint = Xstart;
    Ypoint = Ystart;
    dx     = ( int )Xend - ( int )Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    dy     = ( int )Yend - ( int )Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    XAddway = Xstart < Xend ? 1 : -1;
    YAddway = Ystart < Yend ? 1 : -1;

    Esp        = dx + dy;
    Dotted_Len = 0;

    for ( ;; ) {
        Dotted_Len++;
        OLED_DrawPoint( Xpoint, Ypoint, Color );
        if ( 2 * Esp >= dy ) {
            if ( Xpoint == Xend )
                break;
            Esp += dy;
            Xpoint += XAddway;
        }
        if ( 2 * Esp <= dx ) {
            if ( Ypoint == Yend )
                break;
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

// 画矩形
void OLED_DrawRectangle( uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t Color, uint8_t mode ) {
    uint16_t i;
    if ( mode ) {
        for ( i = Ystart; i < Yend; i++ ) {
            OLED_DrawLine( Xstart, i, Xend, i, Color );
        }
    } else {
        OLED_DrawLine( Xstart, Ystart, Xend, Ystart, Color );
        OLED_DrawLine( Xstart, Ystart, Xstart, Yend, Color );
        OLED_DrawLine( Xend, Yend, Xend, Ystart, Color );
        OLED_DrawLine( Xend, Yend, Xstart, Yend, Color );
    }
}

// 画圆形
void OLED_DrawCircle( uint16_t X_Center, uint16_t Y_Center, uint16_t Radius, uint16_t Color, uint8_t mode ) {
    uint16_t Esp, sCountY;
    uint16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;
    Esp      = 3 - ( Radius << 1 );
    if ( mode ) {
        while ( XCurrent <= YCurrent ) {  // Realistic circles
            for ( sCountY = XCurrent; sCountY <= YCurrent; sCountY++ ) {
                OLED_DrawPoint( X_Center + XCurrent, Y_Center + sCountY, Color );  // 1
                OLED_DrawPoint( X_Center - XCurrent, Y_Center + sCountY, Color );  // 2
                OLED_DrawPoint( X_Center - sCountY, Y_Center + XCurrent, Color );  // 3
                OLED_DrawPoint( X_Center - sCountY, Y_Center - XCurrent, Color );  // 4
                OLED_DrawPoint( X_Center - XCurrent, Y_Center - sCountY, Color );  // 5
                OLED_DrawPoint( X_Center + XCurrent, Y_Center - sCountY, Color );  // 6
                OLED_DrawPoint( X_Center + sCountY, Y_Center - XCurrent, Color );  // 7
                OLED_DrawPoint( X_Center + sCountY, Y_Center + XCurrent, Color );
            }
            if ( ( int )Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * ( XCurrent - YCurrent );
                YCurrent--;
            }
            XCurrent++;
        }
    } else {  // Draw a hollow circle
        while ( XCurrent <= YCurrent ) {
            OLED_DrawPoint( X_Center + XCurrent, Y_Center + YCurrent, Color );  // 1
            OLED_DrawPoint( X_Center - XCurrent, Y_Center + YCurrent, Color );  // 2
            OLED_DrawPoint( X_Center - YCurrent, Y_Center + XCurrent, Color );  // 3
            OLED_DrawPoint( X_Center - YCurrent, Y_Center - XCurrent, Color );  // 4
            OLED_DrawPoint( X_Center - XCurrent, Y_Center - YCurrent, Color );  // 5
            OLED_DrawPoint( X_Center + XCurrent, Y_Center - YCurrent, Color );  // 6
            OLED_DrawPoint( X_Center + YCurrent, Y_Center - XCurrent, Color );  // 7
            OLED_DrawPoint( X_Center + YCurrent, Y_Center + XCurrent, Color );  // 0
            if ( ( int )Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * ( XCurrent - YCurrent );
                YCurrent--;
            }
            XCurrent++;
        }
    }
}

// 显示字符
void OLED_ShowChar( uint16_t x, uint16_t y, uint16_t chr, uint16_t size1, uint16_t color ) {
    uint16_t i, m, temp, size2, chr1;
    uint16_t x0, y0;
    x += 1, y += 1, x0 = x, y0 = y;
    if ( size1 == 8 )
        size2 = 6;
    else
        size2 = ( size1 / 8 + ( ( size1 % 8 ) ? 1 : 0 ) ) * ( size1 / 2 );  // 得到字体一个字符对应点阵集所占的字节数
    chr1 = chr - ' ';                                                       // 计算偏移后的值
    for ( i = 0; i < size2; i++ ) {
        if ( size1 == 8 ) {
            temp = asc2_0806[ chr1 ][ i ];
        }  // 调用0806字体
        else if ( size1 == 12 ) {
            temp = asc2_1206[ chr1 ][ i ];
        }  // 调用1206字体
        else if ( size1 == 16 ) {
            temp = asc2_1608[ chr1 ][ i ];
        }  // 调用1608字体
        else if ( size1 == 24 ) {
            temp = asc2_2412[ chr1 ][ i ];
        }  // 调用2412字体
        else
            return;
        for ( m = 0; m < 8; m++ ) {
            if ( temp & 0x01 )
                OLED_DrawPoint( x, y, color );
            else
                OLED_DrawPoint( x, y, !color );
            temp >>= 1;
            y++;
        }
        x++;
        if ( ( size1 != 8 ) && ( ( x - x0 ) == size1 / 2 ) ) {
            x  = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

// 显示字符串
// x,y:起点坐标
// size1:字体大小
//*chr:字符串起始地址
// mode:0,反色显示;1,正常显示
void OLED_ShowString( uint16_t x, uint16_t y, uint8_t* chr, uint16_t size1, uint16_t color ) {
    while ( *chr != '\0' )  // 判断是不是非法字符!
    {

        OLED_ShowChar( x, y, *chr, size1, color );
        chr++;
        x += size1 / 2;
    }
}

// m^n
uint32_t OLED_Pow( uint16_t m, uint16_t n ) {
    uint32_t result = 1;
    while ( n-- ) {
        result *= m;
    }
    return result;
}

// 显示数字
// x,y :起点坐标
// num :要显示的数字
// len :数字的位数
// size:字体大小
// mode:0,反色显示;1,正常显示
void OLED_ShowNum( uint16_t x, uint16_t y, uint32_t num, uint16_t len, uint16_t size1, uint16_t color ) {
    uint8_t t, temp, m = 0;
    if ( size1 == 8 )
        m = 2;
    for ( t = 0; t < len; t++ ) {
        temp = ( num / OLED_Pow( 10, len - t - 1 ) ) % 10;
        if ( temp == 0 ) {
            OLED_ShowChar( x + ( size1 / 2 + m ) * t, y, '0', size1, color );
        } else {
            OLED_ShowChar( x + ( size1 / 2 + m ) * t, y, temp + '0', size1, color );
        }
    }
}

// 显示汉字
// x,y:起点坐标
// num:汉字对应的序号
// mode:0,反色显示;1,正常显示
void OLED_ShowChinese( uint16_t x, uint16_t y, uint16_t num, uint16_t size1, uint16_t color ) {
    uint16_t m, temp;
    uint16_t x0, y0;
    uint16_t i, size3 = ( size1 / 8 + ( ( size1 % 8 ) ? 1 : 0 ) ) * size1;  // 得到字体一个字符对应点阵集所占的字节数
    x += 1, y += 1, x0 = x, y0 = y;
    for ( i = 0; i < size3; i++ ) {
        if ( size1 == 16 ) {
            temp = Hzk1[ num ][ i ];
        }  // 调用16*16字体
        else if ( size1 == 24 ) {
            temp = Hzk2[ num ][ i ];
        }  // 调用24*24字体
        else if ( size1 == 32 ) {
            temp = Hzk3[ num ][ i ];
        }  // 调用32*32字体
        else if ( size1 == 64 ) {
            temp = Hzk4[ num ][ i ];
        }  // 调用64*64字体
        else
            return;
        for ( m = 0; m < 8; m++ ) {
            if ( temp & 0x01 )
                OLED_DrawPoint( x, y, color );
            else
                OLED_DrawPoint( x, y, !color );
            temp >>= 1;
            y++;
        }
        x++;
        if ( ( x - x0 ) == size1 ) {
            x  = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}
// 显示图片
//  x,y:起点坐标
//  sizex：图片宽度
//  sizey:图片长度
//  BMP：图片数组
//  mode:图片显示的颜色
void OLED_ShowPicture( uint16_t x, uint16_t y, uint16_t sizex, uint16_t sizey, const uint8_t BMP[], uint16_t Color ) {
    uint16_t j = 0;
    uint16_t i, n, temp, m;
    uint16_t x0, y0;
    x += 1, y += 1, x0 = x, y0 = y;
    sizey = sizey / 8 + ( ( sizey % 8 ) ? 1 : 0 );
    for ( n = 0; n < sizey; n++ ) {
        for ( i = 0; i < sizex; i++ ) {
            temp = BMP[ j ];
            j++;
            for ( m = 0; m < 8; m++ ) {
                if ( temp & 0x01 )
                    OLED_DrawPoint( x, y, !Color );
                else
                    OLED_DrawPoint( x, y, Color );
                temp >>= 1;
                y++;
            }
            x++;
            if ( ( x - x0 ) == sizex ) {
                x  = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}
