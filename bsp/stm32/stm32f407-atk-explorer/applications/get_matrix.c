#include <string.h>

#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_flash.h>
#include "com_with_esp.h"
#include "get_matrix.h"

#ifdef _WIN32
#include <stdio.h> /* for putchar */
#endif

static uint16_t utf8_to_unicode(uint8_t      *utf8)
{
	
}

static int get_unicode_offset(uint16_t uni)
{


}

static uint16_t get_gbk_number(int offset)
{

}

int get_matrix(int type,uint8_t *utf8, uint8_t* matrix_dat,int *len)
{
	int index = 0;
	if( type == code_type_utf8)
	{
		

	
	}
	else if( type == code_type_unicode )
	{
	}
	else
	{
	}
}










