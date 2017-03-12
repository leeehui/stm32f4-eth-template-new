

#include "tcp_led_ctrl.h"


uint8_t led_buffer[CH_NUM][CH_LED_NUM * 3];
static uint16_t led_num_to_send = 0;


static void fill_led_buffer(uint8_t *data_start, uint8_t channel, uint16_t led_start, uint16_t led_end);
static void trigger_led();



int32_t process_one_frame(uint8_t *frame_data, uint16_t frame_len)
{
	RECV_FRAME_PTR_t frame_ptr = (RECV_FRAME_PTR_t)frame_data;
	uint8_t *data = (uint8_t *)(&(frame_ptr->data_len));
	uint8_t xor_value = *data++;//first XOR
	uint32_t xor_counter = frame_ptr->data_len;//xor = XOR(len(2) + RGB data), frame_ptr->data_len = (RGB data + xorValue(1))
	if(frame_ptr->start != FRAME_START)
	{
		return 1;
	}
	
	//frame_len = frame_ptr->len + start(1) + end(1) + datalen(2)
	if(frame_ptr->data_len != frame_len - 4) 
	{
		return 2;
	}
	
	//ignore Xor check
	for(int i = 0; i < xor_counter; i++)
	{
		xor_value ^= *data++;
	}
	
	if(xor_value != *data)
	{
		printf("xor error\n");
	}
	
	switch(frame_ptr->cmd)
	{
		case RGB_DATA:
		{
			fill_led_buffer(&(frame_ptr->rgb_data), frame_ptr->channel, frame_ptr->led_start, frame_ptr->led_end);
			send_ack(RGB_DATA_ACK,STATUS_OK);
			break;
		}
		case RGB_TRIGGER:
		{
			trigger_led();
			send_ack(RGB_TRIGGER_ACK,STATUS_OK);
			break;
		}
		case HEART_BEAT:
		{
			break;
		}
		default:break;
	}
	return 0;
}

static void fill_led_buffer(uint8_t *data_start, uint8_t channel, uint16_t led_start, uint16_t led_end)
{
	uint16_t buffer_addr_start = led_start * 3;
	uint16_t len = (led_end - led_start + 1) * 3;
	if(channel > CH_NUM)
	{
		//error channel num
		return;
	}
	if(led_end > CH_LED_NUM - 1)
	{
		return;
	}
	if(led_end < led_start)
	{
		//error channel num
		return;
	}
	
	if(led_num_to_send < led_end)
	{
		led_num_to_send = led_end;
	}
	
	memcpy(&led_buffer[channel][buffer_addr_start], data_start, len);	
}

//just for function test
static void update_channel_data(uint8_t channel, uint8_t red, uint8_t green, uint8_t blue)
{
	uint32_t size = CH_LED_NUM * 3;
	for(int i = 0; i < size; i+=3)
	{
		led_buffer[channel][i] = green;
		led_buffer[channel][i+1] = red;
		led_buffer[channel][i+2] = blue;
	}
}

//just for function test
static void update_all_data(uint8_t red, uint8_t green, uint8_t blue)
{
	for(int i = 0; i < CH_NUM; i++)
	{
		update_channel_data(i, red,green,blue);
	}
}
void delay_ms(uint32_t delay)
{
	for(int i=0; i<delay; i++)
	{
		for(int j=0; j<168067; j++)
		{}
	}
}

//just for function test
void test()
{
	//while(1)
	//{
 

	for(int i = 0; i < 255; i++)
	{
		update_all_data(0,i,0);
		ws2812_send(&led_buffer[0][0], 
							   &led_buffer[1][0],
							   &led_buffer[2][0],
							   &led_buffer[3][0],
							   &led_buffer[4][0],
							   &led_buffer[5][0],
							   CH_LED_NUM*3
								);
		delay_ms(1);
	}

	//}
}
	
static void trigger_led()
{
	__set_PRIMASK(1);
	ws2812_send(&led_buffer[0][0], 
							   &led_buffer[1][0],
							   &led_buffer[2][0],
							   &led_buffer[3][0],
							   &led_buffer[4][0],
							   &led_buffer[5][0],
							   led_num_to_send * 3
								);
	__set_PRIMASK(0);
}

void send_ack(uint8_t cmd, uint8_t status)
{

	uint8_t data[7] = {FRAME_START,3,0,0,0,0,FRAME_END}; 
	data[3] = cmd;
	data[4] = status;
	data[5] = data[1] ^ data[2] ^ data[3] ^ data[4];
	//es->state = ES_RECEIVED;
//	es->pcb = tpcb;
//	es->p_tx = pbuf_alloc(PBUF_TRANSPORT, 7 , PBUF_POOL);
//	if(NULL != es->p_tx)
//	{
//		pbuf_take(es->p_tx, data, 7);
//		tcp_echoclient_send(tpcb, es);
//	}
//	else
//	{
//		printf("pbuf_alloc return null\n");
//	}

}





