#include "mp3.h"
#include "Serial.h"

static uint8_t send_buf[10];

void MP3CMD(uint8_t cmd, uint16_t data)
{
	send_buf[0] = 0x7e;
	send_buf[1] = 0xff;
	send_buf[2] = 0x06;
	send_buf[3] = cmd;
	send_buf[4] = 0x00;
	send_buf[5] = (uint8_t)(data >> 8);
	send_buf[6] = (uint8_t)(data);
	send_buf[7] = (uint8_t)((~(0xff + 0x06 + cmd + data) + 1) >> 8);
	send_buf[8] = (uint8_t)((~(0xff + 0x06 + cmd + data) + 1) & 0x00ff);
	send_buf[9] = 0xef;

	Serial_SendArray(send_buf, 10);
}

void mp3_Init(void)
{
	Serial_Init();
}

void mp3_start(void)
{
	MP3CMD(0x12, 100);
}

void mp3_over(void)
{
	MP3CMD(0x12, 101);
}

void mp3_boom(void)
{
	MP3CMD(0x12, 102);
}

void mp3_boom_music(void)
{
	send_buf[0] = 0x7e;
	send_buf[1] = 0xff;
	send_buf[2] = 0x06;
	send_buf[3] = 0x0f;
	send_buf[4] = 0x00;
	send_buf[5] = 0x02;
	send_buf[6] = 0x00;
	send_buf[7] = 0xfe;
	send_buf[8] = 0xea;
	send_buf[9] = 0xef;

	Serial_SendArray(send_buf, 10);
}
