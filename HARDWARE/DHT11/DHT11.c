#include "DHT11.h"
#include "delay.h"
#include "gpio.h"

uint8_t humi         = 0;
uint8_t temp         = 0;
uint8_t temp_decimal = 0;
uint8_t humi_decimal = 0;

/**
 * @brief  设置引脚模式 PB1	---> DHT11_DQ
 * @param  mode: 0->out, 1->in
 * @retval None
 */
static void DHT11_GPIO_MODE_SET( uint8_t mode ) {
    if ( mode ) {
        /*  输入  */
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Pin  = GPIO_PIN_1;       //  11号引脚
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  //  输入模式
        GPIO_InitStruct.Pull = GPIO_PULLUP;      //  上拉输入
        HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
    } else {
        /*  输出  */
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.Pin   = GPIO_PIN_1;            //  11号引脚
        GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;   //  Push Pull 推挽输出模式
        GPIO_InitStructure.Pull  = GPIO_PULLUP;           //  上拉输出
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;  //  高速
        HAL_GPIO_Init( GPIOB, &GPIO_InitStructure );
    }
}

// 复位DHT11
void DHT11_Rst( void ) {
    DHT11_GPIO_MODE_SET( 0 );                                               // 设置为输出模式
    HAL_GPIO_WritePin( DHT11_DQ_GPIO_Port, DHT11_DQ_Pin, GPIO_PIN_RESET );  // 拉低DQ
    HAL_Delay( 20 );                                                        // 拉低至少18ms
    HAL_GPIO_WritePin( DHT11_DQ_GPIO_Port, DHT11_DQ_Pin, GPIO_PIN_SET );    // 拉高DQ
    delay_us( 30 );                                                         // 拉高20~40us
}

// 等待DHT11的回应
// 返回1:未检测到DHT11的存在
// 返回0:存在
uint8_t DHT11_Check( void ) {
    uint8_t retry = 0;
    DHT11_GPIO_MODE_SET( 1 );               // 设置为输入
    while ( DHT11_READ_IO && retry < 100 )  // DHT11会拉低40~80us
    {
        retry++;
        delay_us( 1 );
    };
    if ( retry >= 100 )
        return 1;
    else
        retry = 0;
    while ( !DHT11_READ_IO && retry < 100 )  // DHT11拉低后会再次拉高40~80us
    {
        retry++;
        delay_us( 1 );
    };
    if ( retry >= 100 )
        return 1;
    return 0;
}
// 从DHT11读取一个位
// 返回值：1/0
uint8_t DHT11_Read_Bit( void ) {
    uint8_t retry = 0;
    while ( DHT11_READ_IO && retry < 100 )  // 等待变为低电平
    {
        retry++;
        delay_us( 1 );
    }
    retry = 0;
    while ( !DHT11_READ_IO && retry < 100 )  // 等待变高电平
    {
        retry++;
        delay_us( 1 );
    }
    delay_us( 40 );  // 等待40us
    if ( DHT11_READ_IO )
        return 1;
    else
        return 0;
}
// 从DHT11读取一个字节
// 返回值：读到的数据
uint8_t DHT11_Read_Byte( void ) {
    uint8_t i, dat;
    dat = 0;
    for ( i = 0; i < 8; i++ ) {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    return dat;
}
// 从DHT11读取一次数据
// temp:温度值(范围:0~50°)
// humi:湿度值(范围:20%~90%)
// 返回值：0,正常;1,读取失败
uint8_t DHT11_Read_Data( void ) {
    uint8_t buf[ 5 ];
    uint8_t i;
    DHT11_Rst();
    if ( DHT11_Check() == 0 ) {
        for ( i = 0; i < 5; i++ )  // 读取40位数据
        {
            buf[ i ] = DHT11_Read_Byte();
        }
        if ( ( buf[ 0 ] + buf[ 1 ] + buf[ 2 ] + buf[ 3 ] ) == buf[ 4 ] ) {
            humi         = buf[ 0 ];
            humi_decimal = buf[ 1 ];
            temp         = buf[ 2 ];
            temp_decimal = buf[ 3 ];
        }
    } else
        return 1;
    return 0;
}
// 初始化DHT11的IO口 DQ 同时检测DHT11的存在
// 返回1:不存在
// 返回0:存在
uint8_t DHT11_Init( void ) {
    DHT11_Rst();           // 复位
    return DHT11_Check();  // 检查DHT11传感是否存在
}

/**
 * @brief  温湿度传感器主函数
 * @param  void
 * @retval None
 */
void DHT11( void ) {
    DHT11_Read_Data();  // 读取温湿度
    HAL_Delay( 100 );   // 主机连续采样间隔建议不小于100ms
}
