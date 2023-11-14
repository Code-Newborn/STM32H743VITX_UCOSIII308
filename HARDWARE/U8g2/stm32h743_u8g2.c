#include "stm32h743_u8g2.h"
#include "tim.h"
#include "spi.h"
#include "OLED_SSD1306.h"
#include "delay.h"

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8,
                               uint8_t msg,
                               uint8_t arg_int,
                               void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND: /*通过SPI发送arg_int个字节数据*/
                             //           HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)arg_ptr, arg_int);while(hspi1.TxXferCount);
        /*配置了DMA取消上一行注释即可*/
        HAL_SPI_Transmit(&hspi1, (uint8_t *)arg_ptr, arg_int, 200);
        /*这是CubeMX生成的初始化*/
        break;
    case U8X8_MSG_BYTE_INIT: /*初始化函数*/
        break;
    case U8X8_MSG_BYTE_SET_DC: /*设置DC引脚,表明发送的是数据还是命令*/
        HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, (GPIO_PinState)arg_int);
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);
        u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->post_chip_enable_wait_ns, NULL);
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        u8x8->gpio_and_delay_cb(u8x8, U8X8_MSG_DELAY_NANO, u8x8->display_info->pre_chip_disable_wait_ns, NULL);
        u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
        break;
    default:
        return 0;
    }
    return 1;
}

uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                  U8X8_UNUSED uint8_t msg,
                                  U8X8_UNUSED uint8_t arg_int,
                                  U8X8_UNUSED void *arg_ptr)
{

    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT: /*delay和GPIO的初始化，在main中已经初始化完成了*/
        break;
    case U8X8_MSG_DELAY_MILLI: /*延时函数*/
        HAL_Delay(arg_int);    // 调用谁stm32系统延时函数
        break;
    case U8X8_MSG_GPIO_CS: /*片选信号*/ // 由于只有一个SPI设备，所以片选信号在初始化时已经设置为常有效
        HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, (GPIO_PinState)arg_int);
        break;
    case U8X8_MSG_GPIO_DC: /*设置DC引脚,表明发送的是数据还是命令*/
        HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, (GPIO_PinState)arg_int);
        break;
    case U8X8_MSG_GPIO_RESET:
        break;
    }
    return 1;
}

// U8g2的初始化，需要调用下面这个u8g2_Setup_ssd1306_128x64_noname_f函数，该函数的4个参数含义：
// u8g2：传入的U8g2结构体
// U8G2_R0：默认使用U8G2_R0即可（用于配置屏幕是否要旋转）
// u8x8_byte_sw_i2c：使用软件IIC驱动，该函数由U8g2源码提供
// u8x8_gpio_and_delay：就是上面我们写的配置函数

void u8g2Init(u8g2_t *u8g2)
{
    u8g2_Setup_ssd1306_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay); // 初始化0.96寸OLED u8g2 结构体
    u8g2_InitDisplay(u8g2);                                                                               // 初始化显示
    u8g2_SetPowerSave(u8g2, 0);                                                                           // 开启显示

    // HAL_Delay(1000);
    // u8g2_DrawLine(u8g2, 0, 0, 127, 63);
    // u8g2_DrawLine(u8g2, 127, 0, 0, 63);
    // u8g2_SendBuffer(u8g2);
    // HAL_Delay(1000);
}

void draw(u8g2_t *u8g2)
{
    u8g2_SetFontMode(u8g2, 1);              /*字体模式选择*/
    u8g2_SetFontDirection(u8g2, 0);         /*字体方向选择*/
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); /*字库选择*/
    u8g2_DrawStr(u8g2, 0, 20, "U");

    u8g2_SetFontDirection(u8g2, 1);
    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
    u8g2_DrawStr(u8g2, 21, 8, "8");

    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
    u8g2_DrawStr(u8g2, 51, 30, "g");
    u8g2_DrawStr(u8g2, 67, 30, "\xb2");

    u8g2_DrawHLine(u8g2, 2, 35, 47);
    u8g2_DrawHLine(u8g2, 3, 36, 47);
    u8g2_DrawVLine(u8g2, 45, 32, 12);
    u8g2_DrawVLine(u8g2, 46, 33, 12);

    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
    u8g2_DrawStr(u8g2, 1, 54, "github.com/olikraus/u8g2");
}

// 画点填充
void testDrawPixelToFillScreen(u8g2_t *u8g2)
{
    u8g2_ClearBuffer(u8g2);

    for (int j = 0; j < 64; j++)
    {
        for (int i = 0; i < 128; i++)
        {
            u8g2_DrawPixel(u8g2, i, j);
        }
    }
    HAL_Delay(1000);
}
