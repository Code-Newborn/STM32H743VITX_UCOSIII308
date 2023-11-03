#include "delay.h"
#include "stm32h7xx.h"
#include "core_cm7.h"
#include "stm32h7xx_hal.h"

static u32 fac_us = 0; // us延时倍乘数

// 初始化延迟函数
// 当使用ucos的时候,此函数会初始化ucos的时钟节拍
// SYSTICK的时钟固定为AHB时钟的1/8
// SYSCLK:系统时钟频率
void delay_init(u16 SYSCLK)
{
#if SYSTEM_SUPPORT_OS // 如果需要支持OS.
    u32 reload;
#endif
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); // SysTick频率为HCLK
    fac_us = SYSCLK;                                     // 不论是否使用OS,fac_us都需要使用
}

// 延时nus
// nus为要延时的us数.
// 注意:nus的值不要大于1000us
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD; // LOAD的值
    ticks = nus * fac_us;       // 需要的节拍数
    told = SysTick->VAL;        // 刚进入时的计数器值
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow; // 这里注意一下SYSTICK是一个递减的计数器就可以了.
            else
                tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break; // 时间超过/等于要延迟的时间,则退出.
        }
    };
}

// 延时nms
// nms:要延时的ms数
void delay_ms(u16 nms)
{
    u32 i;
    for (i = 0; i < nms; i++)
        delay_us(1000);
}
