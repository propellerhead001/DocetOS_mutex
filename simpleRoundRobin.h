#ifndef __simpleRoundRobin_h__
#define __simpleRoundRobin_h__

#include "os.h"
#include "os_internal.h"
#include "task.h"

// How many tasks can this scheduler cope with?
#define SIMPLE_RR_MAX_TASKS 8

typedef enum {HIGH1, HIGH2, HIGH3, LOW1, LOW2}OS_PriorityStates_t;

extern OS_Scheduler_t const simpleRoundRobinScheduler;
void __svc(SVC_TASK_WAIT) OS_wait(_OS_SVC_StackFrame_t * stackFramePointer, uint32_t check);
void __svc(SVC_TASK_NOTIFY) OS_notify(_OS_SVC_StackFrame_t * stackFramePointer);
uint32_t getNotifyCount(void);
#endif /* __simpleRoundRobin_h__ */
