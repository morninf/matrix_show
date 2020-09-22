#include <string.h>

#include <rthw.h>
#include <rtdevice.h>
#include <board.h>

#include "com_with_esp.h"

#ifdef _WIN32
#include <stdio.h> /* for putchar */
#endif

/* matrix thread */
#ifndef RT_USING_HEAP
static struct rt_thread com_thread;
ALIGN(RT_ALIGN_SIZE)
static char com_thread_stack[MATRIX_THREAD_STACK_SIZE];
struct com_struct _com_struct;
#endif

struct com_struct *com_struct;

static rt_device_t com_device = RT_NULL;


static rt_err_t com_rx_ind(rt_device_t dev, rt_size_t size)
{
    RT_ASSERT(com_struct != RT_NULL);

    /* release semaphore to let finsh thread rx data */
    rt_sem_release(&com_struct->rx_sem);

    return RT_EOK;
}

static uint8_t com_getchar(void)
{
#ifdef RT_USING_DEVICE
#ifdef RT_USING_POSIX
    return getchar();
#else
    uint8_t ch = 0;

    RT_ASSERT(com_struct != RT_NULL);
    while (rt_device_read(com_device, -1, &ch, 1) != 1)
        rt_sem_take(&com_struct->rx_sem, RT_WAITING_FOREVER);

    return (uint8_t)ch;
#endif
#else
    extern char rt_hw_console_getchar(void);
    return rt_hw_console_getchar();
#endif
}

static void com_thread_entry(void *parameter)
{
    rt_device_t device;
	rt_err_t ret = 0;
	uint8_t ch;
    /* find and open new uart device */
    device = rt_device_find(COM_DEVICE_NAME);
    if (device != RT_NULL)
    {
        if (com_device != RT_NULL)
        {
            /* close old uart device */
            rt_device_close(com_device);
        }

        /* set new uart device */
        ret = rt_device_open(device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM | RT_DEVICE_FLAG_INT_RX);
        com_device = device;
    }
	
	rt_device_set_rx_indicate(com_device, com_rx_ind);
	while(1)
	{
		ch = com_getchar();
		rt_kprintf("%c ",ch);
	}
}


void com_printf(const char *fmt, ...)
{
    va_list args;
    rt_size_t length;
    static char rt_log_buf[COM_BUF_SIZE];

    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the rt_log_buf, we have to adjust the output
     * length. */
    length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > COM_BUF_SIZE - 1)
        length = COM_BUF_SIZE - 1;

    rt_uint16_t old_flag = com_device->open_flag;

    com_device->open_flag |= RT_DEVICE_FLAG_STREAM;
    rt_device_write(com_device, 0, rt_log_buf, length);
    com_device->open_flag = old_flag;
    va_end(args);
}

int com_thread_init(void)
{
	rt_err_t result = RT_EOK;
    rt_thread_t tid;
	
#ifdef RT_USING_HEAP
	com_struct = (struct com_struct *)rt_calloc(1, sizeof(struct com_struct));
	tid = rt_thread_create(COM_THREAD_NAME,
						   com_thread_entry, RT_NULL,
						   COM_THREAD_STACK_SIZE, COM_THREAD_PRIORITY, 10);
#else
	com_struct = &_com_struct;
	result = rt_thread_init(&com_thread,
							COM_THREAD_NAME,
							com_thread_entry, RT_NULL,
							&com_thread_stack[0], sizeof(com_thread_stack),
							COM_THREAD_PRIORITY, 10);
#endif /* RT_USING_HEAP */

	rt_sem_init(&(com_struct->rx_sem), "comrx", 0, 0);

    if (tid != NULL && result == RT_EOK)
        rt_thread_startup(tid);
    return 0;


}
INIT_APP_EXPORT(com_thread_init);


