#ifndef _MATRIX_SHOW_H_
#define _MATRIX_SHOW_H_

#include <rtthread.h>


#define FONT_LEN							(16)
#define WORD_NUM_EACH_BOARD					(2)
#define ONE_WORD_SIZE_BYTE					(32)
#define ONE_BOARD_BYTE_NUM					(ONE_WORD_SIZE_BYTE*WORD_NUM_EACH_BOARD)

#define MATRIX_CASCADE_NUM					4

#define MATRIX_MAX_CASCADE_NUM				10
#define TEMP_BOARD_NUM						10

#define MATRIX_MAX_WORD						(2*MATRIX_MAX_CASCADE_NUM)

#define BLANK_BOARD_NUM						2

//20*64 * 1280byte
#define MATRIX_MAX_BUFFER_SIZE				((MATRIX_MAX_CASCADE_NUM+MATRIX_MAX_CASCADE_NUM)*ONE_BOARD_BYTE_NUM )

#define AS_MATRIX_MAX_LEN					((BLANK_BOARD_NUM+MATRIX_MAX_CASCADE_NUM+TEMP_BOARD_NUM)* WORD_NUM_EACH_BOARD *2)
#define BLANK_MATRIX_LEN					(BLANK_BOARD_NUM* WORD_NUM_EACH_BOARD *2)

#define SHOW_LEN 							(MATRIX_CASCADE_NUM * WORD_NUM_EACH_BOARD * 2)


#define TURE_WORD_NUM						(9)
#define TRUE_WORD_LEN						( TURE_WORD_NUM * 2 )

#define MATRIX_THREAD_NAME	"matrix"
#define MATRIX_THREAD_STACK_SIZE	1024
#define MATRIX_THREAD_PRIORITY		25
#define MATRIX_THREAD_PRIORITY		21

void set_matrix_data(int *index,int ifasc,uint8_t *data_from);
void set_show_len(int len);

void clean_matrix_buffer(void);

#endif
