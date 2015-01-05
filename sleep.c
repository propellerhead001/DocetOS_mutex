#include "sleep.h"
#include "os_internal.h"
#include "os.h"

void OS_sleep(uint32_t sleepPeriod){
	OS_TCB_t * current = OS_currentTCB();
	uint32_t elapsedTicks= OS_elapsedTicks();
	current->data = sleepPeriod + elapsedTicks;
	current->state = current->state|TASK_STATE_SLEEP;
	OS_yield();
}
