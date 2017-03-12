#ifndef __TCP_LED_CTRL_H
#define	__TCP_LED_CTRL_H

#include <stdint.h>
#include <string.h>
#include "light_ws2812_cortex.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/api.h"



/*send rgb data paralelly*/
#define CH_NUM 6
#define LED_PER_FRAME 483
#define FRAME_PER_CHANNEL 2
/*
 * 483 * 2
 * 483 is max num, the data can be transfered within one tcp frame
 */
#define CH_LED_NUM  LED_PER_FRAME*FRAME_PER_CHANNEL



enum
{
	STATUS_OK = 0,
	STATUS_ERR
};


enum 
{
	FRAME_START = 0xAA,
	FRAME_END = 0x55
};

enum
{
	RGB_DATA = 0xD1,
	RGB_TRIGGER = 0xD2,
	RGB_DATA_ACK = 0xC1,
	RGB_TRIGGER_ACK = 0xC2,
	HEART_BEAT = 0xC3
};

typedef  __packed struct  RECV_FRAME 
{
	uint8_t start;
	uint16_t data_len;
	uint8_t cmd;
	uint8_t channel;
	uint16_t led_start;
	uint16_t led_end;
	uint8_t rgb_data;	
}  RECV_FRAME_t, *RECV_FRAME_PTR_t;


int32_t process_one_frame(struct netconn *conn, uint8_t *frame_data, uint16_t frame_len);
void send_ack(struct netconn *conn, uint8_t cmd, uint8_t status);


#endif
