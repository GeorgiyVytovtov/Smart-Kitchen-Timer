#include "ds3231.h"

static I2C_HandleTypeDef* hi2c1;

void ds3231_init(I2C_HandleTypeDef* hi2c)
{
	hi2c1=hi2c;
}

void ds3231_read_time(uint8_t* hr, uint8_t* mn)
{
	uint8_t command=0x01;
	uint8_t receive[2]={0};
	HAL_I2C_Master_Transmit(hi2c1, DS3231_ADDR, &command, 1, 0xFFFF);
	HAL_I2C_Master_Receive(hi2c1, DS3231_ADDR, receive, 2, 0xFFFF);
	*mn=((receive[0]>>4)*10)+(receive[0]&0x0F);
	*hr=((receive[1]>>4)*10)+(receive[1]&0x0F);
}

void ds3231_set_time(uint8_t hr, uint8_t mn)
{
    uint8_t data[4];
    data[0] = 0x00;

    data[2] = ((mn / 10) << 4) | (mn % 10);
    data[3] = ((hr / 10) << 4) | (hr % 10);

    HAL_I2C_Master_Transmit(hi2c1, DS3231_ADDR, data, 4, HAL_MAX_DELAY);
}
