/**
 ******************************************************************
 * @file    bsp_esp8266.c
 * @author  fire
 * @version V1.0
 * @date    2019-xx-xx
 * @brief   WIFI测试
 ******************************************************************
 * @attention
 *
 * 实验平台:野火 STM32H743 开发板
 * 论坛    :http://www.firebbs.cn
 * 淘宝    :https://fire-stm32.taobao.com
 *
 ******************************************************************
 */
#include "ESP8266.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "delay.h"

// UART_HandleTypeDef ESP8266_UartHandle;
// static void ESP8266_GPIO_Config( void );
// static void ESP8266_USART_Config( void );

struct STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };

/**
 * @brief  ESP8266初始化函数
 * @param  无
 * @retval 无
 */
void ESP8266_Init( void ) {
    // ESP8266_GPIO_Config();

    // ESP8266_USART_Config();

    // macESP8266_RST_HIGH_LEVEL();

    // macESP8266_CH_DISABLE();
}

/**
 * @brief  初始化ESP8266用到的 USART
 * @param  无
 * @retval 无
 */
// static void ESP8266_USART_Config( void ) {
//     GPIO_InitTypeDef         GPIO_InitStruct;
//     RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

//     macESP8266_USART_TX_GPIO_CLK();
//     macESP8266_USART_RX_GPIO_CLK();

//     /* 配置串口1时钟源*/
//     RCC_PeriphClkInit.PeriphClockSelection  = RCC_PERIPHCLK_USART6;
//     RCC_PeriphClkInit.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
//     HAL_RCCEx_PeriphCLKConfig( &RCC_PeriphClkInit );
//     /* 使能串口6时钟 */
//     macESP8266_USART_CLK_ENABLE();

//     /* 配置Tx引脚为复用功能  */
//     GPIO_InitStruct.Pin       = macESP8266_USART_TX_PIN;
//     GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//     GPIO_InitStruct.Pull      = GPIO_PULLUP;
//     GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
//     GPIO_InitStruct.Alternate = macESP8266_USART_TX_AF;
//     HAL_GPIO_Init( macESP8266_USART_TX_PORT, &GPIO_InitStruct );

//     /* 配置Rx引脚为复用功能 */
//     GPIO_InitStruct.Pin       = macESP8266_USART_RX_PIN;
//     GPIO_InitStruct.Alternate = macESP8266_USART_RX_AF;
//     HAL_GPIO_Init( macESP8266_USART_RX_PORT, &GPIO_InitStruct );

//     /* 配置串GPS_USART 模式 */
//     ESP8266_UartHandle.Instance        = macESP8266_USARTx;
//     ESP8266_UartHandle.Init.BaudRate   = macESP8266_USART_BAUD_RATE;
//     ESP8266_UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
//     ESP8266_UartHandle.Init.StopBits   = UART_STOPBITS_1;
//     ESP8266_UartHandle.Init.Parity     = UART_PARITY_NONE;
//     ESP8266_UartHandle.Init.Mode       = UART_MODE_TX_RX;
//     ESP8266_UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
//     HAL_UART_Init( &ESP8266_UartHandle );

//     /*配置中断优先级*/
//     HAL_NVIC_SetPriority( macESP8266_USART_IRQ, 1, 0 );
//     /*使能DMA中断*/
//     HAL_NVIC_EnableIRQ( macESP8266_USART_IRQ );
//     /*配置中断条件*/
//     __HAL_USART_ENABLE_IT( &ESP8266_UartHandle, USART_IT_RXNE );
//     __HAL_USART_ENABLE_IT( &ESP8266_UartHandle, USART_IT_IDLE );
// }

/*
 * 函数名：ESP8266_Rst
 * 描述  ：重启WF-ESP8266模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被 ESP8266_AT_Test 调用
 */
void ESP8266_Rst( void ) {
#if 1
    ESP8266_Cmd( "AT+RST", "OK", "ready", 2500 );

#else
    // macESP8266_RST_LOW_LEVEL();
    delay_ms( 500 );
    // macESP8266_RST_HIGH_LEVEL();

#endif
}

bool ESP8266_DHCP_CUR() {
    char cCmd[ 40 ];

    sprintf( cCmd, "AT+CWDHCP_CUR=1,1" );  // 设置 ESP8266 Station 开启 DHCP

    return ESP8266_Cmd( cCmd, "OK", NULL, 500 );
}

/*
 * 函数名：ESP8266_Cmd
 * 描述  ：对WF-ESP8266模块发送AT指令
 * 输入  ：cmd，待发送的指令
 *         reply1，reply2，期待的响应，为NULL表不需响应，两者为或逻辑关系
 *         waittime，等待响应的时间
 * 返回  : 1，指令发送成功
 *         0，指令发送失败
 * 调用  ：被外部调用
 */
bool ESP8266_Cmd( char* cmd, char* reply1, char* reply2, uint32_t waittime ) {
    strEsp8266_Fram_Record.InfBit.FramLength = 0;  // 从新开始接收新的数据包

    macESP8266_Usart( "%s\r\n", cmd );  // 向ESP32发送命令

    if ( ( reply1 == 0 ) && ( reply2 == 0 ) )  // 不需要接收数据
        return true;

    delay_ms( waittime );  // 延时

    strEsp8266_Fram_Record.Data_RX_BUF[ strEsp8266_Fram_Record.InfBit.FramLength ] = '\0';

    macPC_Usart( "%s", strEsp8266_Fram_Record.Data_RX_BUF );  // 中断接收数据，通过USART1发送至PC

    if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
        return ( ( bool )strstr( strEsp8266_Fram_Record.Data_RX_BUF, reply1 ) || ( bool )strstr( strEsp8266_Fram_Record.Data_RX_BUF, reply2 ) );
    else if ( reply1 != 0 )
        return ( ( bool )strstr( strEsp8266_Fram_Record.Data_RX_BUF, reply1 ) );
    else
        return ( ( bool )strstr( strEsp8266_Fram_Record.Data_RX_BUF, reply2 ) );
}

/*
 * 函数名：ESP8266_AT_Test
 * 描述  ：对WF-ESP8266模块进行AT测试启动
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
// void ESP8266_AT_Test ( void )
//{
//	macESP8266_RST_HIGH_LEVEL();
//
//	Delay_ms ( 1000 );
//
//	while ( ! ESP8266_Cmd ( "AT", "OK", NULL, 200 ) ) ESP8266_Rst ();

//}
void ESP8266_AT_Test( void ) {
    char count = 0;

    // macESP8266_RST_HIGH_LEVEL();
    delay_ms( 1000 );
    while ( count < 10 ) {
        if ( ESP8266_Cmd( "AT", "OK", NULL, 500 ) )  // 测试 AT 启动
            return;
        ESP8266_Rst();
        ++count;
    }
}

/*
 * 函数名：ESP8266_Net_Mode_Choose
 * 描述  ：选择WF-ESP8266模块的工作模式
 * 输入  ：enumMode，工作模式
 * 返回  : 1，选择成功
 *         0，选择失败
 * 调用  ：被外部调用
 */
bool ESP8266_Net_Mode_Choose( ENUM_Net_ModeTypeDef enumMode ) {
    switch ( enumMode ) {
    case STA:
        return ESP8266_Cmd( "AT+CWMODE=1", "OK", "no change", 2500 );

    case AP:
        return ESP8266_Cmd( "AT+CWMODE=2", "OK", "no change", 2500 );

    case STA_AP:
        return ESP8266_Cmd( "AT+CWMODE=3", "OK", "no change", 2500 );

    default:
        return false;
    }
}

/*
 * 函数名：ESP8266_JoinAP
 * 描述  ：WF-ESP8266模块连接外部WiFi
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool ESP8266_JoinAP( char* pSSID, char* pPassWord ) {
    char cCmd[ 120 ];

    sprintf( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );  // 设置 ESP8266 Station 需连接的 AP

    return ESP8266_Cmd( cCmd, "OK", NULL, 5000 );
}

/*
 * 函数名：ESP8266_BuildAP
 * 描述  ：WF-ESP8266模块创建WiFi热点
 * 输入  ：pSSID，WiFi名称字符串
 *       ：pPassWord，WiFi密码字符串
 *       ：enunPsdMode，WiFi加密方式代号字符串
 * 返回  : 1，创建成功
 *         0，创建失败
 * 调用  ：被外部调用
 */
bool ESP8266_BuildAP( char* pSSID, char* pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode ) {
    char cCmd[ 120 ];

    sprintf( cCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, enunPsdMode );

    return ESP8266_Cmd( cCmd, "OK", 0, 1000 );
}

/*
 * 函数名：ESP8266_Enable_MultipleId
 * 描述  ：WF-ESP8266模块启动多连接
 * 输入  ：enumEnUnvarnishTx，配置是否多连接
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool ESP8266_Enable_MultipleId( FunctionalState enumEnUnvarnishTx ) {
    char cStr[ 20 ];

    sprintf( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );  // 配置多连接

    return ESP8266_Cmd( cStr, "OK", 0, 500 );
}

/*
 * 函数名：ESP8266_Link_Server
 * 描述  ：WF-ESP8266模块连接外部服务器
 * 输入  ：enumE，网络协议
 *       ：ip，服务器IP字符串
 *       ：ComNum，服务器端口字符串
 *       ：id，模块连接服务器的ID
 * 返回  : 1，连接成功
 *         0，连接失败
 * 调用  ：被外部调用
 */
bool ESP8266_Link_Server( ENUM_NetPro_TypeDef enumE, char* ip, char* ComNum, ENUM_ID_NO_TypeDef id ) {
    char cStr[ 100 ] = { 0 }, cCmd[ 120 ];

    switch ( enumE ) {
    case enumTCP:
        sprintf( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );  // 建立TCP连接
        break;

    case enumUDP:
        sprintf( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );  // 建立UDP连接
        break;

    default:
        break;
    }

    if ( id < 5 )
        sprintf( cCmd, "AT+CIPSTART=%d,%s", id, cStr );

    else
        sprintf( cCmd, "AT+CIPSTART=%s", cStr );

    return ESP8266_Cmd( cCmd, "OK", "ALREAY CONNECT", 4000 );
}

/*
 * 函数名：ESP8266_StartOrShutServer
 * 描述  ：WF-ESP8266模块开启或关闭服务器模式
 * 输入  ：enumMode，开启/关闭
 *       ：pPortNum，服务器端口号字符串
 *       ：pTimeOver，服务器超时时间字符串，单位：秒
 * 返回  : 1，操作成功
 *         0，操作失败
 * 调用  ：被外部调用
 */
bool ESP8266_StartOrShutServer( FunctionalState enumMode, char* pPortNum, char* pTimeOver ) {
    char cCmd1[ 120 ], cCmd2[ 120 ];

    if ( enumMode ) {
        sprintf( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );

        sprintf( cCmd2, "AT+CIPSTO=%s", pTimeOver );

        return ( ESP8266_Cmd( cCmd1, "OK", 0, 500 ) && ESP8266_Cmd( cCmd2, "OK", 0, 500 ) );
    }

    else {
        sprintf( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

        return ESP8266_Cmd( cCmd1, "OK", 0, 500 );
    }
}

/*
 * 函数名：ESP8266_Get_LinkStatus
 * 描述  ：获取 WF-ESP8266 的连接状态，较适合单端口时使用
 * 输入  ：无
 * 返回  : 2，获得ip
 *         3，建立连接
 *         3，失去连接
 *         0，获取状态失败
 * 调用  ：被外部调用
 */
uint8_t ESP8266_Get_LinkStatus( void ) {
    if ( ESP8266_Cmd( "AT+CIPSTATUS", "OK", 0, 500 ) ) {
        if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:2\r\n" ) )
            return 2;

        else if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:3\r\n" ) )
            return 3;

        else if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:4\r\n" ) )
            return 4;
    }

    return 0;
}

/*
 * 函数名：ESP8266_Get_IdLinkStatus
 * 描述  ：获取 WF-ESP8266 的端口（Id）连接状态，较适合多端口时使用
 * 输入  ：无
 * 返回  : 端口（Id）的连接状态，低5位为有效位，分别对应Id5~0，某位若置1表该Id建立了连接，若被清0表该Id未建立连接
 * 调用  ：被外部调用
 */
uint8_t ESP8266_Get_IdLinkStatus( void ) {
    uint8_t ucIdLinkStatus = 0x00;

    if ( ESP8266_Cmd( "AT+CIPSTATUS", "OK", 0, 500 ) ) {
        if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "+CIPSTATUS:0," ) )
            ucIdLinkStatus |= 0x01;
        else
            ucIdLinkStatus &= ~0x01;

        if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "+CIPSTATUS:1," ) )
            ucIdLinkStatus |= 0x02;
        else
            ucIdLinkStatus &= ~0x02;

        if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "+CIPSTATUS:2," ) )
            ucIdLinkStatus |= 0x04;
        else
            ucIdLinkStatus &= ~0x04;

        if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "+CIPSTATUS:3," ) )
            ucIdLinkStatus |= 0x08;
        else
            ucIdLinkStatus &= ~0x08;

        if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "+CIPSTATUS:4," ) )
            ucIdLinkStatus |= 0x10;
        else
            ucIdLinkStatus &= ~0x10;
    }

    return ucIdLinkStatus;
}

/*
 * 函数名：ESP8266_Inquire_ApIp
 * 描述  ：获取 F-ESP8266 的 AP IP
 * 输入  ：pApIp，存放 AP IP 的数组的首地址
 *         ucArrayLength，存放 AP IP 的数组的长度
 * 返回  : 0，获取失败
 *         1，获取成功
 * 调用  ：被外部调用
 */
uint8_t ESP8266_Inquire_ApIp( char* pApIp, uint8_t ucArrayLength ) {
    char uc;

    char* pCh;

    ESP8266_Cmd( "AT+CIFSR", "OK", 0, 500 );

    pCh = strstr( strEsp8266_Fram_Record.Data_RX_BUF, "APIP,\"" );

    if ( pCh )
        pCh += 6;

    else
        return 0;

    for ( uc = 0; uc < ucArrayLength; uc++ ) {
        pApIp[ uc ] = *( pCh + uc );

        if ( pApIp[ uc ] == '\"' ) {
            pApIp[ uc ] = '\0';
            break;
        }
    }

    return 1;
}

/*
 * 函数名：ESP8266_UnvarnishSend
 * 描述  ：配置WF-ESP8266模块进入透传发送
 * 输入  ：无
 * 返回  : 1，配置成功
 *         0，配置失败
 * 调用  ：被外部调用
 */
bool ESP8266_UnvarnishSend( void ) {
    if ( !ESP8266_Cmd( "AT+CIPMODE=1", "OK", 0, 500 ) )  // 若未成功设置透传模式，直接返回
        return false;

    return ESP8266_Cmd( "AT+CIPSEND", "OK", ">", 500 );  // 发送数据是否成功
}

/*
 * 函数名：ESP8266_ExitUnvarnishSend
 * 描述  ：配置WF-ESP8266模块退出透传模式
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
void ESP8266_ExitUnvarnishSend( void ) {
    delay_ms( 1000 );

    macESP8266_Usart( "+++" );

    delay_ms( 500 );
}

/*
 * 函数名：ESP8266_SendString
 * 描述  ：WF-ESP8266模块发送字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 *       ：pStr，要发送的字符串
 *       ：ulStrLength，要发送的字符串的字节数
 *       ：ucId，哪个ID发送的字符串
 * 返回  : 1，发送成功
 *         0，发送失败
 * 调用  ：被外部调用
 */
bool ESP8266_SendString( FunctionalState enumEnUnvarnishTx, char* pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId ) {
    char cStr[ 20 ];
    bool bRet = false;

    if ( enumEnUnvarnishTx ) {
        macESP8266_Usart( "%s", pStr );

        bRet = true;

    }

    else {
        if ( ucId < 5 )
            sprintf( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

        else
            sprintf( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );

        ESP8266_Cmd( cStr, "> ", 0, 100 );

        bRet = ESP8266_Cmd( pStr, "SEND OK", 0, 500 );
    }

    return bRet;
}

/*
 * 函数名：ESP8266_ReceiveString
 * 描述  ：WF-ESP8266模块接收字符串
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 * 返回  : 接收到的字符串首地址
 * 调用  ：被外部调用
 */
char* ESP8266_ReceiveString( FunctionalState enumEnUnvarnishTx ) {
    char* pRecStr = 0;

    strEsp8266_Fram_Record.InfBit.FramLength     = 0;
    strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;

    while ( !strEsp8266_Fram_Record.InfBit.FramFinishFlag )
        ;
    strEsp8266_Fram_Record.Data_RX_BUF[ strEsp8266_Fram_Record.InfBit.FramLength ] = '\0';

    if ( enumEnUnvarnishTx )
        pRecStr = strEsp8266_Fram_Record.Data_RX_BUF;

    else {
        if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "+IPD" ) )
            pRecStr = strEsp8266_Fram_Record.Data_RX_BUF;
    }

    return pRecStr;
}

/*
 * 函数名：ESP8266_CWLIF
 * 描述  ：查询已接入设备的IP
 * 输入  ：pStaIp，存放已接入设备的IP
 * 返回  : 1，有接入设备
 *         0，无接入设备
 * 调用  ：被外部调用
 */
uint8_t ESP8266_CWLIF( char* pStaIp ) {
    uint8_t uc, ucLen;

    char *pCh, *pCh1;

    ESP8266_Cmd( "AT+CWLIF", "OK", 0, 100 );

    pCh = strstr( strEsp8266_Fram_Record.Data_RX_BUF, "," );

    if ( pCh ) {
        pCh1  = strstr( strEsp8266_Fram_Record.Data_RX_BUF, "AT+CWLIF\r\r\n" ) + 11;
        ucLen = pCh - pCh1;
    }

    else
        return 0;

    for ( uc = 0; uc < ucLen; uc++ )
        pStaIp[ uc ] = *( pCh1 + uc );

    pStaIp[ ucLen ] = '\0';

    return 1;
}

/*
 * 函数名：ESP8266_CIPAP
 * 描述  ：设置模块的 AP IP
 * 输入  ：pApIp，模块的 AP IP
 * 返回  : 1，设置成功
 *         0，设置失败
 * 调用  ：被外部调用
 */
uint8_t ESP8266_CIPAP( char* pApIp ) {
    char cCmd[ 30 ];

    sprintf( cCmd, "AT+CIPAP=\"%s\"", pApIp );

    if ( ESP8266_Cmd( cCmd, "OK", 0, 5000 ) )
        return 1;

    else
        return 0;
}

volatile uint8_t ucTcpClosedFlag = 0;

/**
 * @brief  ESP8266 （Sta Tcp Client）透传
 * @param  无
 * @retval 无
 */
void ESP8266_StaTcpClient_UnvarnishTest( void ) {
    uint8_t ucStatus;

    char cStr[ 100 ] = { 0 };

    // DHT11_Data_TypeDef DHT11_Data;

    printf( "\r\n正在配置 ESP8266 ......\r\n" );

    // macESP8266_CH_ENABLE();

    ESP8266_AT_Test();  // 测试 AT 启动
    // while ( !ESP8266_DHCP_CUR() )  // 动态IP设置成功
    //     ;

    ESP8266_Net_Mode_Choose( STA );  // 设置 Wi-Fi 模式 Station

    while ( !ESP8266_JoinAP( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) )  // 连接目标AP成功
        ;

    ESP8266_Enable_MultipleId( DISABLE );  // 配置非多连接

    while ( !ESP8266_Link_Server( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) )  // 建立TCP连接成功
        ;

    while ( !ESP8266_UnvarnishSend() )  // 发送数据成功
        ;

    printf( "\r\n配置 ESP8266 完毕\r\n" );

    while ( 1 ) {
        // if ( Read_DHT11( &DHT11_Data ) == SUCCESS )  // 读取 DHT11 温湿度信息
        uint8_t humi_int  = 12;
        uint8_t humi_deci = 13;
        uint8_t temp_int  = 14;
        uint8_t temp_deci = 15;
        sprintf( cStr, "\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n", humi_int, humi_deci, temp_int, temp_deci );

        // else sprintf( cStr, "Read DHT11 ERROR!\r\n" );

        printf( "%s", cStr );  // 打印读取 DHT11 温湿度信息

        ESP8266_SendString( ENABLE, cStr, 0, Single_ID_0 );  // 发送 DHT11 温湿度信息到网络调试助手

        delay_ms( 1500 );

        if ( ucTcpClosedFlag )  // 检测是否失去连接
        {
            ESP8266_ExitUnvarnishSend();  // 退出透传模式

            do
                ucStatus = ESP8266_Get_LinkStatus();  // 获取连接状态
            while ( !ucStatus );

            if ( ucStatus == 4 )  // 确认失去连接后重连
            {
                printf( "\r\n正在重连热点和服务器 ......\r\n" );

                while ( !ESP8266_JoinAP( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) )
                    ;

                while ( !ESP8266_Link_Server( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) )
                    ;

                printf( "\r\n重连热点和服务器成功\r\n" );
            }

            while ( !ESP8266_UnvarnishSend() )
                ;
        }
    }
}
