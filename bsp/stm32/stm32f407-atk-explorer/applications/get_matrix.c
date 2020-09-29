#include <string.h>

#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_flash.h>
#include "com_with_esp.h"
<<<<<<< HEAD
#include "matrix_show.h"
=======
>>>>>>> 4a7dcaa0f8ecc6170d325c6c8a2146e0594217e1
#include "get_matrix.h"

#ifdef _WIN32
#include <stdio.h> /* for putchar */
#endif
<<<<<<< HEAD
static int stm32_flash_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size)
{
    size_t i;

    if ((addr + size) > STM32_FLASH_END_ADDRESS)
    {
        return -1;
    }

    for (i = 0; i < size; i++, buf++, addr++)
    {
        *buf = *(rt_uint8_t *) addr;
    }

    return size;
}

static int utf8_to_unicode(uint8_t      *utf8, int len, uint16_t *unicode)
{
	int utf_idx = 0;
	int un_idx = 0;
	rt_kprintf("utf8_to_unicode\n");
	
	while(1)
	{
		rt_kprintf("prase ch: %02x\n",utf8[utf_idx]);
		//ASCII code
		if(utf8[utf_idx]<0x80 && utf8[utf_idx]>0x00)
		{
			unicode[un_idx++] = utf8[utf_idx++];
		}
		//chinese 3 bytes
		else if( utf8[utf_idx]>=0xe0 && utf8[utf_idx]<= 0xef)
		{
			unicode[un_idx] = (uint16_t)(utf8[utf_idx++]&0x0f)<<12;
			unicode[un_idx] += (uint16_t)(utf8[utf_idx++]&0x3f)<<6;
			unicode[un_idx++] += (uint16_t)utf8[utf_idx++]&0x3f;
		}
		else
		{
			rt_kprintf("error utf8 char\n");
			break;
		}
		rt_kprintf("unicode: %04x\n",unicode[un_idx-1]);
		if( utf_idx >= len-1)
			break;

	}
	return un_idx;
}

static int get_unicode_offset(uint8_t *unicode8,int len,uint16_t *unicode16)
{
	int idx8 = 0;
	int idx16 = 0;
	while(1)
	{
		unicode16[idx16] = 0;
		//ASCII code
		if(unicode8[idx8]<0x4e && unicode8[idx8]>0x00)
		{
			unicode16[idx16] = unicode8[idx8++];
			rt_kprintf("%04x ",unicode16[idx16]);
			idx16++;
		}
		//chinese 2 bytes
		else if( unicode8[idx8]>=0x4e && unicode8[idx8]<= 0x9f)
		{
			unicode16[idx16] = (uint16_t)unicode8[idx8++]<<8;
			unicode16[idx16] += (uint16_t)unicode8[idx8++];
			rt_kprintf("%04x ",unicode16[idx16]);
			idx16++;
		}
		if( idx8 >= len-1)
			break;

	}
	return idx16;
=======

static uint16_t utf8_to_unicode(uint8_t      *utf8)
{
	
}

static int get_unicode_offset(uint16_t uni)
{
>>>>>>> 4a7dcaa0f8ecc6170d325c6c8a2146e0594217e1


}

<<<<<<< HEAD
static uint16_t get_gbk_number(uint16_t unicode)
{
	uint8_t buf[2] = {0};
	uint16_t ret = 0;
	
	if(unicode<0x4e && unicode>0x00)
	{
		return unicode;
	}
	stm32_flash_read(FONT_FLASH_ADDRESS+2*unicode,buf,2);
	ret =  buf[1]<<8;
	ret += buf[0];
	return ret;
}

void get_matrix(int type,uint8_t *data_in,int len)
{
	uint16_t unicode[100]={0};
	uint8_t matrix_data_buf[32];
	int index = 0;
	int unicode_len = 0;
	int offset_flash=0;
	uint16_t gbk_num;
	int show_len = 0;
	clean_matrix_buffer();
	if( type == code_type_gbk)
	{
		int i =0;
		set_show_len(len);
		while(1)
		{
			//ASCII 
			if( data_in[i] < 0x81 && data_in[i]>0x00)
			{
				offset_flash = data_in[i]*16;
				stm32_flash_read(ASC_FLASH_ADDRESS+offset_flash,matrix_data_buf,16);
				set_matrix_data(&index,1,matrix_data_buf);
				i+=1;
			}
			else if (data_in[i] >= 0x81 && data_in[i]<=0xfe)//hanzi
			{
				offset_flash = (94*(unsigned int)(data_in[i]-0xa0-1)+(data_in[i+1]-0xa0-1))*32;
				stm32_flash_read(H2K16_FLASH_ADDRESS+offset_flash,matrix_data_buf,32);
				set_matrix_data(&index,0,matrix_data_buf);
				i+=2;
			}
			if( i >= len-1)
				break;
		}
	}
	else
	{
		if(type == code_type_utf8)
		{
			unicode_len = utf8_to_unicode(data_in,len,unicode);
		}
		else
		{
			unicode_len = get_unicode_offset(data_in,len,unicode);
		}
		
		rt_kprintf("\ngbk: len: %d\n",unicode_len);

		for(int i = 0;i<unicode_len;i++)
		{
			gbk_num = get_gbk_number(unicode[i]);
			rt_kprintf("%04x ",gbk_num);
			
			//ASCII 
			if( gbk_num <= 0xA0)
			{
				offset_flash = gbk_num*16;
				stm32_flash_read(ASC_FLASH_ADDRESS+offset_flash,matrix_data_buf,16);
				set_matrix_data(&index,1,matrix_data_buf);
				show_len +=1;
			}
			else//hanzi
			{
				offset_flash = (94*(unsigned int)((gbk_num>>8)-0xa0-1)+((gbk_num&0x00ff)-0xa0-1))*32;
				stm32_flash_read(H2K16_FLASH_ADDRESS+offset_flash,matrix_data_buf,32);
				set_matrix_data(&index,0,matrix_data_buf);
				show_len +=2;
			}
		}
		set_show_len(show_len);
		rt_kprintf("\n");
=======
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
>>>>>>> 4a7dcaa0f8ecc6170d325c6c8a2146e0594217e1
	}
}










