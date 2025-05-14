#include "lcd1602_i2c.h"

static I2C_HandleTypeDef* hi2c1;

HAL_StatusTypeDef lcd_send_internal(uint8_t lcd_addr, uint8_t data,
		uint8_t flags) {
	HAL_StatusTypeDef res;
	for (;;) {
		res = HAL_I2C_IsDeviceReady(hi2c1, lcd_addr, 1, HAL_MAX_DELAY);
		if (res == HAL_OK)
			break;
	}

	uint8_t up = data & 0xF0;
	uint8_t lo = (data << 4) & 0xF0;

	uint8_t data_arr[4];
	data_arr[0] = up | flags | BACKLIGHT | PIN_EN;
	data_arr[1] = up | flags | BACKLIGHT;
	data_arr[2] = lo | flags | BACKLIGHT | PIN_EN;
	data_arr[3] = lo | flags | BACKLIGHT;

	res = HAL_I2C_Master_Transmit(hi2c1, lcd_addr, data_arr, sizeof(data_arr),
			HAL_MAX_DELAY);
	HAL_Delay(LCD_DELAY_MS);
	return res;
}

void lcd_send_command(uint8_t lcd_addr, uint8_t cmd) {
	lcd_send_internal(lcd_addr, cmd, 0);
}

void lcd_send_data(uint8_t lcd_addr, uint8_t data) {
	lcd_send_internal(lcd_addr, data, PIN_RS);
}

void lcd_init(uint8_t lcd_addr, I2C_HandleTypeDef* hi2c) {
	hi2c1=hi2c;
	HAL_Delay(40);
	lcd_send_command(lcd_addr, 0b00110000);
	lcd_send_command(lcd_addr, 0b00000010);
	lcd_send_command(lcd_addr, 0b00001100);
	lcd_send_command(lcd_addr, 0b00000001);
	lcd_clear(lcd_addr);
}

void lcd_send_string(uint8_t lcd_addr, char *str) {
	while (*str) {
		lcd_send_data(lcd_addr, (uint8_t) (*str));
		str++;
	}
}

void lcd_set_position(uint8_t lcd_addr, uint8_t x, uint8_t y)
{
    if (x < 16 && y < 2)
    {
        uint8_t address = (y == 0) ? (0x80 + x) : (0xC0 + x);
        lcd_send_command(lcd_addr, address);
        HAL_Delay(1);
    }
}

void lcd_clear(uint8_t lcd_addr) {
    lcd_send_command(lcd_addr, 0x01);
    HAL_Delay(2);
}

