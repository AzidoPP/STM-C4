#ifndef MP3_H
#define MP3_H

#include <stdint.h>

void mp3_Init(void);
void mp3_SetVolume(uint8_t volume);
void mp3_PlayMp3Track(uint16_t track);

#endif
