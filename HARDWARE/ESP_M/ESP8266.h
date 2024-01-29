#ifndef __ESP8266_H
#define __ESP8266_H

#include "common.h"
#include "main.h"
#include "stm32h7xx.h"

#include <stdbool.h>
#include <stdio.h>

#define LED1_ON  HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET )
#define LED1_OFF HAL_GPIO_WritePin( LED_GPIO_Port, LED_Pin, GPIO_PIN_SET )

/********************************** 用户需要设置的参数**********************************/
#define macUser_ESP8266_BulitApSsid        "YehuoLink"  // 要建立的热点的名称
#define macUser_ESP8266_BulitApPwd         "wildfire"   // 要建立的热点的密钥
#define macUser_ESP8266_BulitApEcn         OPEN         // 要建立的热点的加密方式
#define macUser_ESP8266_TcpServer_OverTime "1800"       // 服务器超时时间（单位：秒）

// 要连接的WIFI名称，密码
#define macUser_ESP8266_ApSsid "Redmi K50 Ultra"
#define macUser_ESP8266_ApPwd  "12345678"

// 要连接的服务器的 IP，即电脑的IP，通过cmd输入命令ipconfig查看IPv4地址
// #define macUser_ESP8266_TcpServer_IP "192.168.205.194"  // 连接手机热点用
#define macUser_ESP8266_TcpServer_IP "192.168.205.94"  // 连接手机热点用
// 要连接的服务器的端口，网络调试助手设置
#define macUser_ESP8266_TcpServer_Port "8080"

/********************************** 外部全局变量 ***************************************/
extern volatile uint8_t ucTcpClosedFlag;

/********************************** 测试函数声明 ***************************************/
void ESP8266_StaTcpClient_UnvarnishTest( void );
void ESP8266_StaTcpClient_UnvarnishTest_LedCtrl( void );
bool ESP8266_ConnectWiFi( void );

bool Get_Weather( void ) ;

// extern UART_HandleTypeDef ESP8266_UartHandle;
#if defined( __CC_ARM )
#pragma anon_unions
#endif

/******************************* ESP8266 数据类型定义 ***************************/
typedef enum { STA, AP, STA_AP } ENUM_Net_ModeTypeDef;

typedef enum {
    enumTCP,
    enumUDP,
} ENUM_NetPro_TypeDef;

typedef enum {
    Multiple_ID_0 = 0,
    Multiple_ID_1 = 1,
    Multiple_ID_2 = 2,
    Multiple_ID_3 = 3,
    Multiple_ID_4 = 4,
    Single_ID_0   = 5,
} ENUM_ID_NO_TypeDef;

typedef enum {
    OPEN         = 0,
    WEP          = 1,
    WPA_PSK      = 2,
    WPA2_PSK     = 3,
    WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;

typedef struct {
    uint8_t humi_int;   // 湿度的整数部分
    uint8_t humi_deci;  // 湿度的小数部分
    uint8_t temp_int;   // 温度的整数部分
    uint8_t temp_deci;  // 温度的小数部分
    uint8_t check_sum;  // 校验和
} DHT11_Data_TypeDef;

/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN 1024  // 最大接收缓存字节数

extern struct STRUCT_USARTx_Fram  // 串口数据帧的处理结构体
{
    char Data_RX_BUF[ RX_BUF_MAX_LEN ];

    union {
        __IO uint16_t InfAll;
        struct {
            __IO uint16_t FramLength : 15;     // 14:0
            __IO uint16_t FramFinishFlag : 1;  // 15
        } InfBit;
    };
} strEsp8266_Fram_Record;

/******************************** ESP8266 连接引脚定义 ***********************************/
// #define macESP8266_CH_PD_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
// #define macESP8266_CH_PD_PORT         GPIOB
// #define macESP8266_CH_PD_PIN          GPIO_PIN_1

// #define macESP8266_RST_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
// #define macESP8266_RST_PORT         GPIOB
// #define macESP8266_RST_PIN          GPIO_PIN_0

#define macESP8266_USART_BAUD_RATE 115200

#define macESP8266_USARTx USART2
// #define macESP8266_USART_CLK_ENABLE() __USART2_CLK_ENABLE()

// #define macESP8266_USART_TX_GPIO_CLK() __HAL_RCC_GPIOC_CLK_ENABLE()
// #define macESP8266_USART_TX_PORT       GPIOC
// #define macESP8266_USART_TX_PIN        GPIO_PIN_6
// #define macESP8266_USART_TX_AF         GPIO_AF7_USART6

// #define macESP8266_USART_RX_GPIO_CLK() __HAL_RCC_GPIOC_CLK_ENABLE()
// #define macESP8266_USART_RX_PORT       GPIOC
// #define macESP8266_USART_RX_PIN        GPIO_PIN_7
// #define macESP8266_USART_RX_AF         GPIO_AF7_USART6

#define macESP8266_USART_IRQ     USART6_IRQn
#define macESP8266_USART_INT_FUN USART6_IRQHandler

/*********************************************** ESP8266 函数宏定义 *******************************************/
#define macESP8266_Usart( fmt, ... ) USART_printf( macESP8266_USARTx, fmt, ##__VA_ARGS__ )
#define macPC_Usart( fmt, ... )      printf( fmt, ##__VA_ARGS__ )

// #define macESP8266_CH_ENABLE()  HAL_GPIO_WritePin( macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN, GPIO_PIN_SET )
// #define macESP8266_CH_DISABLE() HAL_GPIO_WritePin( macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN, GPIO_PIN_RESET )

// #define macESP8266_RST_HIGH_LEVEL() HAL_GPIO_WritePin( macESP8266_RST_PORT, macESP8266_RST_PIN, GPIO_PIN_SET )
// #define macESP8266_RST_LOW_LEVEL()  HAL_GPIO_WritePin( macESP8266_RST_PORT, macESP8266_RST_PIN, GPIO_PIN_RESET )

/****************************************** ESP8266 函数声明 ***********************************************/
void    ESP8266_Init( void );
void    ESP8266_Rst( void );
bool    ESP8266_Cmd( char* cmd, char* reply1, char* reply2, uint32_t waittime );
void    ESP8266_AT_Test( void );
bool    ESP8266_Net_Mode_Choose( ENUM_Net_ModeTypeDef enumMode );
bool    ESP8266_JoinAP( char* pSSID, char* pPassWord );
bool    ESP8266_BuildAP( char* pSSID, char* pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
bool    ESP8266_Enable_MultipleId( FunctionalState enumEnUnvarnishTx );
bool    ESP8266_Link_Server( ENUM_NetPro_TypeDef enumE, char* ip, char* ComNum, ENUM_ID_NO_TypeDef id );
bool    ESP8266_StartOrShutServer( FunctionalState enumMode, char* pPortNum, char* pTimeOver );
uint8_t ESP8266_Get_LinkStatus( void );
uint8_t ESP8266_Get_IdLinkStatus( void );
uint8_t ESP8266_Inquire_ApIp( char* pApIp, uint8_t ucArrayLength );
bool    ESP8266_UnvarnishSend( void );
void    ESP8266_ExitUnvarnishSend( void );
bool    ESP8266_SendString( FunctionalState enumEnUnvarnishTx, char* pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId );
char*   ESP8266_ReceiveString( FunctionalState enumEnUnvarnishTx );
bool    ESP8266_AP_DHCP_CUR( void );
bool    ESP8266_STA_DHCP_CUR( void );

uint8_t ESP8266_CWLIF( char* pStaIp );
uint8_t ESP8266_CIPAP( char* pApIp );

#endif
