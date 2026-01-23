#ifndef MP3_H
#define MP3_H

#include <stdint.h>

void mp3_Init(void);
void MP3CMD(uint8_t cmd, uint16_t data);
void mp3_start(void);
void mp3_over(void);
void mp3_boom(void);
void mp3_boom_music(void);

#endif
