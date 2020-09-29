#include <string.h>

#include <rthw.h>
#include <rtdevice.h>
#include <board.h>

#include "word.h"
#include "matrix_show.h"

#ifdef _WIN32
#include <stdio.h> /* for putchar */
#endif

/* matrix thread */
#ifndef RT_USING_HEAP
static struct rt_thread matrix_thread;
ALIGN(RT_ALIGN_SIZE)
static char matrix_thread_stack[MATRIX_THREAD_STACK_SIZE];
uint8_t matrix_out_data[FONT_LEN][(AS_MATRIX_MAX_LEN];

#endif

uint8_t **matrix_out_data;

static int show_word_len=0;
/*
	A------>PA0
	B------>PA1
	R1----->PF0
	G1----->PF1
	B1----->PF2
	R2----->PF3
	G2----->PF4
	B2----->PF5
	CLK---->PF6
	LAT---->PF7
	OE----->PF8
*/

/* defined the address A pin: PA0 */
#define MATRIX_ADDR_A    GET_PIN(A, 0)
/* defined the address A pin: PA1 */
#define MATRIX_ADDR_B    GET_PIN(A, 1)

/* defined the R1 pin: PF0 */
#define MATRIX_R1    GET_PIN(F, 0)
/* defined the G1 pin: PF1 */
#define MATRIX_G1    GET_PIN(F, 1)
/* defined the B1 pin: PF2 */
#define MATRIX_B1    GET_PIN(F, 2)

/* defined the R2 pin: PF3 */
#define MATRIX_R2    GET_PIN(F, 3)
/* defined the G2 pin: PF4 */
#define MATRIX_G2    GET_PIN(F, 4)
/* defined the B2 pin: PF5 */
#define MATRIX_B2    GET_PIN(F, 5)


/* defined the CLK pin: PF6 */
#define MATRIX_CLK    GET_PIN(F, 6)
/* defined the LAT pin: PF7 */
#define MATRIX_LAT    GET_PIN(F, 7)
/* defined the OE pin: PF8 */
#define MATRIX_OE    GET_PIN(F, 8)


#define MATRIX_CLK_H			rt_pin_write(MATRIX_CLK, PIN_HIGH)
#define MATRIX_CLK_L			rt_pin_write(MATRIX_CLK, PIN_LOW)

#define MATRIX_LAT_H			rt_pin_write(MATRIX_LAT, PIN_HIGH)
#define MATRIX_LAT_L			rt_pin_write(MATRIX_LAT, PIN_LOW)

#define MATRIX_OE_H				rt_pin_write(MATRIX_OE, PIN_HIGH)
#define MATRIX_OE_L				rt_pin_write(MATRIX_OE, PIN_LOW)

#define MATRIX_R1_H				rt_pin_write(MATRIX_R1, PIN_HIGH)
#define MATRIX_R1_L				rt_pin_write(MATRIX_R1, PIN_LOW)
#define MATRIX_G1_H				rt_pin_write(MATRIX_G1, PIN_HIGH)
#define MATRIX_G1_L				rt_pin_write(MATRIX_G1, PIN_LOW)
#define MATRIX_B1_H				rt_pin_write(MATRIX_B1, PIN_HIGH)
#define MATRIX_B1_L				rt_pin_write(MATRIX_B1, PIN_LOW)

#define MATRIX_R2_H				rt_pin_write(MATRIX_R2, PIN_HIGH)
#define MATRIX_R2_L				rt_pin_write(MATRIX_R2, PIN_LOW)
#define MATRIX_G2_H				rt_pin_write(MATRIX_G2, PIN_HIGH)
#define MATRIX_G2_L				rt_pin_write(MATRIX_G2, PIN_LOW)
#define MATRIX_B2_H				rt_pin_write(MATRIX_B2, PIN_HIGH)
#define MATRIX_B2_L				rt_pin_write(MATRIX_B2, PIN_LOW)

static void matrix_io_init(void)
{
	//set all port as output mode.
	rt_pin_mode(MATRIX_ADDR_A, PIN_MODE_OUTPUT);
	rt_pin_mode(MATRIX_ADDR_B, PIN_MODE_OUTPUT);
	
	rt_pin_mode(MATRIX_R1, PIN_MODE_OUTPUT);
	rt_pin_mode(MATRIX_G1, PIN_MODE_OUTPUT);
	rt_pin_mode(MATRIX_B1, PIN_MODE_OUTPUT);
	rt_pin_mode(MATRIX_R2, PIN_MODE_OUTPUT);
	rt_pin_mode(MATRIX_G2, PIN_MODE_OUTPUT);
	rt_pin_mode(MATRIX_B2, PIN_MODE_OUTPUT);
	
	rt_pin_mode(MATRIX_CLK, PIN_MODE_OUTPUT);
	rt_pin_mode(MATRIX_LAT, PIN_MODE_OUTPUT);
	rt_pin_mode(MATRIX_OE, PIN_MODE_OUTPUT);
}

static void matrix_set_line(uint8_t line)
{
	//two address port include 0b'00 01 10 11.
	switch(line)
	{
		case 0:
			rt_pin_write(MATRIX_ADDR_A, PIN_LOW);
			rt_pin_write(MATRIX_ADDR_B, PIN_LOW);
			break;
		case 1:
			rt_pin_write(MATRIX_ADDR_A, PIN_HIGH);
			rt_pin_write(MATRIX_ADDR_B, PIN_LOW);
			break;
		case 2:
			rt_pin_write(MATRIX_ADDR_A, PIN_LOW);
			rt_pin_write(MATRIX_ADDR_B, PIN_HIGH);
			break;
		case 3:
			rt_pin_write(MATRIX_ADDR_A, PIN_HIGH);
			rt_pin_write(MATRIX_ADDR_B, PIN_HIGH);
			break;
		default:
			rt_pin_write(MATRIX_ADDR_A, PIN_LOW);
			rt_pin_write(MATRIX_ADDR_B, PIN_LOW);
			break;
	}
}

static void matrix_show( uint8_t **matrix_out,int start)
{
	uint8_t dat_low;
	uint8_t dat_low1;
	
	uint8_t dat_high;
	uint8_t dat_high1;

	int start_temp = start;
	
	for(int n = 0; n<4; n++)
	{
		matrix_set_line((uint8_t)(n));
		start_temp = start;
		for( int i = 0; i< SHOW_LEN; i++)
		{
			MATRIX_OE_H;
			MATRIX_LAT_L;
			
			if( start_temp  >= (show_word_len +  BLANK_MATRIX_LEN))
				start_temp = 0;
			
			dat_high = matrix_out[n][start_temp];
			dat_high1 = matrix_out[n+4][start_temp];
			
			dat_low = matrix_out[n+8][start_temp];
			dat_low1 = matrix_out[n+12][start_temp];

			start_temp++;
			for( int k = 0; k < 8; k++)
			{
				MATRIX_CLK_L;
				if( dat_high1 & (1<<(7-k)))
				{
					MATRIX_R1_H;
				}
				else
				{
					MATRIX_R1_L;
				}
				
				if( dat_low1 & (1<<(7-k)))
				{
					MATRIX_R2_H;
				}
				else
				{
					MATRIX_R2_L;
				}
			MATRIX_CLK_H;
		}
			
		for( int k = 0; k < 8; k++)
		{

				MATRIX_CLK_L;
				if( dat_high & (1<<k))
				{
					MATRIX_R1_H;
				}
				else
				{
					MATRIX_R1_L;
				}
				if( dat_low & (1<<k))
				{
					MATRIX_R2_H;
				}
				else
				{
					MATRIX_R2_L;
				}
			MATRIX_CLK_H;
		}

		}
		MATRIX_LAT_H;
		MATRIX_LAT_L;
		MATRIX_OE_L;
		rt_hw_us_delay(900);
	}
}

static void matrix_show_round(void)
{
	int start = 0;
	while(1)
	{
		for(int i =0 ; i<40 ;i++)
			matrix_show(matrix_out_data,start);
		start +=1;
		if(start >= show_word_len +  BLANK_MATRIX_LEN)
			start = 0;
	}

}
void set_matrix_data(int *index,int ifasc,uint8_t *data_from)
{
	int start = *index;
	for(int i =0; i < FONT_LEN; i++)
	{
		if(ifasc)
		{
			matrix_out_data[i][start+BLANK_MATRIX_LEN] = data_from[i];
		}
		else
		{
			matrix_out_data[i][start+BLANK_MATRIX_LEN] = data_from[i*2];
			matrix_out_data[i][start+1+BLANK_MATRIX_LEN] = data_from[i*2+1];
		}
	}
	if(ifasc)
	{
		*index+=1;
	}
	else
	{
		*index+=2;
	}
}

static void matrix_buffer_prepare(void)
{
	int idx = 0;
	for( int i = 0; i < TRUE_WORD_LEN; i++)
	{
		set_matrix_data(&idx,0,(uint8_t *)word_date[i]);
	}
}

void clean_matrix_buffer(void)
{
	for(int i = 0; i < FONT_LEN; i++ )
	{
		memset(matrix_out_data[i], 0, sizeof(uint8_t) * AS_MATRIX_MAX_LEN);
	}

}

void set_show_len(int len)
{
	show_word_len = len;
}

static void matrix_thread_entry(void *parameter)
{
	matrix_io_init();
	if(matrix_out_data == NULL)
		rt_kprintf("matrix out buffer error\n");

	matrix_buffer_prepare();
	while(1)
	{
		//matrix_show(matrix_out_data,20);
		matrix_show_round();
	}
	
}

int matrix_thread_init(void)
{
	rt_err_t result = RT_EOK;
    rt_thread_t tid;
	show_word_len = TRUE_WORD_LEN;

#ifdef RT_USING_HEAP
	matrix_out_data = (uint8_t **)rt_malloc(FONT_LEN * sizeof(uint8_t *));
	for(int i = 0; i < FONT_LEN; i++ )
	{
		
		matrix_out_data[i] = (uint8_t *)rt_calloc(AS_MATRIX_MAX_LEN, sizeof(uint8_t));
		memset(matrix_out_data[i], 0, sizeof(uint8_t) * AS_MATRIX_MAX_LEN);
	}
	
	tid = rt_thread_create(MATRIX_THREAD_NAME,
						   matrix_thread_entry, RT_NULL,
						   MATRIX_THREAD_STACK_SIZE, MATRIX_THREAD_PRIORITY, 10);
#else
	matrix_out = matrix_out_data;
	result = rt_thread_init(&matrix_thread,
							MATRIX_THREAD_NAME,
							matrix_thread_entry, RT_NULL,
							&matrix_thread_stack[0], sizeof(matrix_thread_stack),
							MATRIX_THREAD_PRIORITY, 10);
#endif /* RT_USING_HEAP */

    if (tid != NULL && result == RT_EOK)
        rt_thread_startup(tid);
    return 0;


}

INIT_APP_EXPORT(matrix_thread_init);

