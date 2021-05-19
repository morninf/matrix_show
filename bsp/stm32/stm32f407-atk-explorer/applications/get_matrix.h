#ifndef _GET_MATRIX_H_
#define _GET_MATRIX_H_

#include <rtthread.h>

enum 
{
	code_type_utf8,
	code_type_unicode,	
	code_type_gbk
};

#define ASC_FLASH_ADDRESS		((uint32_t)0x08080000)
#define FONT_FLASH_ADDRESS		((uint32_t)0x080A0000)
#define H2K16_FLASH_ADDRESS		((uint32_t)0x080C0000)


void get_matrix(int type,uint8_t *data_in,int len);


/*
#define ASC_FLASH_ADDRESS		ADDR_FLASH_SECTOR_8
#define FONT_FLASH_ADDRESS		ADDR_FLASH_SECTOR_9
#define H2K16_FLASH_ADDRESS		ADDR_FLASH_SECTOR_10
*/

#endif
