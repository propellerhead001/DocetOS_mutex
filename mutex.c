#include "mutex.h"

void init_mutex(volatile OS_mutex_t * mutex){
	mutex->counter = 0;
	mutex->holder = 0;
}

uint32_t OS_mutex_acquire(volatile OS_mutex_t * mutex){
	uint32_t notifyCount = getNotifyCount();
	while(1){
		uint32_t holder = __LDREXW((uint32_t *) mutex);
		while(__STREXW(holder, (uint32_t *)mutex)) holder = __LDREXW((uint32_t *) mutex);
		OS_TCB_t * requester = OS_currentTCB();
		if(mutex->holder == requester){
			mutex->counter++;
			return 1;
		}
		else if(mutex->holder == 0){
			mutex->holder = requester;
			mutex->counter++;
			return 1;
		}
		else{
			OS_wait((void *)mutex, notifyCount);
		}
	};
}

void OS_mutex_release(volatile OS_mutex_t * mutex){
	if(mutex->counter) mutex->counter--;
	if(!(mutex->counter))mutex->holder = 0;
	OS_notify((void *)mutex);
}

