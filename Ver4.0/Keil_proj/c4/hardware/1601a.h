#ifndef LCD1601A_H
#define LCD1601A_H

#include "stm32f10x.h"

void LCD_INIT(void);
void LCD_WRITE_CMD(unsigned char cmd);
void LCD_WRITE_ByteDATA(unsigned char data);
void LCD_WRITE_StrDATA(unsigned char *str, unsigned char col);
void LCD_Clear(void);
void LCD_Backlight_On(void);
void LCD_Backlight_Off(void);
void LCD_Backlight_Toggle(void);

#endif
