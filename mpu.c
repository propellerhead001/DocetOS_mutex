#include "mpu.h"
void initialiseMPU(){

	/*Thread code */
	MPU->RNR = 0;
	MPU->RBAR=0x20000000;
	MPU->RASR |= (PRIV_RO_UPRIV_RO<<ACCESS_FIELD)|(ATTRIBUTE_FLASH<<ATTRIBUTE_FIELD)|(17<<SIZE_FIELD)|ENABLE;
	
	/*Handler code*/
	MPU->RNR = 1;
	MPU->RBAR=0x20010000;
	MPU->RASR |=(PRIV_RO_UPRIV_RO<<ACCESS_FIELD)|(ATTRIBUTE_FLASH<<ATTRIBUTE_FIELD)|(17<<SIZE_FIELD)|ENABLE;
	
	/*Thread RAM */
	MPU->RNR = 2;
	MPU->RBAR=0x10000000;
	MPU->RASR |=(PRIV_RW_UPRIV_RW<<ACCESS_FIELD)|(ATTRIBUTE_SRAM<<ATTRIBUTE_FIELD)|(14<<SIZE_FIELD)|ENABLE;
	
	/*Handler RAM */
	MPU->RNR = 3;
	MPU->RBAR=0x2007C000;
	MPU->RASR |=(PRIV_RW_UPRIV_RO<<ACCESS_FIELD)|(ATTRIBUTE_SRAM<<ATTRIBUTE_FIELD)|(14<<SIZE_FIELD)|ENABLE;

	/*GPIO*/
	MPU->RNR = 4;
	MPU->RBAR=0x40000000;
	MPU->RASR=(PRIV_RW_UPRIV_RW<<ACCESS_FIELD)|(ATTRIBUTE_PERIPHERAL<<ATTRIBUTE_FIELD)|(100000<<SIZE_FIELD)|ENABLE;
	
	/*PERIPHERALS*/
	MPU->RNR = 5;
	MPU->RBAR= 0xE0000000;
	MPU->RASR=(PRIV_RW_UPRIV_RW<<ACCESS_FIELD)|(ATTRIBUTE_PERIPHERAL<<ATTRIBUTE_FIELD)|(0xEDF8<<SIZE_FIELD)|ENABLE;
	
	SCB->SHCSR = 0x01<<16;
	//MPU->CTRL = ENABLE;
	
	//__set_CONTROL(USE_PSP_IN_THREAD_MODE);
	//__set_PSP(TOP_OF_THREAD_RAM);
}
