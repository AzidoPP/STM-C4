#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void Timebase_Init(void);
void Timebase_Tick(void);
uint32_t Timebase_Millis(void);

#endif
