#include "GT911.h"
#include "stdlib.h"
#include "sys.h"
//==================================================================
// Name:						GT911_int_sync
// Author:					miracle
// Date:						2017-07-06
// Function:				Sync GT911 INT signal
//								Sync is the most important part in initial stage
// Input:					unsigned int
// Output:					void
// option:
//==================================================================
void GT911_int_sync( u16 ms ) {
    INT_CTRL = 0;
    delay_ms( ms );
    INT_IN();
}

//==================================================================
// Name:						GT911_reset_guitar
// Author:
// Date:						2017-07-06
// Function:				Rest GT911, and work with function GT911_int_sync
//								sync sequence depends on slave address
// Input:					unsigned char addr
// Output:					void
// option:
//==================================================================
void GT911_reset_guitar( u8 addr ) {
    INT_CTRL = 1;
    RST_CTRL = 1;
    delay_ms( 20 );
    RST_CTRL = 0;
    INT_CTRL = 0;
    delay_ms( 20 );
    INT_CTRL = ( addr == 0x28 );
    delay_ms( 20 );
    RST_CTRL = 1;
    delay_ms( 20 );
}

//==================================================================
// Name:						GT911_reset
// Author:
// Date:						2017-07-06
// Function:				Rest GT911, just reset without sync
// Input:					void
// Output:					void
// option:
//==================================================================
void GT911_reset( void ) {
    RST_CTRL = 0;
    delay_ms( 10 );
    RST_CTRL = 1;
    delay_ms( 10 );
}

//==================================================================
// Name:						GT911_gpio_init
// Author:
// Date:						2017-07-06
// Function:				Init GT911 related GPIO: IIC,RST,INT
// Input:					void
// Output:					void
// option:
//==================================================================
void GT911_gpio_init( void ) {
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOH_CLK_ENABLE();  // 开启GPIOH时钟
    __HAL_RCC_GPIOI_CLK_ENABLE();  // 开启GPIOI时钟

    // GPIO_Set(GPIOH,PIN7,GPIO_MODE_IN,0,0,GPIO_PUPD_PU);	//PH7设置为上拉输入
    //	GPIO_Set(GPIOI,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//PI8设置为推挽输出

    // PH7
    GPIO_Initure.Pin   = GPIO_PIN_7;        // PH7
    GPIO_Initure.Mode  = GPIO_MODE_INPUT;   // 输入
    GPIO_Initure.Pull  = GPIO_PULLUP;       // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   // 高速
    HAL_GPIO_Init( GPIOH, &GPIO_Initure );  // 初始化

    // PI8
    GPIO_Initure.Pin  = GPIO_PIN_8;           // PI8
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
    HAL_GPIO_Init( GPIOI, &GPIO_Initure );    // 初始化

    CT_IIC_Init();
    RST_OUT();
    INT_OUT();
    GT911_reset();
}

//==================================================================
// Name:						GT9XX_WriteHandle
// Author:
// Date:						2017-07-06
// Function:				Handle GT911 write function
// Input:					unsigned int addr
// Output:					void
// option:
//==================================================================
u8 GT9XX_WriteHandle( u16 addr ) {
    u8 status;

    CT_IIC_Start();
    CT_IIC_Send_Byte( GT9XX_IIC_WADDR );  // 写数据指令
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte( ( u8 )( addr >> 8 ) );  // 写入16位地址
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte( ( u8 )addr );
    CT_IIC_Wait_Ack();
    status = SUCCESS;
    return status;
}

//==================================================================
// Name:						GT9XX_WriteData
// Author:
// Date:						2017-07-06
// Function:				Write data to GT911
// Input:					unsigned int addr				//address of register
//								unsigned char value			//value should be writed
// Output:					void
// option:
//==================================================================
u8 GT9XX_WriteData( u16 addr, u8 value ) {
    u8 status;
    CT_IIC_Start();

    GT9XX_WriteHandle( addr );
    CT_IIC_Send_Byte( value );
    CT_IIC_Wait_Ack();
    CT_IIC_Stop();
    status = SUCCESS;
    return status;
}

//==================================================================
// Name:						GT9XX_ReadData
// Author:
// Date:						2017-07-06
// Function:				Read data from GT911
// Input:					unsigned int addr				//address of register
//								unsigned char *value		//pointer of data output
// Output:					void
// option:
//==================================================================
u8 GT9XX_ReadData( u16 addr, u8 cnt, u8* value ) {
    u8 status;
    u8 i;

    status = ERROR;
    CT_IIC_Start();
    GT9XX_WriteHandle( addr );
    CT_IIC_Start();
    delay_us( 20 );
    CT_IIC_Send_Byte( GT9XX_IIC_RADDR );
    CT_IIC_Wait_Ack();
    for ( i = 0; i < cnt; i++ ) {
        if ( i == ( cnt - 1 ) ) {
            value[ i ] = CT_IIC_Read_Byte( 0 );
        } else {
            value[ i ] = CT_IIC_Read_Byte( 1 );
        }
    }
    CT_IIC_Stop();
    status = SUCCESS;
    return ( status );
}

//==================================================================
// Name:						Touch_Init
// Author:
// Date:						2017-07-06
// Function:				GT911 init, including GPIO init, sync, and version check
// Input:					unsigned int addr				//address of register
//								unsigned char *value		//pointer of data output
// Output:					void
// option:
//==================================================================
u8 GT911_Init( void ) {
    u8 touchIC_ID[ 4 ];
    GT911_gpio_init();
    //	GT9xx_Eint_Init();
    GT911_reset_guitar( GT9XX_IIC_WADDR );
    GT911_int_sync( 50 );
    GT9XX_ReadData( GT9XX_ID_ADDR, 4, touchIC_ID );
    if ( touchIC_ID[ 0 ] == '9' ) {
        // printf("Touch ID: %s \r\n",touchIC_ID);
        // GT9xx_send_config();
        return 1;
    } else {
        // printf("Touch Error\r\n");
        return 0;
    }
}

u8 Touch_Get_Count( void ) {
    u8 count[ 1 ] = { 0 };
    GT9XX_ReadData( GT9XX_READ_ADDR, 1, count );  // read touch data
    return ( count[ 0 ] & 0x0f );
}

const u16 TPX[] = { 0x8150, 0x8158, 0x8160, 0x8168, 0x8170 };  // 电容屏触摸点数据地址（1~5）

//==================================================================
// Name:						Touch_Get_Data
// Author:
// Date:						2017-07-06
// Function:				Get GT911 data, such as point and coordinate
// Input:					void
// Output:					void
// option:
//==================================================================
u8 GT911_Scan( u8 mode ) {
    u8        buf[ 4 ];
    u8        i   = 0;
    u8        res = 0;
    u8        temp;
    u8        tempsta;
    static u8 t = 0;  // 控制查询间隔,从而降低CPU占用率
    t++;
    if ( ( t % 10 ) == 0 || t < 10 )  // 空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
    {
        GT9XX_ReadData( GT9XX_READ_ADDR, 1, &mode );
        if ( mode & 0X80 && ( ( mode & 0XF ) < 6 ) ) {
            temp = 0;
            GT9XX_WriteData( GT9XX_READ_ADDR, temp );
        }
        if ( ( mode & 0XF ) && ( ( mode & 0XF ) < 6 ) ) {
            temp          = 0XFF << ( mode & 0XF );  // 将点的个数转换为1的位数,匹配tp_dev.sta定义
            tempsta       = tp_dev.sta;              // 保存当前的tp_dev.sta值
            tp_dev.sta    = ( ~temp ) | TP_PRES_DOWN | TP_CATH_PRES;
            tp_dev.x[ 4 ] = tp_dev.x[ 0 ];  // 保存触点0的数据
            tp_dev.y[ 4 ] = tp_dev.y[ 0 ];
            for ( i = 0; i < 5; i++ ) {
                if ( tp_dev.sta & ( 1 << i ) )  // 触摸有效?
                {
                    GT9XX_ReadData( TPX[ i ], 4, buf );  // 读取XY坐标值
                    if ( tp_dev.touchtype & 0X01 )       // 横屏
                    {
                        tp_dev.y[ i ] = ( ( u16 )buf[ 1 ] << 8 ) + buf[ 0 ];
                        tp_dev.x[ i ] = 480 - ( ( ( u16 )buf[ 3 ] << 8 ) + buf[ 2 ] );
                    } else {
                        tp_dev.x[ i ] = ( ( u16 )buf[ 1 ] << 8 ) + buf[ 0 ];
                        tp_dev.y[ i ] = ( ( u16 )buf[ 3 ] << 8 ) + buf[ 2 ];
                    }
                    // printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
                }
            }
            res = 1;
            if ( tp_dev.x[ 0 ] > lcddev.width || tp_dev.y[ 0 ] > lcddev.height )  // 非法数据(坐标超出了)
            {
                if ( ( mode & 0XF ) > 1 )  // 有其他点有数据,则复第二个触点的数据到第一个触点.
                {
                    tp_dev.x[ 0 ] = tp_dev.x[ 1 ];
                    tp_dev.y[ 0 ] = tp_dev.y[ 1 ];
                    t             = 0;  // 触发一次,则会最少连续监测10次,从而提高命中率
                } else                  // 非法数据,则忽略此次数据(还原原来的)
                {
                    tp_dev.x[ 0 ] = tp_dev.x[ 4 ];
                    tp_dev.y[ 0 ] = tp_dev.y[ 4 ];
                    mode          = 0X80;
                    tp_dev.sta    = tempsta;  // 恢复tp_dev.sta
                }
            } else
                t = 0;  // 触发一次,则会最少连续监测10次,从而提高命中率
        }
    }
    if ( ( mode & 0X8F ) == 0X80 )  // 无触摸点按下
    {
        if ( tp_dev.sta & TP_PRES_DOWN )  // 之前是被按下的
        {
            tp_dev.sta &= ~( 1 << 7 );  // 标记按键松开
        } else                          // 之前就没有被按下
        {
            tp_dev.x[ 0 ] = 0xffff;
            tp_dev.y[ 0 ] = 0xffff;
            tp_dev.sta &= 0XE0;  // 清除点有效标记
        }
    }
    if ( t > 240 )
        t = 10;  // 重新从10开始计数
    return res;
}
