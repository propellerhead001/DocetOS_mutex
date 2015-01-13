#ifndef _MPU_H
#define _MPU_H_

#include "stm32f4xx.h" 

#define SIZE_FIELD 							1
#define ATTRIBUTE_FIELD 					16
#define ACCESS_FIELD 						24
#define ENABLE 								1
#define ATTRIBUTE_FLASH 					0x4
#define ATTRIBUTE_SRAM						0x5
#define ATTRIBUTE_PERIPHERAL 				0x3
#define PRIV_RW_UPRIV_RW 					3
#define PRIV_RO_UPRIV_NONE 					5
#define PRIV_RO_UPRIV_RO 					6
#define PRIV_RW_UPRIV_RO 					2
#define USE_PSP_IN_THREAD_MODE 				2
#define THREAD_MODE_IS_UNPRIVILIGED 		1
#define PSP_STACK_SIZE 						0x200
#define TOP_OF_THREAD_RAM 					0x10007FF0

void initialiseMPU(void);

#endif //_MPU_H_
