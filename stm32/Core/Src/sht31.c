#include "sht31.h"

static I2C_HandleTypeDef* hi2c1;
static bool is_start_mesurement;
static uint32_t start_mesurement_time;

void sht31_init(I2C_HandleTypeDef* hi2c) {
	hi2c1 = hi2c;
}

uint8_t SHT31_CRC8(const uint8_t *data, int len) {
	uint8_t crc = 0xFF;
	for (int j = 0; j < len; ++j) {
		crc ^= data[j];
		for (int i = 0; i < 8; ++i) {
			if (crc & 0x80)
				crc = (crc << 1) ^ 0x31;
			else
				crc <<= 1;
		}
	}
	return crc;
}

bool sht31_start_mesurement() {
	if (!is_start_mesurement) {
		uint8_t command[2];
		command[0] = CMD_MEAS_HIGHREP >> 8;
		command[1] = CMD_MEAS_HIGHREP & 0xFF;

		if (HAL_I2C_Master_Transmit(hi2c1, SHT31_ADDR, command, 2,
				HAL_MAX_DELAY) != HAL_OK) {
			is_start_mesurement = false;
		} else {
			is_start_mesurement = true;
			start_mesurement_time = HAL_GetTick();
		}
	}
	return is_start_mesurement;
}

bool sht31_is_ready_to_read() {
	if ((HAL_GetTick() - start_mesurement_time > 20)
			&& is_start_mesurement) {
		return true;
	}
	return false;
}

bool SHT31_read_temp_humidity(int *temperature, int *humidity) {
	if (sht31_is_ready_to_read()) {
		uint8_t data[6];
		if (HAL_I2C_Master_Receive(hi2c1, SHT31_ADDR, data, 6, HAL_MAX_DELAY)
				!= HAL_OK)
			return false;

		if (SHT31_CRC8(data, 2) != data[2]
				|| SHT31_CRC8(data + 3, 2) != data[5])
			return false;

		uint16_t temp_raw = (data[0] << 8) | data[1];
		uint16_t hum_raw = (data[3] << 8) | data[4];

		*temperature = -45.0f + 175.0f * ((float) temp_raw / 65535.0f);
		*humidity = 100.0f * ((float) hum_raw / 65535.0f);

		is_start_mesurement = false;
		return true;
	}
	return false;
}
