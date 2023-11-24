#include "OLED_SSD1306.h"
#include "stdlib.h"
#include "OLED_SSD1306_FONT.h"
#include "delay.h"
#include "spi.h"

uint8_t OLED_GRAM[144][8]; // 图形显示数据

// 反显函数
void OLED_ColorTurn(uint8_t i)
{
	if (i == 0)
	{
		OLED_WR_Byte(0xA6, OLED_CMD); // 正常显示
	}
	if (i == 1)
	{
		OLED_WR_Byte(0xA7, OLED_CMD); // 反色显示
	}
}

// 屏幕旋转180度
void OLED_DisplayTurn(uint8_t i)
{
	if (i == 0)
	{
		OLED_WR_Byte(0xC8, OLED_CMD); // 正常显示
		OLED_WR_Byte(0xA1, OLED_CMD);
	}
	if (i == 1)
	{
		OLED_WR_Byte(0xC0, OLED_CMD); // 反转显示
		OLED_WR_Byte(0xA0, OLED_CMD);
	}
}

void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
	// uint8_t i;
	if (cmd)
		OLED_DC_Set();
	else
		OLED_DC_Clr();
	OLED_CS_Clr();
	// for (i = 0; i < 8; i++)
	// {
	// 	OLED_SCLK_Clr();
	// 	if (dat & 0x80)
	// 		OLED_SDIN_Set();
	// 	else
	// 		OLED_SDIN_Clr();
	// 	OLED_SCLK_Set();
	// 	dat <<= 1;
	// }
	HAL_SPI_Transmit(&hspi1, &dat, 1, 1000);
	OLED_CS_Set();
	OLED_DC_Set();
}

// 开启OLED显示
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD); // 电荷泵使能
	OLED_WR_Byte(0x14, OLED_CMD); // 开启电荷泵
	OLED_WR_Byte(0xAF, OLED_CMD); // 点亮屏幕
}

// 关闭OLED显示
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD); // 电荷泵使能
	OLED_WR_Byte(0x10, OLED_CMD); // 关闭电荷泵
	OLED_WR_Byte(0xAF, OLED_CMD); // 关闭屏幕
}

// 更新显存到OLED
void OLED_Refresh(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置行起始地址
		OLED_WR_Byte(0x00, OLED_CMD);	  // 设置低列起始地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置高列起始地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
	}
}
// 清屏函数
void OLED_Clear(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		for (n = 0; n < 128; n++)
		{
			OLED_GRAM[n][i] = 0; // 清除所有数据
		}
	}
	OLED_Refresh(); // 更新显示
}

// 画点
// x:0~127
// y:0~63
void OLED_DrawPoint(uint8_t x, uint8_t y)
{
	uint8_t i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	OLED_GRAM[x][i] |= n;
}

// 清除一个点
// x:0~127
// y:0~63
void OLED_ClearPoint(uint8_t x, uint8_t y)
{
	uint8_t i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	OLED_GRAM[x][i] |= n;
	OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
}

// 画线
// x:0~127
// y:0~63
// x1,y1：起点坐标
// x2,y2：终点坐标
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	uint8_t i, k, k1, k2;
	if (x1 == x2) // 画竖线
	{
		for (i = 0; i < (y2 - y1); i++)
		{
			OLED_DrawPoint(x1, y1 + i);
		}
	}
	else if (y1 == y2) // 画横线
	{
		for (i = 0; i < (x2 - x1); i++)
		{
			OLED_DrawPoint(x1 + i, y1);
		}
	}
	else // 画斜线
	{
		k1 = y2 - y1;
		k2 = x2 - x1;
		k = k1 * 10 / k2;
		for (i = 0; i < (x2 - x1); i++)
		{
			OLED_DrawPoint(x1 + i, y1 + i * k / 10);
		}
	}
}
// x,y:圆心坐标
// r:圆的半径
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
	int a, b, num;
	a = 0;
	b = r;
	while (2 * b * b >= r * r)
	{
		OLED_DrawPoint(x + a, y - b);
		OLED_DrawPoint(x - a, y - b);
		OLED_DrawPoint(x - a, y + b);
		OLED_DrawPoint(x + a, y + b);

		OLED_DrawPoint(x + b, y + a);
		OLED_DrawPoint(x + b, y - a);
		OLED_DrawPoint(x - b, y - a);
		OLED_DrawPoint(x - b, y + a);

		a++;
		num = (a * a + b * b) - r * r; // 计算画的点离圆心的距离
		if (num > 0)
		{
			b--;
			a--;
		}
	}
}

// 在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~63
// size:选择字体 12/16/24
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1)
{
	uint8_t i, m, temp, size2, chr1;
	uint8_t y0 = y;
	size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); // 得到字体一个字符对应点阵集所占的字节数
	chr1 = chr - ' ';										   // 计算偏移后的值
	for (i = 0; i < size2; i++)
	{
		if (size1 == 12)
		{
			temp = asc2_1206[chr1][i];
		} // 调用1206字体
		else if (size1 == 16)
		{
			temp = asc2_1608[chr1][i];
		} // 调用1608字体
		else if (size1 == 24)
		{
			temp = asc2_2412[chr1][i];
		} // 调用2412字体
		else
			return;
		for (m = 0; m < 8; m++) // 写入数据
		{
			if (temp & 0x80)
				OLED_DrawPoint(x, y);
			else
				OLED_ClearPoint(x, y);
			temp <<= 1;
			y++;
			if ((y - y0) == size1)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

// 显示字符串
// x,y:起点坐标
// size1:字体大小
//*chr:字符串起始地址
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1)
{
	while ((*chr >= ' ') && (*chr <= '~')) // 判断是不是非法字符!
	{
		OLED_ShowChar(x, y, *chr, size1);
		x += size1 / 2;
		if (x > 128 - size1) // 换行
		{
			x = 0;
			y += 2;
		}
		chr++;
	}
}

// m^n
uint32_t OLED_Pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
	{
		result *= m;
	}
	return result;
}

////显示2个数字
////x,y :起点坐标
////len :数字的位数
////size:字体大小
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1)
{
	uint8_t t, temp;
	for (t = 0; t < len; t++)
	{
		temp = (num / OLED_Pow(10, len - t - 1)) % 10;
		if (temp == 0)
		{
			OLED_ShowChar(x + (size1 / 2) * t, y, '0', size1);
		}
		else
		{
			OLED_ShowChar(x + (size1 / 2) * t, y, temp + '0', size1);
		}
	}
}

// 显示汉字
// x,y:起点坐标
// num:汉字对应的序号
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1)
{
	uint8_t i, m, n = 0, temp, chr1;
	uint8_t x0 = x, y0 = y;
	uint8_t size3 = size1 / 8;
	while (size3--)
	{
		chr1 = num * size1 / 8 + n;
		n++;
		for (i = 0; i < size1; i++)
		{
			if (size1 == 16)
			{
				temp = czfont16[chr1][i];
				// temp = Hzk1[chr1][i];
			} // 调用16*16字体
			else if (size1 == 24)
			{
				temp = Hzk2[chr1][i];
			} // 调用24*24字体
			else if (size1 == 32)
			{
				temp = czfont32[chr1][i];
				// temp = Hzk3[chr1][i];
			} // 调用32*32字体
			else if (size1 == 64)
			{
				temp = Hzk4[chr1][i];
			} // 调用64*64字体
			else
				return;

			for (m = 0; m < 8; m++)
			{
				if (temp & 0x01)
					OLED_DrawPoint(x, y);
				else
					OLED_ClearPoint(x, y);
				temp >>= 1;
				y++;
			}
			x++;
			if ((x - x0) == size1)
			{
				x = x0;
				y0 = y0 + 8;
			}
			y = y0;
		}
	}
}

// num 显示汉字的个数
// space 每一遍显示的间隔
void OLED_ScrollDisplay(uint8_t num, uint8_t space)
{
	uint8_t i, n, t = 0, m = 0, r;
	while (1)
	{
		if (m == 0)
		{
			OLED_ShowChinese(128, 24, t, 16); // 写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if (t == num)
		{
			for (r = 0; r < 16 * space; r++) // 显示间隔
			{
				for (i = 0; i < 144; i++)
				{
					for (n = 0; n < 8; n++)
					{
						OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
					}
				}
				OLED_Refresh();
			}
			t = 0;
		}
		m++;
		if (m == 16)
		{
			m = 0;
		}
		for (i = 0; i < 144; i++) // 实现左移
		{
			for (n = 0; n < 8; n++)
			{
				OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

// 配置写入数据的起始位置
void OLED_WR_BP(uint8_t x, uint8_t y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);				  // 设置行起始地址
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD); // 设置低列起始地址
	OLED_WR_Byte((x & 0x0f) | 0x01, OLED_CMD);		  // 设置高列起始地址
}

// x0,y0：起点坐标
// x1,y1：终点坐标
// BMP[]：要写入的图片数组
void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
	uint32_t j = 0;
	uint8_t x = 0, y = 0;
	if (y % 8 == 0)
		y = 0;
	else
		y += 1;
	for (y = y0; y < y1; y++)
	{
		OLED_WR_BP(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_GRAM[x][y] = BMP[j];
			OLED_WR_Byte(BMP[j], OLED_DATA);
			j++;
		}
	}
}
// OLED的初始化
void OLED_Init(void)
{
	// OLED_RST_Set();
	// HAL_Delay(200);
	OLED_RST_Clr(); // 复位
	HAL_Delay(200);
	OLED_RST_Set();

	OLED_WR_Byte(0xAE, OLED_CMD); // 关闭显示
	OLED_WR_Byte(0x00, OLED_CMD); // 为页面寻址模式设置列起始地址 低位
	OLED_WR_Byte(0x10, OLED_CMD); // 为页面寻址模式设置列起始地址 高位
	OLED_WR_Byte(0x40, OLED_CMD); // 设置显示起始行 (0x40~0x7F)

	OLED_WR_Byte(0x81, OLED_CMD); // 对比度设置
	OLED_WR_Byte(0xCF, OLED_CMD); // 对比度值

	OLED_WR_Byte(0xA1, OLED_CMD); // 设置 列与段 重新映射 0xA0左右反置 0xA1正常
	OLED_WR_Byte(0xC8, OLED_CMD); // 设置 COM 输出的扫描方向 0xC0上下反置 0xC8正常

	OLED_WR_Byte(0xA6, OLED_CMD); // 设置 显示方式	0xA6正常显示 0xA7反相显示

	OLED_WR_Byte(0xA8, OLED_CMD); // set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f, OLED_CMD); // 1/64 duty

	OLED_WR_Byte(0xD3, OLED_CMD); // 显示偏移 (0x00~0x3F)
	OLED_WR_Byte(0x00, OLED_CMD); // not offset

	OLED_WR_Byte(0xd5, OLED_CMD); // set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80, OLED_CMD); // set divide ratio, Set Clock as 100 Frames/Sec

	OLED_WR_Byte(0xD9, OLED_CMD); // 设置预充电时间。
	OLED_WR_Byte(0xF1, OLED_CMD); // 预充电 15 时钟周期 & 放电 1 时钟周期

	OLED_WR_Byte(0xDA, OLED_CMD); // 设置 COM 引脚硬件配置
	OLED_WR_Byte(0x12, OLED_CMD);

	OLED_WR_Byte(0xDB, OLED_CMD); // 调整 VCOMH 稳压器输出
	OLED_WR_Byte(0x40, OLED_CMD); // Set VCOM Deselect Level

	OLED_WR_Byte(0x20, OLED_CMD); // 设置内存寻址模式
	OLED_WR_Byte(0x02, OLED_CMD); // 0x02页寻址 0x01列寻址 0x00行寻址

	OLED_WR_Byte(0x8D, OLED_CMD); // set Charge Pump enable/disable
	OLED_WR_Byte(0x14, OLED_CMD); // set(0x10) disable

	OLED_WR_Byte(0xA4, OLED_CMD); // 根据 GDDRAM 内容启用显示输出(0xa4/0xa5)

	OLED_WR_Byte(0xA6, OLED_CMD); // 设置 显示方式	0xA6正常显示 0xA7反相显示

	OLED_WR_Byte(0xAF, OLED_CMD); // 开启显示
	OLED_Clear();
}
