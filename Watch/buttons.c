/*
   Project: N|Watch
   Author: Zak Kemble, contact@zakkemble.co.uk
   Copyright: (C) 2013 by Zak Kemble
   License: GNU GPL v3 (see License.txt)
   Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
*/
#include "buttons.h"

#include "common.h"
#include "led.h"
#include "sys.h"
// #include "yaogan.h"

#define BTN_IS_PRESSED  4
#define BTN_NOT_PRESSED 4

typedef struct {
    millis_t pressedTime;   // Time of press
    bool     processed;     // Time of press has been stored (don't store again until
                            // next press)
    byte         counter;   // Debounce counter
    bool         funcDone;  // Function has been ran (don't run again until next press)
    button_f     onPress;   // Function to run when pressed
    const ulong* tune;      // Tune to play when pressed
} s_button;

static s_button buttons[ BTN_COUNT ];

static void processButtons( void );
static void processButton( s_button*, BOOL );
static byte bitCount( byte );

// EMPTY_INTERRUPT(PCINT0_vect);

void buttons_init() {
    // buttons_startup(); // 按键引脚配置初始化

    // 蜂鸣器的播放曲调
    buttons[ BTN_1 ].tune = tuneBtn1;
    buttons[ BTN_2 ].tune = tuneBtn2;
    buttons[ BTN_3 ].tune = tuneBtn3;

    // Set up interrupts
    // #ifdef __AVR_ATmega32U4__
    //   SET_BITS(PCMSK0, PCINT4, PCINT6, PCINT7);
    // #else
    //   SET_BITS(PCMSK1, PCINT9, PCINT10, PCINT11);
    // #endif
    //   BTN_INT_ON();
}

void buttons_update() {
    static millis8_t lastUpdate;

    // 按键每10ms检测一次
    millis8_t now = millis();
    if ( ( millis8_t )( now - lastUpdate ) >= 10 ) {
        lastUpdate = now;
        processButtons();  // 处理按键
    }
}

// 按键初始化函数
//  Sets button pins to INPUT with PULLUP
void buttons_startup() {
    //	GPIO_InitTypeDef  GPIO_InitStructure;
    //  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,
    //  ENABLE);//使能GPIOA,GPIOE时钟
    //
    //  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //KEY0
    //  KEY1 KEY2对应引脚 GPIO_InitStructure.GPIO_Mode =
    //  GPIO_Mode_IN;//普通输入模式 GPIO_InitStructure.GPIO_Speed =
    //  GPIO_Speed_100MHz;//100M GPIO_InitStructure.GPIO_PuPd =
    //  GPIO_PuPd_UP;//上拉 GPIO_Init(GPIOE,
    //  &GPIO_InitStructure);//初始化GPIOE2,3,4
    //
    //
    //  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP对应引脚PA0
    //  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
    //  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0

    // GPIO_InitTypeDef GPIO_InitStructure;

    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA |
    // RCC_APB2Periph_GPIOC, ENABLE); // 使能PORTA,PORTC时钟

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;     // PA4
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 设置成上拉输入
    // GPIO_Init(GPIOA, &GPIO_InitStructure);        // 初始化GPIOA15

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    // PC5
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 设置成上拉输入
    // GPIO_Init(GPIOB, &GPIO_InitStructure);        // 初始化GPIOC5

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;    // PA0
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // PA0设置成输入，默认下拉
    // GPIO_Init(GPIOC, &GPIO_InitStructure);        // 初始化GPIOA.0
}

// Sets button pins to OUTPUT LOW
// This stops them wasting current through the pull-up resistor when pressed
void buttons_shutdown() {
    // 配合休眠sleep函数开启后关闭按键，不会做

    //  pinPullup(BTN_1_P,	PU_DIS);
    //  pinPullup(BTN_2_P,	PU_DIS);
    //  pinPullup(BTN_3_P,	PU_DIS);

    //  pinMode(BTN_1_P,	OUTPUT);
    //  pinMode(BTN_2_P,	OUTPUT);
    //  pinMode(BTN_3_P,	OUTPUT);
}

uint8_t     keypressed;
static void processButtons() {
    // 获取按键状态
    BOOL isPressed[ BTN_COUNT ];
    isPressed[ BTN_1 ] = KEY0;
    isPressed[ BTN_2 ] = KEY1;
    isPressed[ BTN_3 ] = KEY2;

    // 依次处理每个按键
    LOOPR( BTN_COUNT, i )
    processButton( &buttons[ i ], !isPressed[ i ] );
}

static void processButton( s_button* button, BOOL isPressed ) {
    button->counter <<= 1;  // 左移位补零
    if ( isPressed ) {
        button->counter |= 1;  // 按键按下则置1

        // 8bit中有足够多的1，相当于按键消抖，稳定按下
        if ( bitCount( button->counter ) >= BTN_IS_PRESSED ) {
            if ( !button->processed ) {
                button->pressedTime = millis();  // 记录按下的时刻，为息屏提供参考
                button->processed   = true;
            }

            // 按键处理回调函数button->onPress()
            if ( !button->funcDone && button->onPress != NULL && button->onPress() ) {
                button->funcDone = true;
                // tune_play(button->tune, VOL_UI, PRIO_UI);

                // led_flash(LED_GREEN, LED_FLASH_FAST, LED_BRIGHTNESS_MAX);
                // LED1=!LED1;
            }
        }
    } else {
        // 未达到按下的稳定状态
        if ( bitCount( button->counter ) <= BTN_NOT_PRESSED ) {
            button->processed = false;
            button->funcDone  = false;
        }
    }
}

// 8bit中1的个数
static byte bitCount( byte val ) {
    byte count = 0;
    for ( ; val; val >>= 1 )
        count += val & 1;
    return count;
}

// 定义一个返回函数指针的函数
//  Set new function to run when button is pressed and return the old function
button_f buttons_setFunc( btn_t btn, button_f func ) {
    button_f old           = buttons[ btn ].onPress;
    buttons[ btn ].onPress = func;
    return old;
}

// 设置每个按键的功能
void buttons_setFuncs( button_f btn1, button_f btn2, button_f btn3 ) {
    buttons[ BTN_1 ].onPress = btn1;
    buttons[ BTN_2 ].onPress = btn2;
    buttons[ BTN_3 ].onPress = btn3;
}

/*
  // Get how long a button has been pressed for
  millis_t buttons_pressTime(btn_t btn) // set max press time to 1 min!!!
  {
  s_button* button = &buttons[btn];
  if(button->pressed == BTN_NOT_PRESSED)
    return 0;
  return (millis() - button->pressedTime);
  }
*/

// See if a button has been pressed in the past x milliseconds
// 检测按键长时间未按下
bool buttons_isActive() {
    // If sleep has been disabled then just say that the buttons are always
    // active
    if ( !appConfig.sleepTimeout )
        return true;

    // Get timeout val in ms
    uint timeout = ( appConfig.sleepTimeout * 5 ) * 1000;
    //  uint timeout =  1000;

    // See if a button has been pressed within that timeout
    LOOPR( BTN_COUNT, i ) {
        if ( millis() - buttons[ i ].pressedTime < timeout )
            return true;
    }

    return false;  // 长时间未按下
}

// 设置按键被按下，但不运行函数
void buttons_wake() {
    LOOPR( BTN_COUNT, i ) {
        buttons[ i ].funcDone    = true;  // 按键按下
        buttons[ i ].processed   = true;  // 按键处理
        buttons[ i ].counter     = BTN_IS_PRESSED;
        buttons[ i ].pressedTime = millis();
    }
}
