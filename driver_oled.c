#include "driver_oled.h"
#include "driver_i2c.h"

void OLED_WriteCmd(uint8_t cmd)
{
	i2c_start();
	i2c_writebyte(OLED_WRITE_ADDR);
	i2c_writebyte(0x00);
	i2c_writebyte(cmd);
	i2c_stop();
}

void OLED_WriteData(uint8_t data)
{
	i2c_start();
	i2c_writebyte(OLED_WRITE_ADDR);
	i2c_writebyte(0x40);
	i2c_writebyte(data);
	i2c_stop();
}

void OLED_WriteNBytes(uint8_t *buf, uint16_t len)
{
	int i = 0;
	if(buf == NULL) return;
	i2c_start();
	i2c_writebyte(OLED_WRITE_ADDR);
	i2c_writebyte(0x40);
	for(i = 0; i < len;i++)
	{
		i2c_writebyte(buf[i]);
	}
	i2c_stop();
}

void OLED_SetContrastValue(uint8_t value)
{
	OLED_WriteCmd(0x81);
	OLED_WriteCmd(value);
}

void OLED_H_Scroll(H_SCROLL_DIR dir, uint8_t start, uint8_t fr_time, uint8_t end)
{
	if((dir != H_RIGHT) && (dir != H_LEFT)) return;
	if((start > 0x07) || (fr_time > 0x07) || (end > 0x07)) return;
	OLED_WriteCmd(dir);
	OLED_WriteCmd(0x00);
	OLED_WriteCmd(start);
	OLED_WriteCmd(fr_time);
	OLED_WriteCmd(end);
	OLED_WriteCmd(0x00);
	OLED_WriteCmd(0xFF);
}

void OLED_HV_SCROLL(HV_SCROLL_DIR dir, uint8_t start, uint8_t fr_time, uint8_t end, uint8_t offset)
{
	if((dir != HV_LEFT) || (dir != HV_RIGHT)) return;
	if((start > 0x07) || (fr_time > 0x07) || (end > 0x07) || (offset > 0x3F)) return;
	OLED_WriteCmd(dir);
	OLED_WriteCmd(0x00);
	OLED_WriteCmd(start);
	OLED_WriteCmd(fr_time);
	OLED_WriteCmd(end);
	OLED_WriteCmd(offset);
}

void OLED_SetScrollArea(uint8_t area, uint8_t row_num)
{
	if((area > 0x3F) || (row_num > 0x7F)) return;
	OLED_WriteCmd(0xA3);
	OLED_WriteCmd(area);
	OLED_WriteCmd(row_num);
}

static MEM_MODE mem_mode = PAGE_ADDR_MODE;

void OLED_SetColAddr_PAGE(uint8_t addr)
{
	if(mem_mode != PAGE_ADDR_MODE) return;
	if(addr > 0x7F) return;
	OLED_WriteCmd(0x00 + (addr & 0x0F));
	OLED_WriteCmd(0x10 + (addr >> 4));
}

void OLED_SetMemAddrMode(MEM_MODE mode)
{
	if((mode != H_ADDR_MODE) && (mode != V_ADDR_MODE) && (mode != PAGE_ADDR_MODE))	return;
	OLED_WriteCmd(0x20);
	OLED_WriteCmd(mode);
	mem_mode = mode;
}

void OLED_SetColAddr_HV(uint8_t start, uint8_t end)
{
	if(mem_mode == PAGE_ADDR_MODE) return;
	if((start > 127) ||(end > 127))	return;
	OLED_WriteCmd(0x21);
	OLED_WriteCmd(start);
	OLED_WriteCmd(end);
}

void OLED_SetPageAddr_HV(uint8_t start, uint8_t end)
{
	if(mem_mode == PAGE_ADDR_MODE) return;
	if((start > 7) || (end > 7)) return;
	OLED_WriteCmd(0x22);
	OLED_WriteCmd(start);
	OLED_WriteCmd(end);
}

void OLED_SetPageAddr_Page(uint8_t addr)
{
	if(mem_mode != PAGE_ADDR_MODE) return;
	if(addr > 7) return;
	OLED_WriteCmd(0xB0 + addr);
}

void OLED_SetDispStartline(uint8_t line)
{
	if(line > 63) return;
	OLED_WriteCmd(0x40 + line);
}

void OLED_SetMuxRatio(uint8_t ratio)
{
	if((ratio < 15 || ratio > 63)) return;
	OLED_WriteCmd(0xA8);
	OLED_WriteCmd(ratio);
}

void OLED_SetDispoffset(uint8_t offset)
{
	if(offset > 63) return;
	OLED_WriteCmd(0xD3);
	OLED_WriteCmd(offset);
}

void OLED_SetComConfig(COM_PINS_MODE mode, COM_REMAP_STATE state)
{
	if((mode != COM_PIN_SEQ) && (mode != COM_PIN_ALT)) return;
	if((state != COM_NOREMAP) &&(state != COM_REMAP)) return;
	OLED_WriteCmd(0xDA);
	OLED_WriteCmd(0x02 + (mode << 4) + (state << 5));s
}

void OLED_SetDCLK_Freq(uint8_t div, uint8_t freq)
{
	if((div > 0x0F) || (freq > 0x0F)) return;
	OLED_WriteCmd(0xD5);
	OLED_WriteCmd(div + (freq << 4));
}

void OLED_SetPreChargePeriod(uint8_t phase1, uint8_t phase2)
{
	if((phase1 > 0x0F) || (phase2 > 0x0F)) return;
	OLED_WriteCmd(0xD9);
	OLED_WriteCmd(phase1 + (phase2 << 4));
}

void OLED_SetVcomlevel(VCOMH_LEVEL level)
{
	if((level != LEVEL_0) && (level != LEVEL_1) && (level != LEVEL_2)) return;
	OLED_WriteCmd(0xDB);
	OLED_WriteCmd(level << 4);
}

void OLED_SetChargePump(CHARGE_PUMP_STATE state)
{
	if((state != PUMP_DISABLE) && (state != PUMP_ENABLE)) return;
	OLED_WriteCmd(0x8D);
	OLED_WriteCmd((state<<2)+ 0x10);
}

void OLED_SetPosition(uint8_t page, uint8_t col)
{
	OLED_SetPageAddr_Page(page);
	OLED_SetColAddr_PAGE(col);
}

void OLED_clear(void)
{
	int i = 0;
	uint8_t buf[128] = {0};
	for(i = 0; i< 8; i++)
	{
		OLED_SetPosition(i, 0);
		OLED_WriteNBytes(&buf[i], 128);
	}
}

void OLED_Copy(uint8_t *buf)
{
	int i = 0;
	for(i = 0; i< 8;i++)
	{
		OLED_SetPosition(i, 0);
		OLED_WriteNBytes(&buf[i*128], 128);
	}
}

void OLED_Init(void)
{
	OLED_SetMemAddrMode(PAGE_ADDR_MODE);
	OLED_SetMuxRatio(0x3F);
	OLED_SetDispoffset(0x00);
	OLED_SetDispStartline(0x00);
	OLED_SEG_REMAP();
	OLED_SCAN_REMAP();
	OLED_SetComConfig(COM_PIN_SEQ, COM_NOREMAP);
	OLED_SetContrastValue(0x7F);
	ENTIRE_DISP_OFF();
	DISP_NORMAL();
	OLED_SetDCLK_Freq(0x00, 0x08);
	OLED_SetChargePump(PUMP_ENABLE);
	OLED_SetComConfig(COM_PIN_ALT, COM_NOREMAP);
	DISP_ON();
}
