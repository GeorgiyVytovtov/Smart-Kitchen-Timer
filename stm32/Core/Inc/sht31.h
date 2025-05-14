#ifndef INC_SHT31_H_
#define INC_SHT31_H_

#include "main.h"


#define SHT31_ADDR       (0x44 << 1)
#define CMD_MEAS_HIGHREP 0x2400

void sht31_init(I2C_HandleTypeDef* hi2c);
bool sht31_start_mesurement();
bool sht31_is_ready_to_read();
bool SHT31_read_temp_humidity(int *temperature, int *humidity);


#endif
