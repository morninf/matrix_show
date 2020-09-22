#ifndef _COM_WITH_ESP_H_
#define _COM_WITH_ESP_H_

#include <rtthread.h>



struct com_struct
{
	struct rt_semaphore rx_sem;
	uint8_t buffer[256];

};


















void com_printf(const char *fmt, ...);














#define COM_BUF_SIZE	1024


#define COM_THREAD_NAME		"com-thread"
#define COM_THREAD_STACK_SIZE	1024
#define COM_THREAD_PRIORITY		9


#define COM_DEVICE_NAME	"uart3"
#endif

