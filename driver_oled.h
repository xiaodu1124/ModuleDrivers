#ifndef __DRIVER_OLED_H
#define __DRIVER_OLED_H

#include "stm32f1xx_hal.h"

#define OLED_WRITE_ADDR	0x78
#define OLED_READ_ADDR	0x79


void OLED_WriteCmd(uint8_t cmd);
void OLED_WriteData(uint8_t data);
void OLED_WriteNBytes(uint8_t *buf, uint16_t len);
void OLED_SetContrastValue(uint8_t value);

#define ENTIRE_DISP_ON() 	OLED_WriteCmd(0xA5)
#define ENTIRE_DISP_OFF()	OLED_WriteCmd(0xA4)

#define DISP_NORMAL()	OLED_WriteCmd(0xA6)
#define DISP_INVERSE()	OLED_WriteCmd(0xA7)

#define DISP_ON()	OLED_WriteCmd(0xAF)
#define DISP_OFF()	OLED_WriteCmd(0xAE)

typedef enum
{
	H_RIGHT = 0,
	H_LEFT = 1, 
}H_SCROLL_DIR;

typedef enum
{
	HV_RIGHT = 0,
	HV_LEFT = 1,
}HV_SCROLL_DIR;

void OLED_H_Scroll(H_SCROLL_DIR dir, uint8_t start, uint8_t fr_time, uint8_t end);
void OLED_HV_SCROLL(HV_SCROLL_DIR dir, uint8_t start, uint8_t fr_time, uint8_t end, uint8_t offset);


typedef enum
{
	H_ADDR_MODE = 0,
	V_ADDR_MODE = 1,
	PAGE_ADDR_MODE = 2,
}MEM_MODE;

#define OLED_SEG_REMAP()	OLED_WriteCmd(0xA1)
#define OLED_SEG_NOREMAP()	OLED_WriteCmd(0xA0)
#define OLED_SCAN_NORMAL()	OLED_WriteCmd(0xC0)
#define OLED_SCAN_REMAP()	OLED_WriteCmd(0xC8)

typedef enum
{
	COM_PIN_SEQ = 0,
	COM_PIN_ALT = 1,
}COM_PINS_MODE;

typedef enum
{
	COM_NOREMAP = 0, 
	COM_REMAP = 1,
}COM_REMAP_STATE;

typedef enum
{
	LEVEL_0 = 0x00,
	LEVEL_1 = 0x01,
	LEVEL_2 = 0x02,
}VCOMH_LEVEL;

typedef enum
{
	PUMP_DISABLE = 0,
	PUMP_ENABLE = 1,
}CHARGE_PUMP_STATE;

void OLED_SetScrollArea(uint8_t area, uint8_t row_num);
void OLED_SetColAddr_PAGE(uint8_t addr);
void OLED_SetMemAddrMode(MEM_MODE mode);
void OLED_SetColAddr_HV(uint8_t start, uint8_t end);
void OLED_SetPageAddr_HV(uint8_t start, uint8_t end);
void OLED_SetPageAddr_Page(uint8_t addr);
void OLED_SetDispStartline(uint8_t line);
void OLED_SetMuxRatio(uint8_t ratio);
void OLED_SetDispoffset(uint8_t offset);
void OLED_SetComConfig(COM_PINS_MODE mode, COM_REMAP_STATE state);
void OLED_SetDCLK_Freq(uint8_t div, uint8_t freq);
void OLED_SetPreChargePeriod(uint8_t phase1, uint8_t phase2);
void OLED_SetVcomlevel(VCOMH_LEVEL level);
void OLED_SetChargePump(CHARGE_PUMP_STATE state);
void OLED_SetPosition(uint8_t page, uint8_t col);
void OLED_clear(void);
void OLED_Copy(uint8_t *buf);
void OLED_Init(void);


#endif

