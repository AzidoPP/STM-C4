#include "mp3.h"
#include "Serial.h"

static uint8_t send_buf[10];

static void MP3_SendFrame(uint8_t cmd, uint8_t para1, uint8_t para2)
{
	uint16_t checksum;

	send_buf[0] = 0x7e;
	send_buf[1] = 0xff;
	send_buf[2] = 0x06;
	send_buf[3] = cmd;
	send_buf[4] = 0x00;
	send_buf[5] = para1;
	send_buf[6] = para2;
	checksum = (uint16_t)(0U - (0xffU + 0x06U + (uint16_t)cmd + (uint16_t)para1 + (uint16_t)para2));
	send_buf[7] = (uint8_t)(checksum >> 8);
	send_buf[8] = (uint8_t)(checksum & 0xffU);
	send_buf[9] = 0xef;

	Serial_SendArray(send_buf, 10);
}

void mp3_Init(void)
{
	Serial_Init();
}

void mp3_SetVolume(uint8_t volume)
{
	MP3_SendFrame(0x06, 0x00, volume);
}

void mp3_PlayMp3Track(uint16_t track)
{
	MP3_SendFrame(0x12, (uint8_t)(track >> 8), (uint8_t)track);
}

