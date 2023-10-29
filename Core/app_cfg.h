/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*	模块名称 : uCOS-III的应用配置
*	文件名称 : app_cfg.c
*	版    本 : V1.0
*	说    明 : ucos-ii的应用配置
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2015-08-10 Eric2013  正式发布
*
*	Copyright (C), 2015-2016, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#ifndef APP_CFG_MODULE_PRESENT
#define APP_CFG_MODULE_PRESENT

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define APP_CFG_TASK_START_PRIO 2u
#define APP_CFG_TASK_MsgPro_PRIO 3u
#define APP_CFG_TASK_USER_IF_PRIO 4u
#define APP_CFG_TASK_COM_PRIO 5u

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define APP_CFG_TASK_START_STK_SIZE 512u
#define APP_CFG_TASK_MsgPro_STK_SIZE 512u
#define APP_CFG_TASK_COM_STK_SIZE 512u
#define APP_CFG_TASK_USER_IF_STK_SIZE 512u

/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#ifndef TRACE_LEVEL_OFF
#define TRACE_LEVEL_OFF 0u
#endif

#ifndef TRACE_LEVEL_INFO
#define TRACE_LEVEL_INFO 1u
#endif

#ifndef TRACE_LEVEL_DBG
#define TRACE_LEVEL_DBG 2u
#endif

#define APP_TRACE_LEVEL TRACE_LEVEL_OFF
#define APP_TRACE printf

#define APP_TRACE_INFO(x) ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO) ? (void)(APP_TRACE x) : (void)0)
#define APP_TRACE_DBG(x) ((APP_TRACE_LEVEL >= TRACE_LEVEL_DBG) ? (void)(APP_TRACE x) : (void)0)

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
