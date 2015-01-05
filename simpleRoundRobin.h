#ifndef __simpleRoundRobin_h__
#define __simpleRoundRobin_h__

#include "os.h"
#include "os_internal.h"
#include "task.h"

// How many tasks can this scheduler cope with?
#define SIMPLE_RR_MAX_TASKS 8

typedef enum {HIGH1, HIGH2, LOW1}OS_PriorityStates_t;

extern OS_Scheduler_t const simpleRoundRobinScheduler;
void __svc(SVC_TASK_WAIT) _task_wait(_OS_SVC_StackFrame_t * stackFramePointer);
void __svc(SVC_TASK_NOTIFY) _task_notify(_OS_SVC_StackFrame_t * stackFramePointer);
#endif /* __simpleRoundRobin_h__ */
