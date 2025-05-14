#include "display.h"

void view_blink_current_time(uint8_t segment, uint8_t hr, uint8_t mn) {
	switch (segment) {
	case 0:
		lcd_set_position(LCD_ADDR, 3, 0);
		lcd_send_string(LCD_ADDR, "  ");
		view_current_time(hr, mn);
		break;

	case 1:
		lcd_set_position(LCD_ADDR, 0, 0);
		lcd_send_string(LCD_ADDR, "  ");
		view_current_time(hr, mn);
		break;
	}
}


void view_blink_timer_time(uint8_t sigment, uint16_t time)
{
	switch(sigment)
	{
	case 0:
		lcd_set_position(LCD_ADDR, 10, 1);
		lcd_send_string(LCD_ADDR, "  ");
		view_timer_time(time);
		break;
	case 1:
		lcd_set_position(LCD_ADDR, 7, 1);
		lcd_send_string(LCD_ADDR, "  ");
		view_timer_time(time);
		break;
	case 2:
		lcd_set_position(LCD_ADDR, 4, 1);
		lcd_send_string(LCD_ADDR, "  ");
		view_timer_time(time);
		break;
	}
}

void view_temperature(int temperature, uint8_t humidity)
{
	char data[30];
	sprintf(data, "%d%cC,%d%%", temperature, 0xDF, humidity);
	lcd_set_position(LCD_ADDR, 7, 0);
	lcd_send_string(LCD_ADDR, data);
}

void view_current_time(uint8_t hr, uint8_t mn) {
	char data[6];
	sprintf(data, "%02d:%02d", hr, mn);
	lcd_set_position(LCD_ADDR, 0, 0);
	lcd_send_string(LCD_ADDR, data);
}

void view_timer_time(uint32_t time) {
	char data[9];
	uint8_t hr = time / 3600;
	uint8_t mn = (time % 3600) / 60;
	uint8_t ss = ((time % 3600) % 60);
	sprintf(data, "%02d:%02d:%02d", hr, mn, ss);
	lcd_set_position(LCD_ADDR, 4, 1);
	lcd_send_string(LCD_ADDR, data);
}
