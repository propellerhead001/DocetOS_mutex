#include "mutex.h"

void init_mutex(volatile OS_mutex_t * mutex, uint32_t canPromote){
	mutex->counter = 0;
	mutex->holder = 0;
	if(canPromote) mutex->promote = 1;
	else mutex->promote = 0;
}
//Allows tasks to aquire a mutex on a resource, if a low priority task requests a resource more than 10
//times unsuccesfully it is promoted to high priority until it succesfully grabs the mutex.
uint32_t OS_mutex_acquire(volatile OS_mutex_t * mutex){
	uint32_t counter = 0;
	OS_TCB_t * requester = OS_currentTCB();
	uint32_t isHighPriority = requester->priority;
	uint32_t notifyCount = getNotifyCount();
	while(1){
		uint32_t holder = __LDREXW((uint32_t *) mutex);
		while(__STREXW(holder, (uint32_t *)mutex)) holder = __LDREXW((uint32_t *) mutex);
		if(mutex->holder == requester){
			mutex->counter++;
			return 1;
		}
		else if(mutex->holder == 0){
			if(! isHighPriority){
				demoteTask(requester);
			}
			mutex->holder = requester;
			mutex->counter++;
			return 1;
		}
		else{
			counter += mutex->promote;
			if(counter>10){
				promoteTask(requester);
			}
			OS_wait((void *)mutex, notifyCount);
		}
	}
}

void OS_mutex_release(volatile OS_mutex_t * mutex){
	if(mutex->counter) mutex->counter--;
	if(!(mutex->counter))mutex->holder = 0;
	OS_notify((void *)mutex);
}

