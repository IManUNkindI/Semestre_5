#include "stm32f7xx.h"
#include <stdio.h>
#define FREQTRABAHB 12000000

// Fr 12Mhz 
// Retardo 50ms
void SysTick_Init(void);
void SysTick_Wait(uint32_t);
void SysTick_Wait1ms(uint32_t);	


void SysTick_Init(void){
	SysTick->LOAD = 0xFFFFFF;
	SysTick->CTRL = 0x5;
}

void SysTick_Wait(uint32_t n){
	SysTick->LOAD = n-1;
	SysTick->VAL = 0;
	while((SysTick->CTRL&(1<<16))== 0){}
}

void SysTick_Wait1ms(uint32_t delay){
 for(uint32_t i=0; i<delay; i++){
 SysTick_Wait(FREQTRABAHB/1000);
 }
}
