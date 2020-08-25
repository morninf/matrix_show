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
#endif

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

static void matrix_show( void )
{
	//行扫描
	uint8_t dat1;
	uint8_t dat2;
	uint16_t data1=0;
	uint16_t data2=0;
	for(int n = 0; n<4; n++)
	{
		//流转递进
		matrix_set_line((uint8_t)(n));
		for( int i = 0; i< 4;i++)
		{
			
			MATRIX_OE_H;
			MATRIX_LAT_L;
			
			if( i < 2)
			{
				dat1 = word_date[0][n*2+i];
				dat2 = word_date[0][2*n+8+i];
				data1 = dat1 | (dat2<<8);
				
				dat1 = word_date[0][n*2+16+i];
				dat2 = word_date[0][2*n+24+i];
				data2 = dat1 | (dat2<<8);
			}
			else
			{
				dat1 = word_date[1][n*2+i-2];
				dat2 = word_date[1][2*n+8+i-2];
				data1 = dat1 | (dat2<<8);
				
				dat1 = word_date[1][n*2+16+i-2];
				dat2 = word_date[1][2*n+24+i-2];
				data2 = dat1 | (dat2<<8);
			}
			for( int k = 0; k < 8; k++)
			{
				MATRIX_CLK_L;
				if( data1 & (1<<(15-k)))
				{
					MATRIX_R1_H;
				}
				else
				{
					MATRIX_R1_L;
				}
				if( data2 & (1<<(15-k)))
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
				if( data1 & (1<<k))
				{
					MATRIX_R1_H;
				}
				else
				{
					MATRIX_R1_L;
				}
				if( data2 & (1<<k))
				{
					MATRIX_R2_H;
				}
				else
				{
					MATRIX_R2_L;
				}
			MATRIX_CLK_H;
		}

		MATRIX_LAT_H;
		MATRIX_LAT_L;
		}
		MATRIX_OE_L;
		rt_hw_us_delay(1000);
	}
	
}

static void matrix_thread_entry(void *parameter)
{
	matrix_io_init();
	while(1)
	{
		//rt_kprintf("matrix show\n");
		matrix_show();
		rt_thread_mdelay(10);
	}
}

int matrix_thread_init(void)
{
	rt_err_t result = RT_EOK;
    rt_thread_t tid;
	
#ifdef RT_USING_HEAP
	tid = rt_thread_create(MATRIX_THREAD_NAME,
						   matrix_thread_entry, RT_NULL,
						   MATRIX_THREAD_STACK_SIZE, MATRIX_THREAD_PRIORITY, 10);
#else
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

