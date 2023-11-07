/*
*********************************************************************************************************
*
*	模块名称 : 头文件汇总
*	文件名称 : includes.h
*	版    本 : V1.0
*	说    明 : 当前使用头文件汇总
*
*	修改记录 :
*		版本号    日期        作者     说明
*		V1.0    2015-08-02  Eric2013   首次发布
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __INCLUDES_H__
#define __INCLUDES_H__

/*
*********************************************************************************************************
*                                         标准库
*********************************************************************************************************
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
*********************************************************************************************************
*                                         其它库
*********************************************************************************************************
*/

#include <cpu.h>
#include <lib_def.h>
#include <lib_ascii.h>
#include <lib_math.h>
#include <lib_mem.h>
#include <lib_str.h>
#include <app_cfg.h>

/*
*********************************************************************************************************
*                                           OS
*********************************************************************************************************
*/

#include <os.h>

/*
*********************************************************************************************************
*                                           宏定义
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        APP / BSP
*********************************************************************************************************
*/

#include <bsp.h>
#include "bsp_os.h"

/*
*********************************************************************************************************
*                                          变量和函数
*********************************************************************************************************
*/
/* 方便RTOS里面使用 */
extern void SysTick_ISR(void);

#define bsp_ProPer1ms SysTick_ISR

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/