
#include "WIFI/ESP8266.h"
#include "common.h"

// 界面绘制
static display_t draw( void );
// 3按键功能
static bool down( void );
static bool up( void );
static bool select( void );

int frame_cnt = 0;

typedef enum { STATE_STOPPED = 0, STATE_TIMING } weather_state_t;
static weather_state_t state = 1;
static ulong           timer;
static millis_t        lastMS;

void weatherOpen() {
    display_setDrawFunc( draw );
    buttons_setFuncs( up, select, down );

    animation_start( NULL, ANIM_MOVE_ON );
}

static display_t draw() {
    frame_cnt = frame_cnt % Animation1_FrameNum;
    draw_bitmap( 0, 0, ( const byte* )Animation1_Frames[ frame_cnt ], 64, 64, NOINVERT, 0 );  // 绘制区域
    draw_string_P( PSTR( "22:10:16--" ), false, 64, 4 );
    draw_string_P( PSTR( "Harbin" ), false, 64, 16 );
    draw_string_P( PSTR( "Sunuy" ), false, 64, 28 );
    draw_string_P( PSTR( "-23C" ), false, 64, 40 );
    draw_string_P( PSTR( "24-02-20--" ), false, 64, 52 );
    frame_cnt++;
    return DISPLAY_DONE;
}

static bool select( void ) {
    animation_start( back, ANIM_MOVE_OFF );
    return true;
}

uint8_t linkStatus;

void weather_Update( void ) {
    // 处于计时状态
    if ( state == STATE_TIMING ) {
        millis_t now = millis();
        timer        = now - lastMS;

        if ( timer >= 5000 ) {            // 每5s进行api天气查询
            ESP8266_ExitUnvarnishSend();  // 退出透传模式
            linkStatus = ESP8266_Get_LinkStatus();

            switch ( linkStatus ) {
            case 0:
                printf( "[LOG]连接状态0：ESP station 为未初始化状态\r\n" );
                ESP8266_ExitUnvarnishSend();  // 退出透传模式
                ESP8266_Net_Mode_Choose( STA_AP );
                ESP8266_JoinAP( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd );
                ESP8266_Enable_MultipleId( DISABLE );
                ESP8266_Link_Server( enumTCP, "api.seniverse.com", "80", Single_ID_0 );
                break;
            case 1:
                printf( "[LOG]连接状态1：ESP station 为已初始化状态，但还未开始Wi-Fi连接\r\n" );
                ESP8266_JoinAP( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd );
                break;
            case 2:
                printf( "[LOG]连接状态2：ESP station 已连接AP，获得IP地址\r\n" );
                ESP8266_Link_Server( enumTCP, "api.seniverse.com", "80", Single_ID_0 );
                break;
            case 3:
                printf( "[LOG]连接状态3：ESP station 已建立TCP、UDP 或SSL 传输\r\n" );
                Get_Weather();
                break;
            case 4:
                printf( "[LOG]连接状态4：ESP 设备所有的TCP、UDP 和SSL 均断开\r\n" );
                // 重新连接
                printf( "[LOG]尝试重新连接!\r\n" );
                ESP8266_JoinAP( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd );
                ESP8266_Link_Server( enumTCP, "api.seniverse.com", "80", Single_ID_0 );
                break;
            case 5:
                printf( "[LOG]连接状态5：ESP station 开始过Wi-Fi 连接，但尚未连接上AP 或从AP 断开\r\n" );
                // 重新连接
                printf( "[LOG]尝试重新连接!\r\n" );
                ESP8266_JoinAP( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd );
                ESP8266_Link_Server( enumTCP, "api.seniverse.com", "80", Single_ID_0 );
                break;
            default:
                printf( "[LOG]未知状态！！！\r\n" );
                break;
            };

            lastMS = now;
            timer  = 0;
        }
    }
}

static bool down() {}

static bool up() {}