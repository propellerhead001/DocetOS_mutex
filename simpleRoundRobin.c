#include "simpleRoundRobin.h"
#include "task.h"
#include <stm32f4xx.h>

/* This is an implementation of an extremely simple (and inefficient!) round-robin scheduler.

   An array of pointers to TCBs is declared, and when tasks are added they are inserted into
	 this array.  When tasks finish, they are removed from the array (pointers are set back to
	 zero).  When the scheduler is invoked, it simply looks for the next available non-zero
	 pointer in the array, and returns it.  If there are no non-zero TCB pointers in the array,
	 a pointer to the idle task is returned instead.
	 
	 The inefficiency in this implementation arises mainly from the way arrays are used for list
	 storage.  If the array is large and the number of tasks is small, the scheduler will spend
	 a lot of time looking through the array each time it is called. */

/* Prototypes (functions are static, so these aren't in the header file) */
OS_TCB_t const * simpleRoundRobin_scheduler(void);
void simpleRoundRobin_addTask(OS_TCB_t * const tcb);
void simpleRoundRobin_taskExit(OS_TCB_t * const tcb);
void simpleRoundRobin_wait(void * reason, uint32_t check);
void simpleRoundRobin_notify(void * reason);

static OS_TCB_t * tasks[SIMPLE_RR_MAX_TASKS] = {0};
static OS_TCB_t * high[SIMPLE_RR_MAX_TASKS] = {0};
static OS_PriorityStates_t priorityState = HIGH1;
static uint32_t notifyCount = 0;

/* Scheduler block for the simple round-robin */
OS_Scheduler_t const simpleRoundRobinScheduler = {
	.preemptive = 1,
	.scheduler_callback = simpleRoundRobin_scheduler,
	.addtask_callback = simpleRoundRobin_addTask,
	.taskexit_callback = simpleRoundRobin_taskExit,
	.wait_callback = simpleRoundRobin_wait,
	.notify_callback = simpleRoundRobin_notify
};


/* Round-robin scheduler callback */
static OS_TCB_t const * simpleRoundRobin_scheduler(void) {
	static int i = 0;
	switch(priorityState){
		case HIGH1 : priorityState = LOW1;
		break;
		case HIGH2 : priorityState = LOW2;
		break;
		case HIGH3 : priorityState = HIGH1;
		break;
		case LOW1 : priorityState = HIGH2;
		break;
		case LOW2 : priorityState = HIGH3;
		break;
	}
	
	// Clear the yield flag if it's set - we simply don't care
	OS_currentTCB()->state &= ~TASK_STATE_YIELD;
	//Check to see which priority tasks should be run
	if((priorityState == LOW1)||(priorityState == LOW2)){
		for (int j = 1; j <= SIMPLE_RR_MAX_TASKS; j++) {
			i = (i + 1) % SIMPLE_RR_MAX_TASKS;
			if (tasks[i] != 0) {
				OS_TCB_t * task = tasks[i];
					if(task->state & TASK_STATE_WAIT){//Do nothing if task is waiting
					}
					if(task->state & TASK_STATE_SLEEP){//If task is just asleep
						uint32_t ticks = OS_elapsedTicks();
						if(task->data <= ticks){//Check to see if it should be awake
							task->state = task->state & (~ TASK_STATE_SLEEP); //wake task
							return task;
						}
					}
					else{
						return task;
					}
			}
		}
	}
	else{
		for (int j = 1; j <= SIMPLE_RR_MAX_TASKS; j++) {
			i = (i + 1) % SIMPLE_RR_MAX_TASKS;
			if (high[i] != 0) {
				OS_TCB_t * task = high[i];
					if(task->state & TASK_STATE_WAIT){//Do nothing if task is waiting
					}
					if(task->state & TASK_STATE_SLEEP){//If task is just asleep
						uint32_t ticks = OS_elapsedTicks();
						if(task->data <= ticks){//Check to see if it should be awake
							task->state = task->state & (~ TASK_STATE_SLEEP); //wake task
							return task;
						}
					}
					else{
						return task;
					}
			}
		}
	}
	// No tasks in the list, so return the idle task
	return OS_idleTCB_p;
}

/* 'Add task' callback */
static void simpleRoundRobin_addTask(OS_TCB_t * const tcb) {
	uint32_t priority = tcb->priority;
	if(priority){
		for (int i = 0; i < SIMPLE_RR_MAX_TASKS; i++) {
			if (high[i] == 0) {
				high[i] = tcb;
				return;
			}
		}
	}
	else{
		for (int i = 0; i < SIMPLE_RR_MAX_TASKS; i++) {
			if (tasks[i] == 0) {
				tasks[i] = tcb;
				return;
			}
		}
	}
	// If we get here, there are no free TCB slots, so the task just won't be added
}

/* 'Task exit' callback */
static void simpleRoundRobin_taskExit(OS_TCB_t * const tcb) {
	// Remove the given TCB from the list of tasks so it won't be run again
	for (int i = 0; i < SIMPLE_RR_MAX_TASKS; i++) {
		if (tasks[i] == tcb ) {
			tasks[i] = 0;
		}
	}	
}
static void simpleRoundRobin_wait(void * reason, uint32_t check){
	if(check == notifyCount){
		OS_TCB_t * TCB = OS_currentTCB();
		TCB->data = (uint32_t) (reason);
		TCB->state |= TASK_STATE_WAIT; 	
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk ;
	}
}

static void simpleRoundRobin_notify(void * reason){
	static int i = 0;
	uint32_t iReason = (uint32_t) reason;
	for (int j = 1; j <= SIMPLE_RR_MAX_TASKS; j++) {
		i = (i + 1) % SIMPLE_RR_MAX_TASKS;
		if (tasks[i] != 0) {
			OS_TCB_t* TCB = tasks[i];
			if(TCB->state & TASK_STATE_WAIT){
				if(TCB->data == iReason){
					TCB->data = 0;
					TCB->state &= (~TASK_STATE_WAIT);
				}					
			}
		}
		if (high[i] != 0) {
			OS_TCB_t* TCB = tasks[i];
			if(TCB->state & TASK_STATE_WAIT){
				if(TCB->data == iReason){
					TCB->data = 0;
					TCB->state &= (~TASK_STATE_WAIT);
				}					
			}
		}
	}
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk ;
	notifyCount++;
}

void _svc_OS_wait(_OS_SVC_StackFrame_t * stackFramePointer, uint32_t  check){
	simpleRoundRobin_wait(((void *) stackFramePointer->r0),stackFramePointer->r1);
}

void _svc_OS_notify(_OS_SVC_StackFrame_t * stackFramePointer){
	simpleRoundRobin_notify(((void *) stackFramePointer->r0));
}

uint32_t getNotifyCount(){
	return notifyCount;
}
