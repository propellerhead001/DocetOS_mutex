#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "task.h"
#include "OS.h"
#include "simpleRoundRobin.h"
#include <stm32f4xx.h>

typedef struct{
	OS_TCB_t * holder;
	uint32_t counter;
} OS_mutex_t;

void init_mutex(volatile OS_mutex_t * mutex);
uint32_t OS_mutex_acquire(volatile OS_mutex_t * mutex);
void OS_mutex_release(volatile OS_mutex_t * mutex);
#endif /* _MUTEX_H_ */
