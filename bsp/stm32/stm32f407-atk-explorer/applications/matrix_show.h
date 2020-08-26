#ifndef _MATRIX_SHOW_H_
#define _MATRIX_SHOW_H_

#include <rtthread.h>

#define ONE_WORD_SIZE_BYTE					(32)						
#define MATRIX_CASCADE_NUM					1
#define MATRIX_MAX_CASCADE_NUM				10
#define MATRIX_MAX_WORD						(2*MATRIX_MAX_CASCADE_NUM)
#define MATRIX_MAX_BUFFER_SIZE				(MATRIX_MAX_WORD * ONE_WORD_SIZE_BYTE)

#define MATRIX_THREAD_NAME	"matrix"
#define MATRIX_THREAD_STACK_SIZE	2048
#define MATRIX_THREAD_PRIORITY		21


#endif
