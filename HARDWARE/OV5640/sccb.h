/**
 ****************************************************************************************************
 * @file        sccb.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-09-06
 * @brief       OV系列摄像头 SCCB 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 阿波罗 H743开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20220906
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __SCCB_H
#define __SCCB_H

#include "main.h"
#include "sys.h"

/******************************************************************************************/

/* 引脚 定义 */
// #define SCCB_SCL_GPIO_PORT GPIOB
// #define SCCB_SCL_GPIO_PIN  GPIO_PIN_4
// #define SCCB_SCL_GPIO_CLK_ENABLE()    \
//     do {                              \
//         __HAL_RCC_GPIOB_CLK_ENABLE(); \
//     } while ( 0 ) /* PB口时钟使能 */

// #define SCCB_SDA_GPIO_PORT GPIOB
// #define SCCB_SDA_GPIO_PIN  GPIO_PIN_3
// #define SCCB_SDA_GPIO_CLK_ENABLE()    \
//     do {                              \
//         __HAL_RCC_GPIOB_CLK_ENABLE(); \
//     } while ( 0 ) /* PB口时钟使能 */

#define OV_SCCB_TYPE_NOD 0 /* 如果不用开漏模式或SCCB上无上拉电阻，我们需要推挽和输入切换的方式 */
#if OV_SCCB_TYPE_NOD
#define SCCB_SDA_IN()                        \
    {                                        \
        GPIOB->MODER &= ~( 3 << ( 7 * 2 ) ); \
        GPIOB->MODER |= 0 << ( 7 * 2 );      \
    } /* PB3 输入 */
#define SCCB_SDA_OUT()                       \
    {                                        \
        GPIOB->MODER &= ~( 3 << ( 7 * 2 ) ); \
        GPIOB->MODER |= 1 << ( 7 * 2 );      \
    } /* PB3 输出 */
#endif

/******************************************************************************************/

/* IO操作函数 */
#define SCCB_SCL( x )                                                                                                                                    \
    do {                                                                                                                                                 \
        x ? HAL_GPIO_WritePin( SCCB_SCL_GPIO_Port, SCCB_SCL_Pin, GPIO_PIN_SET ) : HAL_GPIO_WritePin( SCCB_SCL_GPIO_Port, SCCB_SCL_Pin, GPIO_PIN_RESET ); \
    } while ( 0 ) /* SCL */

#define SCCB_SDA( x )                                                                                                                                    \
    do {                                                                                                                                                 \
        x ? HAL_GPIO_WritePin( SCCB_SDA_GPIO_Port, SCCB_SDA_Pin, GPIO_PIN_SET ) : HAL_GPIO_WritePin( SCCB_SDA_GPIO_Port, SCCB_SDA_Pin, GPIO_PIN_RESET ); \
    } while ( 0 ) /* SDA */

#define SCCB_READ_SDA HAL_GPIO_ReadPin( SCCB_SDA_GPIO_Port, SCCB_SDA_Pin ) /* 读取SDA */

/******************************************************************************************/

/* 对外接口函数 */
void    sccb_init( void );
void    sccb_start( void );
void    sccb_stop( void );
void    sccb_nack( void );
uint8_t sccb_send_byte( uint8_t data );
uint8_t sccb_read_byte( void );

#endif
