
#ifndef __I2C_H
#define __I2C_H
#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

void I2C1_Config (uint8_t mode);

bool I2C1_Start (void);

bool I2C1_Write (uint8_t data);

bool I2C1_TransmitMaster(char *buffer, uint32_t size);

bool I2C1_Address (uint8_t Address);

void I2C1_Stop (void);

void I2C1_SetAddress(uint8_t address);

char* I2C1_ReceiveSlave(uint8_t *buffer);



#ifdef __cplusplus
}
#endif
#endif
