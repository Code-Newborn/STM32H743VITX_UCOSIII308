/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "stdio.h"
#include "string.h"
#include "common.h"

typedef enum {
    OPERATION_DRAWICON,       // 画图标操作
    OPERATION_DRAWNAME_ICON,  // 画图标操作
    OPERATION_DRAWNAME_STR,   // 画文字操作
    OPERATION_ACTION          // 执行操作
} operation_t;

typedef struct {
    byte        data;
    operation_t op;
    byte        id;
} operation_s;

s_menuNowSetting   setting;
menu_s             menuData;
static operation_s operation;
const char         menuBack[] = STR_BACK;

static void      doBtn( menu_f );
static void      drawTitle( void );
static void      loader( operation_t, byte, byte );
static void      menu_drawStr( void );
static display_t menu_drawIcon( void );
static void      checkScroll( void );
static void      clear( void );

// 主菜单选择界面
bool menu_select() {
    if ( !animation_active() || animation_movingOn() ) {
        // 第一次进入主菜单函数，执行mMainOpen();打开主菜单
        if ( !menuData.isOpen ) {
            menuData.isOpen = true;
            mMainOpen();
        }
        else if ( menuData.func.btn2 != NULL )  // 打开后再次按下确认功能
        {
            menuData.func.btn2();
        }
    }
    return true;
}

bool menu_down() {
    doBtn( menuData.func.btn3 );
    return true;
}

bool menu_up() {
    doBtn( menuData.func.btn1 );
    return true;
}

static void doBtn( menu_f btn ) {
    if ( menuData.isOpen && ( !animation_active() || animation_movingOn() ) && btn != NULL )
        btn();
}

display_t menu_draw() {
    display_t busy = DISPLAY_DONE;
    if ( menuData.menuType == MENU_TYPE_STR )
        menu_drawStr();  // 绘制菜单界面文字
    else
        busy = menu_drawIcon();

    if ( menuData.func.draw != NULL )
        busy = busy || menuData.func.draw() ? DISPLAY_BUSY : DISPLAY_DONE;

    return busy;
}

static void drawTitle() {
    char buff[ BUFFSIZE_STR_MENU ];
    memset( buff, ' ', sizeof( buff ) );
    strcpy( ( buff + ( 9 - ( strlen( menuData.title ) / 2 ) ) ), menuData.title );
    draw_string( buff, false, 0, 0 );
}

static void loader( operation_t op, byte num, byte data ) {
    operation.op   = op;    // op是option，选项类型
    operation.id   = num;   // 选择菜单选项的位置索引值
    operation.data = data;  // anim，是否动画

    if ( menuData.func.loader != NULL )
        menuData.func.loader( num );  // 指向itemLoader函数
}

static void menu_drawStr() {
    drawTitle();

    byte scroll = menuData.scroll;
    byte count  = ( ( MAX_MENU_ITEMS < menuData.optionCount ) ? MAX_MENU_ITEMS : menuData.optionCount ) + scroll;
    for ( byte i = scroll; i < count; i++ )  // 从滚动的行数scroll到count遍历8行要显示的字符串
    {
        byte y = 8 + ( 8 * ( i - scroll ) );
        if ( i == menuData.selected )
            draw_string( ">", false, 0, y );     // 当前选择光标
        loader( OPERATION_DRAWNAME_STR, i, y );  // 加载其他选项名称
    }
}

/**
 * @brief     : 绘制图标
 * @msg       :
 * @return     {*} 绘制忙标志
 */
static display_t menu_drawIcon() {
    static int animX = 64;

    int x = 64 - ( 48 * menuData.selected );

    display_t busy = DISPLAY_DONE;

#if COMPILE_ANIMATIONS
    if ( appConfig.animations ) {
        byte speed;
        if ( x > animX ) {
            speed = ( ( x - animX ) / 4 ) + 1;
            if ( speed > 16 )
                speed = 16;
            animX += speed;
            if ( x <= animX )
                animX = x;
            else
                busy = DISPLAY_BUSY;
        }
        else if ( x < animX ) {
            speed = ( ( animX - x ) / 4 ) + 1;
            if ( speed > 16 )
                speed = 16;
            animX -= speed;
            if ( x >= animX )
                animX = x;
            else
                busy = DISPLAY_BUSY;
        }
    }
    else
#endif
        animX = x;

    x = animX - 16;

    drawTitle();

    draw_bitmap( 46, 14, selectbar_top, 36, 8, NOINVERT, 0 );     // 选中框上部分
    draw_bitmap( 46, 42, selectbar_bottom, 36, 8, NOINVERT, 0 );  // 选中框下部分

    LOOP( menuData.optionCount, i )  // 遍历所有图标
    {
        if ( x < FRAME_WIDTH && x > -32 )  // 满足在屏幕范围内打印图标
            loader( OPERATION_DRAWICON, i, x );
        x += 48;
    }

    loader( OPERATION_DRAWNAME_ICON, menuData.selected, 0 );

    return busy;
}

void setMenuOption_P( byte num, const char* name, const byte* icon, menu_f actionFunc ) {
    if ( num != operation.id )
        return;

    char buff[ BUFFSIZE_STR_MENU ];
    strcpy( buff, name );
    setMenuOption( num, buff, icon, actionFunc );  // 设置菜单内选项信息
}

#include <math.h>
void setMenuOption( byte num, const char* name, const byte* icon, menu_f actionFunc ) {
    if ( num != operation.id )
        return;

    switch ( operation.op ) {
    case OPERATION_DRAWICON:
    {
        byte  a = operation.data;
        // if(a > FRAME_WIDTH)
        //	a -= (FRAME_WIDTH+32);
        float x = ( ( a / ( float )( FRAME_WIDTH - 32 ) ) * ( M_PI / 2 ) ) + ( M_PI / 4 );
        byte  y = ( sin( x ) * 32 );

        y = 28;  // 注释此处，启用选项图标弧形切换特效

        draw_bitmap( operation.data, y + 4 - 16, icon != NULL ? icon : menu_default, 32, 32, NOINVERT, 0 );
    } break;
    case OPERATION_DRAWNAME_ICON:
        draw_string( ( char* )name, false, 0, FRAME_HEIGHT - 8 );
        break;
    case OPERATION_DRAWNAME_STR:
        draw_string( ( char* )name, false, 6, operation.data );
        break;
    case OPERATION_ACTION:
        if ( actionFunc != NULL )
            operation.data ? animation_start( actionFunc, ANIM_MOVE_OFF ) : actionFunc();  // 是否采用动画方式
        break;
    default:
        break;
    }
}

bool menu_isOpen() {
    return menuData.isOpen;
}

void menu_close() {
    memset( &menuData.func, 0, sizeof( menuFuncs_t ) );
    menuData.isOpen   = false;
    menuData.prevMenu = NULL;
    display_load();  // Move somewhere else, sometimes we don't want to load the watch face when closing the menu
}
// 保存上一次菜单的打开功能函数
void setPrevMenuOpen( prev_menu_s* prevMenu, menu_f newPrevMenu ) {
    if ( menuData.prevMenu != newPrevMenu )      // 确保新旧菜单功能不一样，否则会陷入菜单循环
        prevMenu->last = menuData.prevMenu;      // 保存之前的打开功能函数
    menuData.selected = prevMenu->lastSelected;  //
    menuData.prevMenu = newPrevMenu;             // 设置新的菜单功能
}
// 保存上一次菜单的选项
void setPrevMenuExit( prev_menu_s* prevMenu ) {
    if ( !exitSelected() )  // 非最后选项（退出）选项
        prevMenu->lastSelected = menuData.selected;
    else {
        prevMenu->lastSelected = 0;               // Reset selected item
        menuData.prevMenu      = prevMenu->last;  //
    }
}

bool exitSelected() {
    return menuData.selected == menuData.optionCount - 1;
}
/*
void do10sStuff(byte* val, byte now)
{
        byte mod = mod10(*val);
        *val = (setting.val * 10) + mod;

        setting.val = mod;
        setting.now = now;
}

void do1sStuff(byte* val, byte max, byte now, byte newVal)
{
    if(val != NULL)
    {
        byte temp = *val;
        temp = ((temp / 10) * 10) + setting.val;
        if(temp > max)
            temp = max;
        *val = temp;
    }

    setting.val = newVal;
    setting.now = now;
}
*/
static void clear() {
    memset( &menuData.func, 0, sizeof( menuFuncs_t ) );
}

void addBackOption() {
    // 设置退出当前菜单的选项
    // 索引、选项名、选项图标、
    setMenuOption_P( menuData.optionCount - 1, menuBack, menu_exit, back );
}

// 返回上级菜单，上级只有打开动画过度
void back() {
    menuData.prevMenu != NULL ? menuData.prevMenu() : mMainOpen();
}

/**
 * @brief     : 设置菜单界面信息
 * @msg       : 菜单管理
 * @param      {byte} optionCount 选项数
 * @param      {menu_type_t} menuType 选项的显示方式 图标 or 文字
 * @param      {char} *title 当前界面标题
 * @return     {*}
 */
void setMenuInfo( byte optionCount, menu_type_t menuType, const char* title ) {
    clear();  // 清除相应按键函数
    menuData.scroll      = 0;
    menuData.selected    = 0;
    menuData.optionCount = optionCount + 1;
    menuData.menuType    = menuType;
    menuData.title       = title;
}

// 设置菜单功能函数
void setMenuFuncs( menu_f btn1Func, menu_f btn2Func, menu_f btn3Func, itemLoader_f loader ) {
    menuData.func.btn1   = btn1Func;
    menuData.func.btn2   = btn2Func;
    menuData.func.btn3   = btn3Func;
    menuData.func.loader = loader;
}

// 下一菜单项
void nextOption() {
    menuData.selected++;
    if ( menuData.selected >= menuData.optionCount )
        menuData.selected = 0;

    checkScroll();
}

// 上一菜单项
void prevOption() {
    menuData.selected--;
    if ( menuData.selected >= menuData.optionCount )
        menuData.selected = menuData.optionCount - 1;

    checkScroll();
}

void doAction( bool anim ) {
    loader( OPERATION_ACTION, menuData.selected, anim );
}

// 检查设置滚动的行数scroll
static void checkScroll() {
    byte scroll = menuData.scroll;
    if ( menuData.selected >= scroll + MAX_MENU_ITEMS )
        scroll = ( menuData.selected + 1 ) - MAX_MENU_ITEMS;
    else if ( menuData.selected < scroll )
        scroll = menuData.selected;
    menuData.scroll = scroll;
}
