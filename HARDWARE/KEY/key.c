/**
 ****************************************************************************************************
 * @file        key.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-09-06
 * @brief       按键输入 驱动代码
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

#include "key.h"
#include "delay.h"

/**
 * @brief       按键扫描函数
 * @note        该函数有响应优先级(同时按下多个按键): WK_UP > KEY2 > KEY1 > KEY0!!
 * @param       mode:0 / 1, 具体含义如下:
 *   @arg       0,  不支持连续按(当按键按下不放时, 只有第一次调用会返回键值,
 *                  必须松开以后, 再次按下才会返回其他键值)
 *   @arg       1,  支持连续按(当按键按下不放时, 每次调用该函数都会返回键值)
 * @retval      键值, 定义如下:
 *              KEY0_PRES, 1, KEY0按下
 *              KEY1_PRES, 2, KEY1按下
 *              KEY2_PRES, 3, KEY2按下
 *              WKUP_PRES, 4, WKUP按下
 */
uint8_t key_scan( uint8_t mode ) {
    static uint8_t key_up = 1; /* 按键按松开标志 */
    uint8_t        keyval = 0;

    if ( mode )
        key_up = 1; /* 支持连按 */

    if ( key_up && ( KEY0 == 0 ) ) /* 按键松开标志为1, 且有任意一个按键按下了 */
    {
        delay_ms( 10 ); /* 去抖动 */
        key_up = 0;

        if ( KEY0 == 0 )
            keyval = KEY0_PRES;

    } else if ( KEY0 == 1 ) /* 没有任何按键按下, 标记按键松开 */
    {
        key_up = 1;
    }

    return keyval; /* 返回键值 */
}
