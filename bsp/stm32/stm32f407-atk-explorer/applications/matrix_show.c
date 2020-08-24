#include <rthw.h>

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

void matrix_thread_entry(void *parameter)
{
	while(1)
	{
		rt_kprintf("matrix show\n");
		rt_thread_mdelay(500);
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
	result = rt_thread_init(&finsh_thread,
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

