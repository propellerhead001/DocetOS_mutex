#include "os.h"
#include <stdio.h>
#include "serial.h"
#include "simpleRoundRobin.h"
#include "mutex.h"
#include "sleep.h"

volatile OS_mutex_t printOuti;
volatile OS_mutex_t * printOut = &printOuti;

void task1(void const *const args) {
	while (1) {
		OS_mutex_acquire(printOut);
		printf("Message from Task 1\r\n");
		OS_mutex_release(printOut);
		OS_sleep(100);
	}
}

void task2(void const *const args) {
	while (1) {
		OS_mutex_acquire(printOut);
		printf("Message from Task 2\r\n");
		OS_mutex_release(printOut);
		OS_sleep(100);
	}
}
void task3(void const *const args) {
	while (1) {
		OS_mutex_acquire(printOut);
		printf("Message from Task 3\r\n");
		OS_mutex_release(printOut);
		OS_sleep(200);
	}
}
void task4(void const *const args) {
	while (1) {
		OS_mutex_acquire(printOut);
		printf("Message from Task 4\r\n");
		OS_mutex_release(printOut);
	}
}
void task5(void const *const args) {
	while (1) {
		OS_mutex_acquire(printOut);
		printf("Message from Task 5\r\n");
		OS_mutex_release(printOut);
		OS_sleep(100);
	}
}
void task6(void const *const args) {
	while (1) {
		OS_mutex_acquire(printOut);
		printf("Message from Task 6\r\n");
		OS_mutex_release(printOut);
	}
}
void task7(void const *const args) {
	while (1) {
		OS_mutex_acquire(printOut);
		printf("Message from Task 7\r\n");
		OS_mutex_release(printOut);
	}
}
void task8(void const *const args) {
	while (1) {
		OS_mutex_acquire(printOut);
		printf("Message from Task 8\r\n");
		OS_mutex_release(printOut);
	}
}

/* MAIN FUNCTION */

int main(void) {
	/* Initialise the serial port so printf() works */
	serial_init();

	printf("\r\nDocetOS Demo\r\n");

	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	__align(8)
	uint32_t stack1[64], stack2[64], stack3[64], stack4[64], stack5[64], stack6[64], stack7[64], stack8[64];
	OS_TCB_t TCB1, TCB2, TCB3, TCB4, TCB5, TCB6, TCB7, TCB8;
	
	init_mutex(printOut);
	

	/* Initialise the TCBs using the two functions above */
	OS_initialiseTCB(&TCB1, stack1+64, task1, NULL);
	OS_initialiseTCB(&TCB2, stack2+64, task2, NULL);
	OS_initialiseTCB(&TCB3, stack3+64, task3, NULL);
	OS_initialiseTCB(&TCB4, stack4+64, task4, NULL);
	OS_initialiseTCB(&TCB5, stack5+64, task5, NULL);
	OS_initialiseTCB(&TCB6, stack6+64, task6, NULL);
	OS_initialiseTCB(&TCB7, stack7+64, task7, NULL);
	OS_initialiseTCB(&TCB8, stack8+64, task8, NULL);
	
	TCB1.priority = 1;

	/* Initialise and start the OS */
	OS_init(&simpleRoundRobinScheduler);
	OS_addTask(&TCB1);
	OS_addTask(&TCB2);
	OS_addTask(&TCB3);
	OS_addTask(&TCB4);
	OS_addTask(&TCB5);
	OS_addTask(&TCB6);
	OS_addTask(&TCB7);
	OS_addTask(&TCB8);
	
	OS_start();
}
