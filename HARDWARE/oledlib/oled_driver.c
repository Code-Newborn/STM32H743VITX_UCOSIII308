#include "oled_driver.h"
#include "sys.h"
#include "delay.h"
#include "spi.h"
#if (TRANSFER_METHOD == HW_IIC)
// I2C_Configuration，初始化硬件IIC引脚
void I2C_Configuration(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(IIC_RCC_APB1Periph_I2CX, ENABLE);
	RCC_APB2PeriphClockCmd(IIC_RCC_APB2Periph_GPIOX, ENABLE);

	GPIO_InitStructure.GPIO_Pin = IIC_SCL_Pin_X | IIC_SDA_Pin_X;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // I2C必须开漏输出
	GPIO_Init(IIC_GPIOX, &GPIO_InitStructure);

	I2C_DeInit(I2CX);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30; // 主机的I2C地址,随便写
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 1320000;

	I2C_Cmd(I2CX, ENABLE);
	I2C_Init(I2CX, &I2C_InitStructure);
	WaitTimeMs(200);
}

/**
 * @brief  I2C_WriteByte，向OLED寄存器地址写一个byte的数据
 * @param  addr：寄存器地址
 *					data：要写入的数据
 * @retval 无
 */
void I2C_WriteByte(uint8_t addr, uint8_t data)
{
	while (I2C_GetFlagStatus(I2CX, I2C_FLAG_BUSY))
		;

	I2C_GenerateSTART(I2CX, ENABLE); // 开启I2C1
	while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_MODE_SELECT))
		; /*EV5,主模式*/

	I2C_Send7bitAddress(I2CX, OLED_ADDRESS, I2C_Direction_Transmitter); // 器件地址 -- 默认0x78
	while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;

	I2C_SendData(I2CX, addr); // 寄存器地址
	while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	I2C_SendData(I2CX, data); // 发送数据
	while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	I2C_GenerateSTOP(I2CX, ENABLE); // 关闭I2C1总线
}

void WriteCmd(unsigned char cmd) // 写命令
{
	I2C_WriteByte(0x00, cmd);
}

void WriteDat(unsigned char dat) // 写数据
{
	I2C_WriteByte(0x40, dat);
}

void OLED_FILL(unsigned char BMP[])
{
	unsigned int n;
	unsigned char *p;
	p = BMP;
	WriteCmd(0xb0); // page0-page1
	WriteCmd(0x00); // low column start address
	WriteCmd(0x10); // high column start address
	while (I2C_GetFlagStatus(I2CX, I2C_FLAG_BUSY))
		;
	I2C_GenerateSTART(I2C1, ENABLE); // 开启I2C1
	while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_MODE_SELECT))
		;																/*EV5,主模式*/
	I2C_Send7bitAddress(I2CX, OLED_ADDRESS, I2C_Direction_Transmitter); // 器件地址 -- 默认0x78
	while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;
	I2C_SendData(I2CX, 0x40); // 寄存器地址
	while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;
	for (n = 0; n < 128 * 8; n++)
	{
		I2C_SendData(I2CX, *p++); // 发送数据
		while (!I2C_CheckEvent(I2CX, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
			;
	}
	I2C_GenerateSTOP(I2CX, ENABLE); // 关闭I2C1总线
}

#elif (TRANSFER_METHOD == SW_IIC)

#elif (TRANSFER_METHOD == HW_SPI)

#define OLED_RESET_LOW() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET) // 低电平复位
#define OLED_RESET_HIGH() HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET)

#define OLED_CMD_MODE() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET) // 命令模式
#define OLED_DATA_MODE() HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)  // 数据模式

#define OLED_CS_HIGH() HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET)
#define OLED_CS_LOW() HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET)

// void SPI_Configuration(void)
// {
// 	SPI_InitTypeDef SPI_InitStructure;
// 	GPIO_InitTypeDef GPIO_InitStructure;
// #if (USE_HW_SPI == SPI_2)
// 	RCC_APB1PeriphClockCmd(SPI_RCC_APB1Periph_SPIX, ENABLE);
// #elif (USE_HW_SPI == SPI_1)
// 	RCC_APB2PeriphClockCmd(SPI_RCC_APB2Periph_SPIX, ENABLE);
// #endif
// 	RCC_APB2PeriphClockCmd(SPI_RCC_APB2Periph_GPIOX | RCC_APB2Periph_AFIO, ENABLE);

// 	GPIO_InitStructure.GPIO_Pin = SPI_CS_Pin_X;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_Init(SPI_CS_GPIOX, &GPIO_InitStructure);
// 	OLED_CS_HIGH();

// 	GPIO_InitStructure.GPIO_Pin = SPI_HW_ALL_PINS;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
// 	GPIO_Init(SPI_HW_ALL_GPIOX, &GPIO_InitStructure);

// 	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // SPI_Direction_1Line_Tx;
// 	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
// 	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
// 	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
// 	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
// 	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
// 	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
// 	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
// 	SPI_InitStructure.SPI_CRCPolynomial = 7;

// 	SPI_Init(SPIX, &SPI_InitStructure);
// 	SPI_Cmd(SPIX, ENABLE);

// 	GPIO_InitStructure.GPIO_Pin = SPI_RES_PIN;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_Init(SPI_RES_GPIOX, &GPIO_InitStructure);
// 	GPIO_InitStructure.GPIO_Pin = SPI_DC_PIN;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_Init(SPI_DC_GPIOX, &GPIO_InitStructure);
// 	OLED_DATA_MODE();

// 	OLED_RESET_HIGH();
// 	WaitTimeMs(100);
// 	OLED_RESET_LOW();
// 	WaitTimeMs(100);
// 	OLED_RESET_HIGH();
// }

void SPI_WriterByte(unsigned char dat)
{

	// while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET)
	// {
	// };										 // 检查指定的SPI标志位设置与否:发送缓存空标志位
	HAL_SPI_Transmit(&hspi1, &dat, 1, 1000); // 通过外设SPIx发送一个数据

	// while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET)
	// {
	// };									   // 检查指定的SPI标志位设置与否:接受缓存非空标志位
	// HAL_SPI_Receive(&hspi1, dat, 1, 1000); // 返回通过SPIx最近接收的数据
}

void WriteCmd(unsigned char cmd)
{
	OLED_CMD_MODE();
	// OLED_CS_LOW();
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 200); // 通过外设SPIx发送一个数据
	// OLED_CS_HIGH();
	OLED_DATA_MODE();
}

void WriteDat(unsigned char dat)
{
	OLED_DATA_MODE();
	// OLED_CS_LOW();
	HAL_SPI_Transmit(&hspi1, &dat, 1, 200); // 通过外设SPIx发送一个数据
	// OLED_CS_HIGH();
	OLED_DATA_MODE();
}

void OLED_FILL(unsigned char BMP[])
{
	uint8_t i, j;
	unsigned char *p;
	p = BMP;

	for (i = 0; i < 8; i++)
	{
		WriteCmd(0xb0 + i); // page0-page1
		WriteCmd(0x00);		// low column start address
		WriteCmd(0x10);

		for (j = 0; j < 128; j++)
		{
			WriteDat(*p++);
		}
	}
}

#elif (TRANSFER_METHOD == SW_SPI)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA, GPIO_Pin_6) // D0
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA, GPIO_Pin_6)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA, GPIO_Pin_7) // DIN  D1
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA, GPIO_Pin_7)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_0) // RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB, GPIO_Pin_0)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_1) // DC
#define OLED_DC_Set() GPIO_SetBits(GPIOB, GPIO_Pin_1)

#define OLED_CS_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_10) // CS
#define OLED_CS_Set() GPIO_SetBits(GPIOB, GPIO_Pin_10)

void SW_SPI_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			  // 初始化GPIOD3,6
	GPIO_SetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10);
	OLED_RST_Set();

	delay_ms(100);
	OLED_RST_Clr(); // 复位
	delay_ms(100);
	OLED_RST_Set();
}

void WriteCmd(unsigned char dat)
{
	u8 i;
	OLED_DC_Clr();
	OLED_CS_Clr();
	for (i = 0; i < 8; i++)
	{
		OLED_SCLK_Clr();
		if (dat & 0x80)
			OLED_SDIN_Set();
		else
			OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat <<= 1;
	}
	OLED_CS_Set();
	OLED_DC_Set();
}

void WriteDat(unsigned char dat)
{
	u8 i;
	OLED_DC_Set();
	OLED_CS_Clr();
	for (i = 0; i < 8; i++)
	{
		OLED_SCLK_Clr();
		if (dat & 0x80)
			OLED_SDIN_Set();
		else
			OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat <<= 1;
	}
	OLED_CS_Set();
	OLED_DC_Set();
}

void OLED_FILL(unsigned char BMP[])
{
	u8 i, j;
	unsigned char *p;
	p = BMP;

	for (i = 0; i < 8; i++)
	{
		WriteCmd(0xb0 + i); // page0-page1
		WriteCmd(0x00);		// low column start address
		WriteCmd(0x10);

		for (j = 0; j < 128; j++)
		{
			WriteDat(*p++);
		}
	}
}

#endif

void OLED_Init(void)
{
	OLED_RESET_LOW();
	HAL_Delay(200);
	OLED_RESET_HIGH(); // 复位

	WriteCmd(0xAE); // 关闭显示
	WriteCmd(0x00); // 为页面寻址模式设置列起始地址 低位
	WriteCmd(0x10); // 为页面寻址模式设置列起始地址 高位
	WriteCmd(0x40); // 设置显示起始行 (0x40~0x7F)

	WriteCmd(0x81); // 对比度设置
	WriteCmd(0xCF); // 对比度值

	WriteCmd(0xA1); // 设置 列与段 重新映射 0xA0左右反置 0xA1正常
	WriteCmd(0xC8); // 设置 COM 输出的扫描方向 0xC0上下反置 0xC8正常

	WriteCmd(0xA6); // 设置 显示方式	0xA6正常显示 0xA7反相显示

	WriteCmd(0xA8); // set multiplex ratio(1 to 64)
	WriteCmd(0x3f); // 1/64 duty

	WriteCmd(0xD3); // 显示偏移 (0x00~0x3F)
	WriteCmd(0x00); // not offset

	WriteCmd(0xd5); // set display clock divide ratio/oscillator frequency
	WriteCmd(0x80); // set divide ratio, Set Clock as 100 Frames/Sec

	WriteCmd(0xD9); // 设置预充电时间。
	WriteCmd(0xF1); // 预充电 15 时钟周期 & 放电 1 时钟周期

	WriteCmd(0xDA); // 设置 COM 引脚硬件配置
	WriteCmd(0x12);

	WriteCmd(0xDB); // 调整 VCOMH 稳压器输出
	WriteCmd(0x40); // Set VCOM Deselect Level

	WriteCmd(0x20); // 设置内存寻址模式
	WriteCmd(0x02); // 0x02页寻址 0x01列寻址 0x00行寻址

	WriteCmd(0x8D); // set Charge Pump enable/disable
	WriteCmd(0x14); // set(0x10) disable

	WriteCmd(0xA4); // 根据 GDDRAM 内容启用显示输出(0xa4/0xa5)

	WriteCmd(0xA6); // 设置 显示方式	0xA6正常显示 0xA7反相显示

	WriteCmd(0xAF); // 开启显示
	OLED_CLS();
}

void OLED_CLS(void) // 清屏
{
	unsigned char m, n;
	for (m = 0; m < 8; m++)
	{
		WriteCmd(0xb0 + m); // page0-page1
		WriteCmd(0x00);		// low column start address
		WriteCmd(0x10);		// high column start address
		for (n = 0; n < 128; n++)
		{
			WriteDat(0x00);
		}
	}
}

void OLED_ON(void)
{
	WriteCmd(0X8D); // 设置电荷泵
	WriteCmd(0X14); // 开启电荷泵
	WriteCmd(0XAF); // OLED唤醒
}

void OLED_OFF(void)
{
	WriteCmd(0X8D); // 设置电荷泵
	WriteCmd(0X10); // 关闭电荷泵
	WriteCmd(0XAE); // OLED休眠
}
