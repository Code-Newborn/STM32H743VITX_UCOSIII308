/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"
#include "led.h"
#include "OLED_SSD1306.h"
// #include "lcd.h"

inline static void setBuffByte(byte *, byte, byte, byte); //, byte);
inline uint8_t pgm_read_byte(const uint8_t *abc) { return *abc; }

void draw_string_P(const char *string, bool invert, byte x, byte y)
{
	byte len = strlen(string);
	char buff[len + 1];
	strcpy(buff, string);
	draw_string(buff, invert, x, y);
}

void draw_string(char *string, bool invert, byte x, byte y)
{
	byte charCount = 0;
	while (*string)
	{
		char c = *string - 0x20;
		byte xx = x + (charCount * 7);
		draw_bitmap(xx, y, smallFont[(byte)c], SMALLFONT_WIDTH, SMALLFONT_HEIGHT, invert, 0);
		if (invert)
		{
			if (xx > 0)
				setBuffByte(oledBuffer, xx - 1, y, 0xFF); //, WHITE);
			if (xx < FRAME_WIDTH - 5)
				setBuffByte(oledBuffer, xx + 5, y, 0xFF); //, WHITE);
		}
		string++;
		charCount++;
	}
}

inline static void setBuffByte(byte *buff, byte x, byte y, byte val) //, byte colour)
{
	uint pos = x + (y / 8) * FRAME_WIDTH;
	buff[pos] |= val;
}

inline static byte readPixels(const byte *loc, bool invert)
{
	// byte pixels = ((int)loc & 0x8000 ? eeprom_read_byte((int)loc & ~0x8000) : pgm_read_byte(loc));
	byte pixels = pgm_read_byte(loc); // d读取flash里面的数据到ram
	if (invert)
		pixels = ~pixels;
	return pixels;
}

/**
 * @brief     : 超快位图绘制
 * @msg       : 唯一的缺点是高度必须是8的倍数，否则将四舍五入到最接近8的倍数，
 * 				画位图，完全在屏幕上，有一个Y坐标是8的倍数在最佳性能
 * @param      {byte} x 绘制位置 横坐标
 * @param      {byte} yy
 * @param      {byte} *bitmap 绘制图片
 * @param      {byte} w 绘制宽度
 * @param      {byte} h 绘制高度
 * @param      {bool} invert
 * @param      {byte} offsetY
 * @return     {*}
 */
void draw_bitmap(byte x, byte yy, const byte *bitmap, byte w, byte h, bool invert, byte offsetY)
{
	yy += animation_offsetY();

	byte y = yy - offsetY;

	byte h2 = h / 8;

	byte pixelOffset = (y % 8);

	byte thing3 = (yy + h);

	LOOP(h2, hh)
	{
		//
		byte hhh = (hh * 8) + y; // Current Y pos (every 8 pixels)
		byte hhhh = hhh + 8;	 // Y pos at end of pixel column (8 pixels)

		if (offsetY && (hhhh < yy || hhhh > FRAME_HEIGHT || hhh > thing3))
			continue;

		byte offsetMask = 0xFF; // 移动遮罩
		if (offsetY)
		{
			if (hhh < yy)
				offsetMask = (0xFF << (yy - hhh));
			else if (hhhh > thing3)
				offsetMask = (0xFF >> (hhhh - thing3));
		}

		uint aa = ((hhh / 8) * FRAME_WIDTH);

		const byte *b = bitmap + (hh * w);

		// If() outside of loop makes it faster (doesn't have to keep re-evaluating it)
		// Downside is code duplication
		if (!pixelOffset && hhh < FRAME_HEIGHT)
		{
			//
			LOOP(w, ww)
			{
				// Workout X co-ordinate in frame buffer to place next 8 pixels
				byte xx = ww + x;

				// Stop if X co-ordinate is outside the frame
				if (xx >= FRAME_WIDTH)
					continue;

				// Read pixels
				byte pixels = readPixels(b + ww, invert) & offsetMask;

				oledBuffer[xx + aa] |= pixels;

				// setBuffByte(buff, xx, hhh, pixels, colour);
			}
		}
		else
		{
			uint aaa = ((hhhh / 8) * FRAME_WIDTH);

			//
			LOOP(w, ww)
			{
				// Workout X co-ordinate in frame buffer to place next 8 pixels
				byte xx = ww + x;

				// Stop if X co-ordinate is outside the frame
				if (xx >= FRAME_WIDTH)
					continue;

				// Read pixels
				byte pixels = readPixels(b + ww, invert) & offsetMask;

				//
				if (hhh < FRAME_HEIGHT)
					oledBuffer[xx + aa] |= pixels << pixelOffset;
				// setBuffByte(buff, xx, hhh, pixels << pixelOffset, colour);

				//
				if (hhhh < FRAME_HEIGHT)
					oledBuffer[xx + aaa] |= pixels >> (8 - pixelOffset);
				// setBuffByte(buff, xx, hhhh, pixels >> (8 - pixelOffset), colour);
			}
		}
	}
}

// 纵坐标必须是8的倍数
// 高始终是8
void draw_clearArea(byte x, byte y, byte w)
{
	uint pos = x + (y / 8) * FRAME_WIDTH;
	memset(&oledBuffer[pos], 0x00, w);
}

void draw_end()
{
	LCD_Flush(); // 刷新屏幕的意思
}
