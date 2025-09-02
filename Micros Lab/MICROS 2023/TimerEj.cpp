/*#include "stm32f7xx.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>

unsigned long int BAUDRATE,Over8;
int DRNK[4] = {0x3E,0x5,0x56,0x52};
int HBRG[4] = {0x7C,0x5};
int display[27]={0x77,0x7C,0x59,0x3E,0x79,0x71};
char dato;


void SysTick_Init(void);
void SysTick_Wait(uint32_t);
void SysTick_Wait1ms(uint32_t);

void SysTick_Init(void){
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->CTRL = 0x00000005;
}

void SysTick_Wait(uint32_t n){
	SysTick->LOAD = n-1;
	SysTick->VAL = 0;
	while((SysTick->CTRL&(1<<16))== 0){};
}

void SysTick_Wait1ms(uint32_t delay){
	for(uint32_t i=0; i<delay;i++){
		SysTick_Wait(16000);
	}
}

int main(void){
	// Display
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->APB1ENR |= (1<<3); // HABILITA TIM5
	GPIOD->MODER |= 0x1555;
	GPIOD->OTYPER &= 0x0;
	GPIOD->OSPEEDR &= 0x0;
	GPIOD->PUPDR |= 0x1555;
	
	TIM5 ->ARR = 49999;
	TIM5->PSC = 319;
	TIM5->CR1 = (0<<4)|(1<<0); // DIR 0 & CEN 1
	TIM5->DIER = (1<<0); // UE INTERRUPCION ENABLE
	
	
	SysTick_Init();

while(1){
	


}}*/

#include <stm32f767xx.h>
#include <stdio.h>
#define FREQTRABAHB 16000000


//100 HZ
volatile int ARR=1999;
volatile int PSC=159;
int contador=0;


void SysTick_Init(void);
void SysTick_Wait(uint32_t n);
void SysTick_Wait1ms(uint32_t delay);

void SysTick_Inicial(void);
void SysTick_Delay(unsigned long t);

void SysTick_Inicial(void){
	SysTick->LOAD = 0x493E;     //se habilita el tiempo a un milisegundo 18.75/1000
	SysTick->VAL = 0;
	while((SysTick->CTRL&0x00010000)==0);
}
void SysTick_Delay(unsigned long t){
	for(; t>0; t--){
		SysTick_Inicial();
	}
}

int main (void)
{
	SysTick_Config(0x493E);
	//MOTOR>PPD3 CON SEÑAL PWM
	RCC->AHB1ENR =0xFF;
	GPIOD->MODER|=0x40;
	GPIOA->AFR[0]=0X2222;
	
	//PRENDER TIMMER 5
	RCC->APB1ENR|=0x8; 
	TIM5->ARR=ARR;
	TIM5->PSC=PSC;//700
	TIM5->DIER|=0x1;
	TIM5->EGR=0x1;
	TIM5->CCMR1=0x6060;
	TIM5->CCMR2=0x6060;
  TIM5->CCER|=(1<<0)| (1<<4) | (1<<8) | (1<<12);
	TIM5->CR1|=0x1 | (0<<4);

	
	
while(1){	
	if(contador==0){
			TIM5->CCR1=ARR*0;
		}else
		if(contador==1){
			TIM5->CCR1=ARR*0.1;
		}else
		if(contador==2){
			TIM5->CCR1=ARR*0.2;
		}else
		if(contador==3){
			TIM5->CCR1=ARR*0.3;
		}else
		if(contador==4){
			TIM5->CCR1=ARR*0.4;
		}else
		if(contador==5){
			TIM5->CCR1=ARR*0.5;
		}else
		if(contador==6){
			TIM5->CCR1=ARR*0.6;
		}else
		if(contador==7){
			TIM5->CCR1=ARR*0.7;
		}else
		if(contador==8){
			TIM5->CCR1=ARR*0.8;
		}else
		if(contador==9){
			TIM5->CCR1=ARR*0.9;
		}else
		if(contador==10){
			TIM5->CCR1=ARR*1;
		}else
		if(contador>10){
		TIM5->CCR1=ARR*0;
		}
	}
}

void SysTick_Init(void){
  SysTick->LOAD = 0x00FFFFFF;
	SysTick->CTRL = 0x00000005;
}

void SysTick_Wait(uint32_t n){
  SysTick->LOAD = n-1;
	SysTick->VAL = 0;
	while((SysTick->CTRL&0x00010000)==0);
}

void SysTick_Wait1ms(uint32_t delay){
 for(uint32_t i=0; i<delay; i++){
 SysTick_Wait(FREQTRABAHB/1000);
 }
}