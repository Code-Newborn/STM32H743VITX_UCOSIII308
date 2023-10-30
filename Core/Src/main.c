/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "includes.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
*********************************************************************************************************
*                                       静态全局变量
*********************************************************************************************************
*/
static OS_TCB AppTaskStartTCB;
static CPU_STK AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static OS_TCB AppTaskMsgProTCB;
static CPU_STK AppTaskMsgProStk[APP_CFG_TASK_MsgPro_STK_SIZE];

// static OS_TCB AppTaskCOMTCB;
// static CPU_STK AppTaskCOMStk[APP_CFG_TASK_COM_STK_SIZE];

// static OS_TCB AppTaskUserIFTCB;
// static CPU_STK AppTaskUserIFStk[APP_CFG_TASK_USER_IF_STK_SIZE];

/*
*********************************************************************************************************
*                                      函数声明
*********************************************************************************************************
*/
static void AppTaskStart(void *p_arg);
static void AppTaskMsgPro(void *p_arg);
// static void AppTaskUserIF(void *p_arg);
// static void AppTaskCOM(void *p_arg);
static void AppTaskCreate(void);
// static void DispTaskInfo(void);
static void AppObjCreate(void);
static void App_Printf(CPU_CHAR *format, ...);

/*
*******************************************************************************************************
*                               变量
*******************************************************************************************************
*/
static OS_SEM AppPrintfSemp; /* 用于printf互斥 */
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */
  OS_ERR err;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* 初始化滴答时钟，在启动任务里面开启 */
  BSP_OS_TickInit();

  /* 初始化uC/OS-III 内核 */
  OSInit(&err);

  /* USER CODE END 2 */

  /* Infinite loop */

  /* 创建一个启动任务（也就是主任务）。启动任务会创建所有的应用程序任务 */
  OSTaskCreate((OS_TCB *)&AppTaskStartTCB,                     /* 任务控制块地址 */
               (CPU_CHAR *)"App Task Start",                   /* 任务名 */
               (OS_TASK_PTR)AppTaskStart,                      /* 启动任务函数地址 */
               (void *)0,                                      /* 传递给任务的参数 */
               (OS_PRIO)APP_CFG_TASK_START_PRIO,               /* 任务优先级 */
               (CPU_STK *)&AppTaskStartStk[0],                 /* 堆栈基地址 */
               (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE / 10, /* 堆栈监测区，这里表示后10%作为监测区 */
               (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,      /* 堆栈空间大小 */
               (OS_MSG_QTY)0,                                  /* 本任务支持接受的最大消息数 */
               (OS_TICK)0,                                     /* 设置时间片 */
               (void *)0,                                      /* 堆栈空间大小 */
               (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),

               /*  定义如下：
                OS_OPT_TASK_STK_CHK      使能检测任务栈，统计任务栈已用的和未用的
                OS_OPT_TASK_STK_CLR      在创建任务时，清零任务栈
                OS_OPT_TASK_SAVE_FP      如果CPU有浮点寄存器，则在任务切换时保存浮点寄存器的内容
               */
               (OS_ERR *)&err);

  /* 启动多任务系统，控制权交给uC/OS-III */
  OSStart(&err);

  (void)&err;

  return (0);
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
   */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }
  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 25;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/*
*********************************************************************************************************
*	函 数 名: AppTaskStart
*	功能说明: 这是一个启动任务，在多任务系统启动后，必须初始化滴答计数器。本任务主要实现按键检测。
*	形    参: p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
  优 先 级: 2
*********************************************************************************************************
*/
static void AppTaskStart(void *p_arg)
{
  OS_ERR err;

  (void)p_arg;

  HAL_ResumeTick();

  CPU_Init(); /* 此函数要优先调用，因为外设驱动中使用的us和ms延迟是基于此函数的 */
  // bsp_Init();
  BSP_OS_TickEnable();

#if OS_CFG_STAT_TASK_EN > 0u
  OSStatTaskCPUUsageInit(&err);
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
  CPU_IntDisMeasMaxCurReset();
#endif

  /* 创建任务 */
  AppTaskCreate();

  /* 创建任务间通信机制 */
  AppObjCreate();

  while (1)
  {
    /* 需要周期性处理的程序，对应裸机工程调用的SysTick_ISR */
    // bsp_ProPer1ms();
    OSTimeDly(1, OS_OPT_TIME_PERIODIC, &err);
  }
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskMsgPro
*	功能说明: 消息处理，这里用作浮点数串口打印
*	形    参: p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
  优 先 级: 3
*********************************************************************************************************
*/
static void AppTaskMsgPro(void *p_arg)
{
  OS_ERR err;
  double f_a = 1.1;
  double f_b = 2.2345;

  (void)p_arg;

  while (1)
  {
    f_a += 0.00000000001;
    f_b -= 0.00000000002;
    App_Printf("AppTaskMsg: f_a = %.11f, f_b = %.11f\r\n", f_a, f_b);
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    // bsp_LedToggle(2);
    OSTimeDly(100, OS_OPT_TIME_DLY, &err);
  }
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskUserIF
*	功能说明: 按键消息处理
*	形    参: p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
  优 先 级: 4
*********************************************************************************************************
*/
// static void AppTaskUserIF(void *p_arg)
// {
//   OS_ERR err;
//   uint8_t ucKeyCode; /* 按键代码 */

//   (void)p_arg;

//   while (1)
//   {
//     ucKeyCode = bsp_GetKey();

//     if (ucKeyCode != KEY_NONE)
//     {
//       switch (ucKeyCode)
//       {
//       case KEY_DOWN_K1: /* K1键按下 打印任务执行情况 */
//         DispTaskInfo();
//         break;

//       default: /* 其他的键值不处理 */
//         break;
//       }
//     }

//     OSTimeDly(20, OS_OPT_TIME_DLY, &err);
//   }
// }

/*
*********************************************************************************************************
*	函 数 名: AppTaskCom
*	功能说明: 浮点数串口打印
*	形    参: p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
  优 先 级: 5
*********************************************************************************************************
*/
// static void AppTaskCOM(void *p_arg)
// {
//   OS_ERR err;
//   double f_c = 1.1;
//   double f_d = 2.2345;

//   (void)p_arg;

//   while (1)
//   {
//     f_c += 0.00000000001;
//     f_d -= 0.00000000002;
//     ;
//     App_Printf("AppTaskCom: f_a = %.11f, f_b = %.11f\r\n", f_c, f_d);
//     bsp_LedToggle(4);
//     OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
//   }
// }

/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate(void)
{
  OS_ERR err;

  /**************创建MsgPro任务*********************/
  OSTaskCreate((OS_TCB *)&AppTaskMsgProTCB,
               (CPU_CHAR *)"App Msp Pro",
               (OS_TASK_PTR)AppTaskMsgPro,
               (void *)0,
               (OS_PRIO)APP_CFG_TASK_MsgPro_PRIO,
               (CPU_STK *)&AppTaskMsgProStk[0],
               (CPU_STK_SIZE)APP_CFG_TASK_MsgPro_STK_SIZE / 10,
               (CPU_STK_SIZE)APP_CFG_TASK_MsgPro_STK_SIZE,
               (OS_MSG_QTY)0,
               (OS_TICK)0,
               (void *)0,
               (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
               (OS_ERR *)&err);

  /**************创建USER IF任务*********************/
  // OSTaskCreate((OS_TCB *)&AppTaskUserIFTCB,
  //              (CPU_CHAR *)"App Task UserIF",
  //              (OS_TASK_PTR)AppTaskUserIF,
  //              (void *)0,
  //              (OS_PRIO)APP_CFG_TASK_USER_IF_PRIO,
  //              (CPU_STK *)&AppTaskUserIFStk[0],
  //              (CPU_STK_SIZE)APP_CFG_TASK_USER_IF_STK_SIZE / 10,
  //              (CPU_STK_SIZE)APP_CFG_TASK_USER_IF_STK_SIZE,
  //              (OS_MSG_QTY)0,
  //              (OS_TICK)0,
  //              (void *)0,
  //              (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
  //              (OS_ERR *)&err);

  /**************创建COM任务*********************/
  // OSTaskCreate((OS_TCB *)&AppTaskCOMTCB,
  //              (CPU_CHAR *)"App Task COM",
  //              (OS_TASK_PTR)AppTaskCOM,
  //              (void *)0,
  //              (OS_PRIO)APP_CFG_TASK_COM_PRIO,
  //              (CPU_STK *)&AppTaskCOMStk[0],
  //              (CPU_STK_SIZE)APP_CFG_TASK_COM_STK_SIZE / 10,
  //              (CPU_STK_SIZE)APP_CFG_TASK_COM_STK_SIZE,
  //              (OS_MSG_QTY)0,
  //              (OS_TICK)0,
  //              (void *)0,
  //              (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
  //              (OS_ERR *)&err);
}

/*
*********************************************************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建任务通讯
*	形    参: p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppObjCreate(void)
{
  OS_ERR err;

  /*
     创建信号量数值为1的时候可以实现互斥功能，也就是只有一个资源可以使用
     本例程是将串口1的打印函数作为保护的资源。防止串口打印的时候被其它任务抢占
     造成串口打印错乱。
  */
  OSSemCreate((OS_SEM *)&AppPrintfSemp,
              (CPU_CHAR *)"AppPrintfSemp",
              (OS_SEM_CTR)1,
              (OS_ERR *)&err);
}

/*
*********************************************************************************************************
*	函 数 名: App_Printf
*	功能说明: 线程安全的printf方式
*	形    参: 同printf的参数。
*             在C中，当无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
*	返 回 值: 无
*********************************************************************************************************
*/
static void App_Printf(CPU_CHAR *format, ...)
{
  CPU_CHAR buf_str[200 + 1]; /* 特别注意，如果printf的变量较多，注意此局部变量的大小是否够用 */
  va_list v_args;
  OS_ERR os_err;

  va_start(v_args, format);
  (void)vsnprintf((char *)&buf_str[0],
                  (size_t)sizeof(buf_str),
                  (char const *)format,
                  v_args);
  va_end(v_args);

  /* 互斥操作 */
  OSSemPend((OS_SEM *)&AppPrintfSemp,
            (OS_TICK)0u,
            (OS_OPT)OS_OPT_PEND_BLOCKING,
            (CPU_TS *)0,
            (OS_ERR *)&os_err);

  printf("%s", buf_str);

  (void)OSSemPost((OS_SEM *)&AppPrintfSemp,
                  (OS_OPT)OS_OPT_POST_1,
                  (OS_ERR *)&os_err);
}

/*
*********************************************************************************************************
*	函 数 名: DispTaskInfo
*	功能说明: 将uCOS-III任务信息通过串口打印出来
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

// OS_CPU_USAGE k1;
// unsigned short k2;

// static void DispTaskInfo(void)
// {
//   OS_TCB *p_tcb; /* 定义一个任务控制块指针, TCB = TASK CONTROL BLOCK */
//   float CPU = 0.0f;
//   CPU_SR_ALLOC();

//   CPU_CRITICAL_ENTER();
//   p_tcb = OSTaskDbgListPtr;
//   CPU_CRITICAL_EXIT();

//   /* 打印标题 */
//   App_Printf("===============================================================\r\n");
//   App_Printf(" 优先级 使用栈 剩余栈 百分比 利用率   任务名\r\n");
//   App_Printf("  Prio   Used  Free   Per    CPU     Taskname\r\n");

//   /* 遍历任务控制块列表(TCB list)，打印所有的任务的优先级和名称 */
//   while (p_tcb != (OS_TCB *)0)
//   {
//     CPU = (float)p_tcb->CPUUsage / 100;
//     k1 = CPU;
//     k2 = p_tcb->StkUsed;
//     App_Printf("   %2d  %5d  %5d   %02d%%   %5.2f%%   %s\r\n",
//                p_tcb->Prio,
//                p_tcb->StkUsed,
//                p_tcb->StkFree,
//                (p_tcb->StkUsed * 100) / (p_tcb->StkUsed + p_tcb->StkFree),
//                CPU,
//                p_tcb->NamePtr);

//     CPU_CRITICAL_ENTER();
//     p_tcb = p_tcb->DbgNextPtr;
//     CPU_CRITICAL_EXIT();
//   }
// }

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
