#include "uC_OS3.h"
#include "usart.h"
#include "led.h"
#include "key.h"

/*uC/OS-III*********************************************************************************************/
#include "os.h"
#include "cpu.h"
#include "includes.h"
/******************************************************************************************************/
/*uC/OS-III配置*/

static OS_SEM AppPrintfSemp; /* 用于printf互斥 */
static void App_Printf(CPU_CHAR *format, ...);
static void DispTaskInfo(void);

/* START_TASK 初始任务 配置
 * 包括: 任务优先级 任务栈大小 任务控制块 任务栈 任务函数
 */
#define START_TASK_PRIO 2
#define START_TASK_STACK_SIZE 512
CPU_STK start_task_stack[START_TASK_STACK_SIZE];
OS_TCB start_task_tcb;
void start_task(void *p_arg);

/* STATISTIC_INFO_TASK 任务统计信息打印任务 配置
 * 包括: 任务优先级 任务栈大小 任务控制块 任务栈 任务函数
 */
#define STATISTICINFO_TASK_PRIO 4
#define STATISTICINFO_TASK_STACK_SIZE 256
CPU_STK statisticInfo_task_stack[STATISTICINFO_TASK_STACK_SIZE];
OS_TCB statisticInfo_task_tcb;
void statisticInfo_task(void *p_arg);

/* TASK1 任务 配置
 * 包括: 任务优先级 任务栈大小 任务控制块 任务栈 任务函数
 */
#define TASK1_PRIO 3
#define TASK1_STACK_SIZE 256
CPU_STK SetLed_task1_stack[TASK1_STACK_SIZE];
OS_TCB task1_tcb;
void SetLed(void *p_arg);

/* TASK2 任务 配置
 * 包括: 任务优先级 任务栈大小 任务控制块 任务栈 任务函数
 */
#define TASK2_PRIO 5
#define TASK2_STACK_SIZE 256
CPU_STK ScanKey_task2_stack[TASK2_STACK_SIZE];
OS_TCB task2_tcb;
void Task2(void *p_arg);

// VAR2DATATYPE Var2DataType;

void uc_os3(void)
{
    OS_ERR err;

    // 初始化uC/OS-III
    OSInit(&err);
    OSTaskCreate((OS_TCB *)&start_task_tcb,
                 (CPU_CHAR *)"start_task",
                 (OS_TASK_PTR)start_task,
                 (void *)0,
                 (OS_PRIO)START_TASK_PRIO,
                 (CPU_STK *)&start_task_stack[0],
                 (CPU_STK_SIZE)START_TASK_STACK_SIZE / 10,
                 (CPU_STK_SIZE)START_TASK_STACK_SIZE,
                 (OS_MSG_QTY)0,
                 (OS_TICK)0,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);
    if (err != OS_ERR_NONE)
    {
        // The task didn't get created. Look up the value of the error code ...
        // ... in OS.H for the meaning of the error
    }
    OSStart(&err); // 开始任务调度
}

void start_task(void *p_arg)
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

    /**************创建MsgPro任务 浮点数串口打印及闪灯*********************/
    OSTaskCreate((OS_TCB *)&task2_tcb,
                 (CPU_CHAR *)"Task2",
                 (OS_TASK_PTR)Task2,
                 (void *)0,
                 (OS_PRIO)TASK2_PRIO,
                 (CPU_STK *)&ScanKey_task2_stack[0],
                 (CPU_STK_SIZE)TASK2_STACK_SIZE / 10,
                 (CPU_STK_SIZE)TASK2_STACK_SIZE,
                 (OS_MSG_QTY)0,
                 (OS_TICK)0,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);

    /**************创建USER IF任务 按键消息打印CPU使用信息处理*********************/
    OSTaskCreate((OS_TCB *)&statisticInfo_task_tcb,
                 (CPU_CHAR *)"statisticInfo_task",
                 (OS_TASK_PTR)statisticInfo_task,
                 (void *)0,
                 (OS_PRIO)STATISTICINFO_TASK_PRIO,
                 (CPU_STK *)&statisticInfo_task_stack[0],
                 (CPU_STK_SIZE)STATISTICINFO_TASK_STACK_SIZE / 10,
                 (CPU_STK_SIZE)STATISTICINFO_TASK_STACK_SIZE,
                 (OS_MSG_QTY)0,
                 (OS_TICK)0,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);

    /**************创建COM任务 浮点数串口打印及闪灯*********************/
    OSTaskCreate((OS_TCB *)&task1_tcb,
                 (CPU_CHAR *)"SetLed",
                 (OS_TASK_PTR)SetLed,
                 (void *)0,
                 (OS_PRIO)TASK1_PRIO,
                 (CPU_STK *)&SetLed_task1_stack[0],
                 (CPU_STK_SIZE)TASK1_STACK_SIZE / 10,
                 (CPU_STK_SIZE)TASK1_STACK_SIZE,
                 (OS_MSG_QTY)0,
                 (OS_TICK)0,
                 (void *)0,
                 (OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR *)&err);

    /* 创建任务间通信机制 */
    OSSemCreate((OS_SEM *)&AppPrintfSemp,
                (CPU_CHAR *)"AppPrintfSemp",
                (OS_SEM_CTR)1,
                (OS_ERR *)&err);

    while (1)
    {
        /* 需要周期性处理的程序，对应裸机工程调用的SysTick_ISR */
        // bsp_ProPer1ms();
        OSTimeDly(1, OS_OPT_TIME_PERIODIC, &err);
    }
}

void statisticInfo_task(void *p_arg)
{
    OS_ERR err;

    (void)p_arg;

    while (1)
    {
        key = KEY_Scan(0);

        switch (key)
        {
        case KEY1_PRES: /* K1键按下 打印任务执行情况 */
            DispTaskInfo();
            break;
        case KEY2_PRES: /* K2键按下 不处理 */
            break;
        default: /* 其他的键值不处理 */
            break;
        }

        OSTimeDly(10, OS_OPT_TIME_DLY, &err);
    }
}

void SetLed(void *p_arg)
{
    OS_ERR err;
    double f_c = 1.1;
    double f_d = 2.2345;

    (void)p_arg;

    while (1)
    {
        f_c += 0.00000000001;
        f_d -= 0.00000000002;
        ;
        App_Printf("AppTaskCom: f_c = %.11f, f_d = %.11f\r\n", f_c, f_d);
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        OSTimeDly(250, OS_OPT_TIME_DLY, &err);
    }
}

/* 检测按键输入，挂起和恢复任务 */
void Task2(void *p_arg)
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
        OSTimeDly(1000, OS_OPT_TIME_DLY, &err);
    }
}

OS_CPU_USAGE k1;
unsigned short k2;

static void DispTaskInfo(void)
{
    OS_TCB *p_tcb; /* 定义一个任务控制块指针, TCB = TASK CONTROL BLOCK */
    float CPU = 0.0f;
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    p_tcb = OSTaskDbgListPtr;
    CPU_CRITICAL_EXIT();

    /* 打印标题 */
    App_Printf("===============================================================\r\n");
    App_Printf(" 优先级 使用栈 剩余栈 百分比 利用率   任务名\r\n");
    App_Printf("  Prio   Used  Free   Per    CPU     Taskname\r\n");

    /* 遍历任务控制块列表(TCB list)，打印所有的任务的优先级和名称 */
    while (p_tcb != (OS_TCB *)0)
    {
        CPU = (float)p_tcb->CPUUsage / 100;
        k1 = CPU;
        k2 = p_tcb->StkUsed;
        App_Printf("   %2d  %5d  %5d   %02d%%   %5.2f%%   %s\r\n",
                   p_tcb->Prio,
                   p_tcb->StkUsed,
                   p_tcb->StkFree,
                   (p_tcb->StkUsed * 100) / (p_tcb->StkUsed + p_tcb->StkFree),
                   CPU,
                   p_tcb->NamePtr);

        CPU_CRITICAL_ENTER();
        p_tcb = p_tcb->DbgNextPtr;
        CPU_CRITICAL_EXIT();
    }
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
