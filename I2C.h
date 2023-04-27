/**
  ****************************************

  I2C Setup For STM32F446RE
  Author:   ControllersTech
  Updated:  31st Jan 2020

  *****************************************
  Copyright (C) 2017 ControllersTech.com
*/
#ifndef __I2C_H
#define __I2C_H
#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include "stm32f4xx.h"

void I2C1_Config (uint8_t mode);

void I2C1_Start (void);

void I2C1_Write (uint8_t data);

void I2C1_TransmitMaster(char *buffer, uint32_t size);

void I2C1_Address (uint8_t Address);

void I2C1_Stop (void);

void I2C1_SetAddress(uint8_t address);

void I2C1_ReceiveSlave(uint8_t *buffer, uint32_t size);

#ifdef __cplusplus
}
#endif
#endif
