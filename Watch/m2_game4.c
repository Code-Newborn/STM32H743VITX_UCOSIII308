#include "common.h"

#if COMPILE_GAME4

static uint highscore;
static uint score;
static byte uptMove;
static byte lives;

static bool      btnExit( void );
static bool      btnJump( void );
static bool      btnPause( void );
static display_t draw( void );

// 厚度为8pix的地面
static const byte GROUND[] = {
    0xc8, 0xc8, 0xc8, 0x28, 0x28, 0x28, 0x8,  0xc8, 0xc8, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x48, 0x8,  0x8,  0xc8, 0xc8, 0xc8, 0x8,  0x38, 0x38, 0x8,  0x8,
    0x8,  0x8,  0x8,  0x48, 0x48, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x8,  0x48, 0x48, 0x48, 0x8,  0x8,  0x8,  0x28, 0x28, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0xc8, 0x8,  0x8,
    0x8,  0x8,  0x8,  0x8,  0x8,  0x48, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x38, 0x38, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x48, 0x8,  0x28, 0x28, 0x8,  0x48, 0x48,
    0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x8,  0x8,  0x8,  0x8,  0x38, 0x38, 0x8,  0x68, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x8,  0x8,  0x8,  0x8,
    0x8,  0x8,  0x8,  0x48, 0x8,  0x48, 0x48, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x48, 0x48, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x68, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,
    0x8,  0x28, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0xc8, 0xc8, 0x8,  0x9,  0x8,  0x8,  0x8,  0x8,  0x8,  0x38, 0x38, 0x8,  0x8,  0x28, 0x8,  0x9,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28,
    0x28, 0x8,  0x8,  0x8,  0x9,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x68, 0x68, 0x8,  0xc9, 0xc8, 0xc8, 0x8,  0x8,  0x8,  0xc8, 0x8,  0x8,  0x8,  0x8,  0x28, 0x29, 0x8,  0x8,  0x8,
    0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x48, 0x49, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x68, 0x68, 0x8,  0x8,  0x28, 0x28, 0x8,  0x8,  0x8,  0x8,  0x8,
    0x8,  0x8,  0x68, 0x68, 0x8,  0x8,  0xc8, 0xc8, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x28, 0x8,  0x28, 0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x28, 0x28, 0x8,  0x8,  0x48, 0x8,
    0x38, 0x38, 0x8,  0x8,  0x8,  0x8,  0xc8, 0x8,  0x8,  0x8,  0x8,  0x8,  0xc8, 0xc8, 0xc8, 0x28, 0x28, 0x28, 0x8,  0xc8, 0xc8, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,
    0x48, 0x48, 0x8,  0x8,  0xc8, 0xc8, 0xc8, 0x8,  0x38, 0x38, 0x8,  0x8,  0x8,  0x8,  0x8,  0x48, 0x48, 0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x28, 0x8,  0x48, 0x48, 0x48, 0x8,  0x8,  0x8,  0x28,
    0x28, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0xc8, 0x8,  0x8,  0x8,  0xc,  0x6,  0x2,  0x42, 0x42, 0x6,  0xc,  0x8,  0x8,  0x8,  0x8,  0x38, 0x38, 0x8,  0x18, 0x18, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x18, 0x18, 0x8,  0x48, 0x8,  0x28, 0x28, 0x8,  0x48, 0x48, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x28, 0x8,  0x8,  0x8,  0x8,  0x38, 0x38, 0x68,
    0x68, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x48, 0x8,  0x48, 0x48, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x48, 0x48, 0x8,  0x8,  0x8,  0x8,  0x8,
    0x8,  0x68, 0x68, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x28, 0x8,  0x8,  0x8,  0x8,  0x8,  0xc8, 0xc8, 0xc8, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x38, 0x38, 0x8,
    0x8,  0x28, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x28, 0x28, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x68, 0x68, 0x8,  0xc8, 0xc8, 0xc8, 0x8,
    0x8,  0x8,  0xc8, 0x8,  0x8,  0x8,  0x8,  0x28, 0x28, 0x8,  0x8,  0x8,  0x8,  0xc,  0x4,  0x6,  0x2,  0x2,  0x6,  0xc,  0x4c, 0x48, 0x8,  0x8,  0x8,  0xc,  0x4,  0x6,  0x2,  0x2,  0x6,  0xc,
    0xc,  0x68, 0x68, 0x8,  0x28, 0x28, 0x28, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x68, 0x68, 0x68, 0x8,  0x8,  0xc8, 0xc8, 0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x28, 0x28, 0x28,
    0x8,  0x8,  0x8,  0x8,  0x8,  0x28, 0x28, 0x28, 0x8,  0x8,  0x48, 0x28, 0x38, 0x38, 0x8,  0x8,  0x8,  0x8,  0xc8, 0x8,  0x8,
};

// 厚度为8pix的云朵
static const byte CLOUD[] = {
    0x80, 0xC0, 0xE0, 0xF0, 0xB0, 0xB0, 0xB0, 0x98, 0x88, 0x8E, 0x83, 0x83, 0x83, 0x81, 0x81, 0x83, 0x8E, 0x8C, 0x88, 0x88, 0x98, 0xB0, 0xF0, 0xC0,
};

// 跳跃恐龙和落地恐龙 宽 16 高 32
static const byte DINO[] = {
    0xE0, 0x80, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xFE, 0xFF, 0xFD, 0xBF, 0xAF, 0x2F, 0x2F, 0x0E,  //
    0x03, 0x07, 0x0F, 0x1E, 0xFF, 0xBF, 0x1F, 0x1F, 0x3F, 0x2F, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00,  //
    0xE0, 0x80, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xFE, 0xFF, 0xFD, 0xBF, 0xAF, 0x2F, 0x2F, 0x0E,  //
    0x03, 0x07, 0x0F, 0x1E, 0x3F, 0x7F, 0x5F, 0x3F, 0xFF, 0x8F, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00,  //
};

// 宽16 高8 pix
const byte CACTUS_1[] = {
    0xF0, 0xF0, 0x00, 0xFF, 0xFF, 0x00, 0xF0, 0xF0, 0x03, 0x07, 0x86, 0xFF, 0xFF, 0x06, 0x03, 0x01,
};


// 宽16 高16 pix
const byte CACTUS_2[] = {
    0xF0, 0xE0, 0x00, 0xFF, 0xFE, 0x80, 0xFC, 0x00, 0x00, 0x7C, 0xC0, 0xFE, 0xFF, 0x00, 0x80, 0xFC,  //
    0x03, 0x07, 0x06, 0xFF, 0xFF, 0x01, 0x10, 0x90, 0x90, 0x90, 0x00, 0xFF, 0xFF, 0x03, 0x03, 0x01,  //
};

// 宽24 高16 pix
const byte CACTUS_3[] = {
    0xF0, 0xE0, 0x00, 0xFF, 0xFE, 0x80, 0xFC, 0x00, 0xFC, 0xFE, 0x00, 0xFF, 0xFF, 0x00, 0xF8, 0xF0, 0x00, 0xFE, 0x80, 0xFE, 0xFF, 0xFE, 0x78, 0xFC,  //
    0x13, 0x17, 0x06, 0xFF, 0xFF, 0x01, 0x10, 0x10, 0x13, 0x37, 0x04, 0xFF, 0xFF, 0x08, 0x0F, 0x17, 0x10, 0x10, 0x01, 0xFF, 0xFF, 0x03, 0x13, 0x11,  //
};

// 宽24 高16 pix
const byte CACTUS_4[] = {
    0xF0, 0xE0, 0x00, 0xFF, 0xFE, 0x00, 0xF0, 0x00, 0xC0, 0x00, 0xFF, 0xFE, 0x60, 0x3C, 0x80, 0x00, 0x00, 0x7C, 0xC0, 0xFE, 0xFF, 0x00, 0xF0, 0xF8,  //
    0x43, 0x47, 0x86, 0xFF, 0xFF, 0x26, 0xA3, 0xA0, 0x27, 0x04, 0xFF, 0xFF, 0x00, 0x08, 0xFF, 0x88, 0x2F, 0x00, 0x00, 0xFF, 0xFF, 0x06, 0x23, 0x21,  //
};


/*

//===========功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7===========
void OLED_DrawBMP( unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[] ) {
    unsigned int  j = 0;
    unsigned char x, y;

    if ( y1 % 8 == 0 )
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for ( y = y0; y < y1; y++ ) {
        OLED_SetPos( x0, y );
        for ( x = x0; x < x1; x++ ) {
            OLED_WR_Byte( BMP[ j++ ], OLED_DATA );
        }
    }
}


// 快速绘制图像
void OLED_DrawBMPFast( const unsigned char BMP[] ) {
    unsigned int  j = 0;
    unsigned char x, y;

    for ( y = 0; y < 8; y++ ) {
        OLED_SetPos( 0, y );
        IIC_Start();
        IIC_WriteByte( 0x78 );
        IIC_WaitAck();
        IIC_WriteByte( 0x40 );
        IIC_WaitAck();
        for ( x = 0; x < 128; x++ ) {
            IIC_WriteByte( BMP[ j++ ] );
            IIC_WaitAck();
        }
        IIC_Stop();
    }
}

void oled_drawbmp_block_clear( int bx, int by, int clear_size ) {
    unsigned int i;
    OLED_SetPos( bx, by );
    IIC_Start();
    IIC_WriteByte( 0x78 );
    IIC_WaitAck();
    IIC_WriteByte( 0x40 );
    IIC_WaitAck();

    for ( i = 0; i < clear_size; i++ ) {
        if ( bx + i > 128 )
            break;
        IIC_WriteByte( 0x0 );
        IIC_WaitAck();
    }
    IIC_Stop();
}

void OLED_DrawGround() {
    static unsigned int pos           = 0;
    unsigned char       speed         = 5;
    unsigned int        ground_length = sizeof( GROUND );
    unsigned char       x;

    OLED_SetPos( 0, 7 );
    IIC_Start();
    IIC_WriteByte( 0x78 );
    IIC_WaitAck();
    IIC_WriteByte( 0x40 );
    IIC_WaitAck();
    for ( x = 0; x < 128; x++ ) {
        IIC_WriteByte( GROUND[ ( x + pos ) % ground_length ] );
        IIC_WaitAck();
    }
    IIC_Stop();

    pos = pos + speed;
    // if(pos>ground_length) pos=0;
}


// 绘制云朵
void OLED_DrawCloud() {
    static int    pos    = 128;
    static char   height = 0;
    char          speed  = 3;
    unsigned int  i      = 0;
    int           x;
    int           start_x = 0;
    int           length  = sizeof( CLOUD );
    unsigned char byte;

    // if (pos + length <= -speed) pos = 128;

    if ( pos + length <= -speed ) {
        pos    = 128;
        height = rand() % 3;
    }
    if ( pos < 0 ) {
        start_x = -pos;
        OLED_SetPos( 0, 1 + height );
    }
    else {
        OLED_SetPos( pos, 1 + height );
    }

    IIC_Start();
    IIC_WriteByte( 0x78 );
    IIC_WaitAck();
    IIC_WriteByte( 0x40 );
    IIC_WaitAck();
    for ( x = start_x; x < length + speed; x++ ) {
        if ( pos + x > 127 )
            break;
        if ( x < length )
            byte = CLOUD[ x ];
        else
            byte = 0x0;

        IIC_WriteByte( byte );
        IIC_WaitAck();
    }
    IIC_Stop();

    pos = pos - speed;
}

// 绘制小恐龙
void OLED_DrawDino() {
    static unsigned char dino_dir = 0;
    unsigned int         j        = 0;
    unsigned char        x, y;
    unsigned char        byte;

    dino_dir++;
    dino_dir = dino_dir % 2;
    for ( y = 0; y < 2; y++ ) {
        OLED_SetPos( 16, 6 + y );
        IIC_Start();
        IIC_WriteByte( 0x78 );
        IIC_WaitAck();
        IIC_WriteByte( 0x40 );
        IIC_WaitAck();
        for ( x = 0; x < 16; x++ ) {
            j    = y * 16 + x;
            byte = DINO[ dino_dir ][ j ];

            IIC_WriteByte( byte );
            IIC_WaitAck();
        }
        IIC_Stop();
    }
}

// 绘制仙人掌障碍物
void OLED_DrawCactus() {
    char       speed   = 5;
    static int pos     = 128;
    int        start_x = 0;
    int        length  = sizeof( CACTUS_2 ) / 2;

    unsigned int  j = 0;
    unsigned char x, y;
    unsigned char byte;

    if ( pos + length <= 0 ) {
        oled_drawbmp_block_clear( 0, 6, speed );
        pos = 128;
    }

    for ( y = 0; y < 2; y++ ) {
        if ( pos < 0 ) {
            start_x = -pos;
            OLED_SetPos( 0, 6 + y );
        }
        else {
            OLED_SetPos( pos, 6 + y );
        }

        IIC_Start();
        IIC_WriteByte( 0x78 );
        IIC_WaitAck();
        IIC_WriteByte( 0x40 );
        IIC_WaitAck();

        for ( x = start_x; x < length; x++ ) {
            if ( pos + x > 127 )
                break;
            j    = y * length + x;
            byte = CACTUS_2[ j ];
            IIC_WriteByte( byte );
            IIC_WaitAck();
        }
        IIC_Stop();
    }
    oled_drawbmp_block_clear( pos + length, 6, speed );  // 清除残影
    pos = pos - speed;
}


// 绘制随机出现的仙人掌障碍物
int OLED_DrawCactusRandom( unsigned char ver, unsigned char reset ) {
    char       speed   = 5;
    static int pos     = 128;
    int        start_x = 0;
    int        length  = 0;

    unsigned int  i = 0, j = 0;
    unsigned char x, y;
    unsigned char byte;
    if ( reset == 1 ) {
        pos = 128;
        oled_drawbmp_block_clear( 0, 6, speed );
        return 128;
    }
    if ( ver == 0 )
        length = 8;  // sizeof(CACTUS_1) / 2;
    else if ( ver == 1 )
        length = 16;  // sizeof(CACTUS_2) / 2;
    else if ( ver == 2 || ver == 3 )
        length = 24;

    for ( y = 0; y < 2; y++ ) {
        if ( pos < 0 ) {
            start_x = -pos;
            OLED_SetPos( 0, 6 + y );
        }
        else {
            OLED_SetPos( pos, 6 + y );
        }

        IIC_Start();
        IIC_WriteByte( 0x78 );
        IIC_WaitAck();
        IIC_WriteByte( 0x40 );
        IIC_WaitAck();

        for ( x = start_x; x < length; x++ ) {
            if ( pos + x > 127 )
                break;

            j = y * length + x;
            if ( ver == 0 )
                byte = CACTUS_1[ j ];
            else if ( ver == 1 )
                byte = CACTUS_2[ j ];
            else if ( ver == 2 )
                byte = CACTUS_3[ j ];
            else
                byte = CACTUS_4[ j ];

            IIC_WriteByte( byte );
            IIC_WaitAck();
        }
        IIC_Stop();
    }

    oled_drawbmp_block_clear( pos + length, 6, speed );

    pos = pos - speed;
    return pos + speed;
}


// 绘制跳跃小恐龙
int OLED_DrawDinoJump( char reset ) {
    char        speed_arr[] = { 1, 1, 3, 3, 4, 4, 5, 6, 7 };
    static char speed_idx   = sizeof( speed_arr ) - 1;
    static int  height      = 0;
    static char dir         = 0;
    // char speed = 4;

    unsigned int  j = 0;
    unsigned char x, y;
    char          offset = 0;
    unsigned char byte;
    if ( reset == 1 ) {
        height    = 0;
        dir       = 0;
        speed_idx = sizeof( speed_arr ) - 1;
        return 0;
    }
    if ( dir == 0 ) {
        height += speed_arr[ speed_idx ];
        speed_idx--;
        if ( speed_idx < 0 )
            speed_idx = 0;
    }
    if ( dir == 1 ) {
        height -= speed_arr[ speed_idx ];
        speed_idx++;
        if ( speed_idx > sizeof( speed_arr ) - 1 )
            speed_idx = sizeof( speed_arr ) - 1;
    }
    if ( height >= 31 ) {
        dir    = 1;
        height = 31;
    }
    if ( height <= 0 ) {
        dir    = 0;
        height = 0;
    }
    if ( height <= 7 )
        offset = 0;
    else if ( height <= 15 )
        offset = 1;
    else if ( height <= 23 )
        offset = 2;
    else if ( height <= 31 )
        offset = 3;
    else
        offset = 4;

    for ( y = 0; y < 3; y++ )  // 4
    {
        OLED_SetPos( 16, 5 - offset + y );

        IIC_Start();
        IIC_WriteByte( 0x78 );
        IIC_WaitAck();
        IIC_WriteByte( 0x40 );
        IIC_WaitAck();
        for ( x = 0; x < 16; x++ )  // 32
        {
            j    = y * 16 + x;  // 32
            byte = DINO_JUMP[ height % 8 ][ j ];

            IIC_WriteByte( byte );
            IIC_WaitAck();
        }
        IIC_Stop();
    }
    if ( dir == 0 )
        oled_drawbmp_block_clear( 16, 8 - offset, 16 );
    if ( dir == 1 )
        oled_drawbmp_block_clear( 16, 4 - offset, 16 );
    return height;
}

// 绘制重启
void OLED_DrawRestart() {
    unsigned int  j = 0;
    unsigned char x, y;
    unsigned char byte;
    // OLED_SetPos(0, 0);
    for ( y = 2; y < 5; y++ ) {
        OLED_SetPos( 52, y );
        IIC_Start();
        IIC_WriteByte( 0x78 );
        IIC_WaitAck();
        IIC_WriteByte( 0x40 );
        IIC_WaitAck();
        for ( x = 0; x < 24; x++ ) {
            byte = RESTART[ j++ ];
            IIC_WriteByte( byte );
            IIC_WaitAck();
        }
        IIC_Stop();
    }
    OLED_ShowString( 10, 3, "GAME", 16 );
    OLED_ShowString( 86, 3, "OVER", 16 );
}
// 绘制封面
void OLED_DrawCover() {
    OLED_DrawBMPFast( COVER );
}

*/

#define UPT_MOVE_NONE 0
#define UPT_MOVE_UP   1
#define UPT_MOVE_DOWN 2

// 退出
static bool btnExit() {

    animation_start( back, ANIM_MOVE_OFF );  // 返回上一界面
    return true;
}

// 跳跃
static bool btnJump() {
    jump = true;
    return true;
}

// 暂停
static bool btnPause() {
    // gameState = !gameState;
    return true;
}

void game4_start( void ) {
    srand( millis() );  // 随机种子

    display_setDrawFunc( draw );                     // 设置绘制函数
    buttons_setFuncs( btnPause, btnJump, btnExit );  // 设置按键功能
}

unsigned char key_num         = 0;
unsigned char cactus_category = 0;
unsigned char cactus_length   = 8;
unsigned int  score           = 0;
unsigned int  highest_score   = 0;
int           height          = 0;
int           cactus_pos      = 128;
unsigned char cur_speed       = 30;
char          failed          = 0;
char          reset           = 0;

static unsigned int ground_pos = 0;

// Storage Vars
static uint highscore;
static uint score;
static bool jump;
static bool newHighscore;
static bool overlap;
static bool gameState = false;


static display_t draw() {

    static double y;
    static double yVel;
    static byte   tubeOffset;

    uint8_t ground_speed = 3;

    char buff[ 6 ];

    if ( gameState ) {        // 游戏运行状态
        if ( height <= 0 ) {  // 触底后才响应跳跃
            if ( jump ) {     // 跳跃
                yVel = -1.2;
                jump = false;
            }
        }

        else if ( ( byte )y > 200 ) {  // 触顶
            yVel = 0;
            y    = 0;
        }
        else if ( ( byte )y < FRAME_HEIGHT - 8 ) {  // 自然降落
            if ( yVel < 0 )
                yVel += 0.1;
            else
                yVel += 0.06;
        }
        else {  // 触底
            yVel = 0;
            y    = FRAME_HEIGHT - 8;
        }

        y += yVel;  // 降落值

        score++;  // 加分
    }

    // 循环裁剪地面贴图
    byte ground_slice[ 128 ];
    for ( size_t i = 0; i < 128; i++ ) {
        ground_slice[ i ] = GROUND[ ( ground_pos + i ) % sizeof( GROUND ) ];
    }
    draw_bitmap( 0, 0, ground_slice, FRAME_WIDTH, FRAME_HEIGHT, NOINVERT, 0 );
    ground_pos += ground_speed;
    ground_pos = ( ground_pos % sizeof( GROUND ) );



    // 绘制分数
    sprintf_P( buff, PSTR( "%u" ), score / 80 );
    draw_string( buff, false, FRAME_WIDTH - ( 7 * 5 ), 1 );

    // 绘制血量
    LOOP( lives, i )
    draw_bitmap( 32 + ( 8 * i ), 1, livesImg, 7, 8, NOINVERT, 0 );

    // draw_bitmap( 0, 0, dinoCover, FRAME_WIDTH, FRAME_HEIGHT, NOINVERT, 0 );  // 绘制DinoRun游戏封面
}


#endif