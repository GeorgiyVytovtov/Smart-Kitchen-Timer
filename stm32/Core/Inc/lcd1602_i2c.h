#ifndef INC_LCD1602_I2C_H_
#define INC_LCD1602_I2C_H_

#include "main.h"

#define LCD_ADDR (0x27 << 1)
#define PIN_RS    (1 << 0)
#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)

#define LCD_DELAY_MS 5

void lcd_init(uint8_t lcd_addr, I2C_HandleTypeDef* hi2c);
HAL_StatusTypeDef lcd_send_internal(uint8_t lcd_addr, uint8_t data,
		uint8_t flags);
void lcd_send_command(uint8_t lcd_addr, uint8_t cmd);
void lcd_send_data(uint8_t lcd_addr, uint8_t data);
void lcd_send_string(uint8_t lcd_addr, char *str);
void lcd_set_position(uint8_t lcd_addr, uint8_t x, uint8_t y);
void lcd_clear(uint8_t lcd_addr);

#endif
