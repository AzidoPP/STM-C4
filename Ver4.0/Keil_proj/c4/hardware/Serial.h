#ifndef SERIAL_H
#define SERIAL_H

#include "stm32f10x.h"
#include <stdint.h>

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);

#endif
