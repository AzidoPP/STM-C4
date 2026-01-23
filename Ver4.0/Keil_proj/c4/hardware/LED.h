#ifndef LED_H
#define LED_H

#include "stm32f10x.h"
#include <stdint.h>

#define LED_PWM_MAX 1000

void LED_Init(void);
void LED_SetColor(uint16_t red, uint16_t green);
void LED_SetRed(uint16_t red);
void LED_SetGreen(uint16_t green);
void LED_SetYellow(uint16_t level);
void LED_AllOff(void);

void Relay_On(void);
void Relay_Off(void);

void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);

#endif
