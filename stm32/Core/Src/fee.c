#include "fee.h"

HAL_StatusTypeDef fee_write(uint32_t address, uint16_t *data, uint32_t dataSize)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t page_address = address & ~(FLASH_PAGE_SIZE - 1);
    uint16_t buffer[FEE_BUFFER_LEN];
    HAL_FLASH_Unlock();

    for (uint32_t i = 0; i < FEE_BUFFER_LEN; i++) {
        buffer[i] = *(__IO uint16_t*)(page_address + i * 2);
    }

    uint32_t offset = address - page_address;
    for (uint32_t i = 0; i < dataSize; i++) {
        buffer[offset / 2 + i] = data[i];
    }

    FLASH_EraseInitTypeDef erase_init_struct;
    erase_init_struct.TypeErase = FLASH_TYPEERASE_PAGES;
    erase_init_struct.PageAddress = page_address;
    erase_init_struct.NbPages = 1;
    uint32_t page_error;
    status = HAL_FLASHEx_Erase(&erase_init_struct, &page_error);
    if (status != HAL_OK) {
        HAL_FLASH_Lock();
        return status;
    }

    for (uint32_t i = 0; i < FEE_BUFFER_LEN; i++) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, page_address + i * 2, buffer[i]);
        if (status != HAL_OK) {
            break;
        }
    }
    HAL_FLASH_Lock();
    return status;
}

void fee_read(uint32_t address, uint16_t *data, uint32_t dataSize)
{
    for (uint32_t i = 0; i < dataSize; i++) {
        data[i] = *(__IO uint16_t*)address;
        address += 2;
    }
}

void fee_write_data(uint32_t address, void *data, size_t dataSize)
{
    uint16_t bytes[dataSize / 2];
    memcpy(bytes, data, dataSize);
    fee_write(address, bytes, dataSize);
}

void fee_read_data(uint32_t address, void *data, size_t dataSize)
{
    uint16_t bytes[dataSize / 2];
    fee_read(address, bytes, dataSize);
    memcpy(data, bytes, dataSize);
}
