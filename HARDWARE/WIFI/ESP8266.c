#include "WIFI/ESP8266.h"
#include "cJSON.h"
#include "delay.h"

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

/*
 * 函数名：ESP8266_Rst
 * 描述  ：重启WF-ESP8266模块
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被 ESP8266_AT_Test 调用
 */
void ESP8266_Rst( void ) {
#if 1
    ESP8266_Cmd( "AT+RST", "OK", "ready", 1000 );
#else
    macESP8266_RST_LOW_LEVEL();  // 硬件拉低复位
    delay_ms( 500 );
    macESP8266_RST_HIGH_LEVEL();
#endif
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

    if ( ( reply1 == 0 ) && ( reply2 == 0 ) )  // 不需要验证接收数据直接返回
        return true;

    delay_ms( waittime );  // 延时

    strEsp8266_Fram_Record.Data_RX_BUF[ strEsp8266_Fram_Record.InfBit.FramLength ] = '\0';

    macPC_Usart( "%s", strEsp8266_Fram_Record.Data_RX_BUF );  // 中断接收数据，通过USART1发送至PC

    if ( ( reply1 != 0 ) && ( reply2 != 0 ) )  // 验证接收数据中是否包含 reply1 和 reply2
        return ( ( bool )strstr( strEsp8266_Fram_Record.Data_RX_BUF, reply1 ) || ( bool )strstr( strEsp8266_Fram_Record.Data_RX_BUF, reply2 ) );
    else if ( reply1 != 0 )
        return ( ( bool )strstr( strEsp8266_Fram_Record.Data_RX_BUF, reply1 ) );
    else
        return ( ( bool )strstr( strEsp8266_Fram_Record.Data_RX_BUF, reply2 ) );
}

// AT指令测试
void ESP8266_AT_Test( void ) {
    char count = 0;
    // delay_ms( 1000 );
    while ( count < 10 ) {
        if ( ESP8266_Cmd( "AT", "OK", NULL, 500 ) )  // 测试 AT 启动
            return;
        ESP8266_Rst();  // 复位
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
        return ESP8266_Cmd( "AT+CWMODE=1", "OK", "no change", 1000 );

    case AP:
        return ESP8266_Cmd( "AT+CWMODE=2", "OK", "no change", 1000 );

    case STA_AP:
        return ESP8266_Cmd( "AT+CWMODE=3", "OK", "no change", 1000 );

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

    return ESP8266_Cmd( cCmd, "OK", NULL, 1000 );
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
        if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:0\r\n" ) )
            return 0;
        else if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:1\r\n" ) )
            return 1;
        else if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:2\r\n" ) )
            return 2;
        else if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:3\r\n" ) )
            return 3;
        else if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:4\r\n" ) )
            return 4;
        else if ( strstr( strEsp8266_Fram_Record.Data_RX_BUF, "STATUS:5\r\n" ) )
            return 5;
    }
    return 0;
}

bool ESP8266_ConnectWiFi( void ) {
    char cStr[ 100 ] = { 0 };

    printf( "[LOG]正在配置 ESP8266 ......\r\n" );

    ESP8266_ExitUnvarnishSend();  // 退出透传模式

    ESP8266_Rst();                      // 复位
    ESP8266_AT_Test();                  // AT测试
    ESP8266_Net_Mode_Choose( STA_AP );  // 模式设置

    while ( !ESP8266_JoinAP( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) )  // 尝试连接目标AP（WiFi）
        ;

    ESP8266_Inquire_ApIp( cStr, 20 );  // 查询ESP8266的IP

    printf( "[LOG]成功连接Wifi\r\n" );

    ESP8266_Enable_MultipleId( DISABLE );  // 作为客户端-单连接

    // 建立TCP连接，参数为 连接类型 远端IP 远端端口号
    while ( !ESP8266_Link_Server( enumTCP, "api.seniverse.com", "80", Single_ID_0 ) )
        ;

    printf( "[LOG]访问网站API\r\n" );

    if ( !ESP8266_UnvarnishSend() )  // 设置透传
    {
        printf( "[LOG]未成功设置透传\r\n" );
        return false;
    }

    return true;
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
    // json 解析 https://zhuanlan.zhihu.com/p/54574542
    return pRecStr;
}

bool Get_Weather( void ) {
    ESP8266_UnvarnishSend();  // 进入透传模式

    // 私钥才能访问
    // 中文 https://api.seniverse.com/v3/weather/now.json?key=SNnoJymqbUTJCXYiX&location=beijing&language=zh-Hans&unit=c
    // 英文 GET https://api.seniverse.com/v3/weather/now.json?key=SNnoJymqbUTJCXYiX&location=zhaoqing&language=en&unit=c
    // 包含最后2字节转义符号共112+2=114字节
    char*   request_url = "GET https://api.seniverse.com/v3/weather/now.json?key=SNnoJymqbUTJCXYiX&location=haerbin&language=zh-Hans&unit=c\r\n";
    uint8_t str_len     = strlen( request_url );
    ESP8266_SendString( ENABLE, request_url, str_len, Single_ID_0 );

    char* weather_str = ESP8266_ReceiveString( ENABLE );
    printf( weather_str );  // 打印API返回数据
    printf( "\r\n\n" );
    printf( "[LOG]开始解析数据 \r\n" );

    // cJson解析库使用 https://zhuanlan.zhihu.com/p/54574542

    cJSON* root;
    cJSON* results;
    cJSON* last_update;
    cJSON *loc_json, *now_json;
    char * loc_tmp, *weather_tmp, *update_tmp;

    root = cJSON_Parse( ( const char* )weather_str );
    if ( root ) {
        // printf( "JSON格式正确:\n%s\n\n", cJSON_Print( root ) );  // 输出json字符串
        results = cJSON_GetObjectItem( root, "results" );
        results = cJSON_GetArrayItem( results, 0 );
        if ( results ) {
            loc_json = cJSON_GetObjectItem( results, "location" );  // 得到location键对应的值，是一个对象
            if ( loc_json ) {
                loc_tmp = cJSON_GetObjectItem( loc_json, "id" )->valuestring;
                printf( "城市ID:%s\r\n", loc_tmp );
                loc_tmp = cJSON_GetObjectItem( loc_json, "name" )->valuestring;
                printf( "城市名称:%s\r\n", loc_tmp );
                loc_tmp = cJSON_GetObjectItem( loc_json, "timezone" )->valuestring;
                printf( "城市时区:%s\r\n", loc_tmp );
            } else
                printf( "daily json格式错误\r\n" );

            now_json = cJSON_GetObjectItem( results, "now" );
            if ( now_json ) {
                weather_tmp = cJSON_GetObjectItem( now_json, "text" )->valuestring;
                printf( "天气:%s\r\n", weather_tmp );
                weather_tmp = cJSON_GetObjectItem( now_json, "code" )->valuestring;
                printf( "天气代码:%s\r\n", weather_tmp );
                weather_tmp = cJSON_GetObjectItem( now_json, "temperature" )->valuestring;
                printf( "温度:%s\r\n", weather_tmp );
            } else
                printf( "daily json格式错误\r\n" );

            last_update = cJSON_GetObjectItem( results, "last_update" );
            update_tmp  = last_update->valuestring;
            if ( last_update ) {
                printf( "更新时间:%s\r\n", update_tmp );
            }
        } else {
            printf( "[LOG]results格式错误:%s\r\n", cJSON_GetErrorPtr() );
            return false;
        }
    } else {
        printf( "[LOG]JSON格式错误\r\n" );
        return false;
    }
    cJSON_Delete( root );

    ESP8266_ExitUnvarnishSend();  // 退出透传模式
    return true;
}
