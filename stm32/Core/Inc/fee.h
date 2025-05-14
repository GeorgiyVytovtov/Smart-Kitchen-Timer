#ifndef INC_FEE_H_
#define INC_FEE_H_

#include "main.h"

#define FEE_START_ADDRESS	((uint32_t)0x0800F000)
#define FEE_PAGE_SIZE		((uint32_t)0x400)
#define FEE_BUFFER_LEN		(FEE_PAGE_SIZE/2U)

void fee_write_data(uint32_t address, void* data, size_t dataSize);
void fee_read_data(uint32_t address, void* data, size_t dataSize);

#endif
