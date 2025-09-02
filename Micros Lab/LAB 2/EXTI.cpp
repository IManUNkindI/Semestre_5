#include "stm32f7xx.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>
void SysTick_Init(void);
void SysTick_Wait(uint32_t);
void SysTick_Wait1ms(uint32_t);

int num[16]={0x5F,0x6,0x3B,0x2F,0x66,0x6D,0x7D,0x27,0x7F, 0x6F, 0x77,0x7C,0x59,0x3E,0x79,0x71}; // CONTEO DE 0 A F
int frec[4]={0x77,0x7C,0X59,0X3E}; // A, B, C ,D CAMBIO DE FRECUENCIA
int cont = 0;
int a;
int frecuencia;

int main(){
RCC->CR|=0x30003;
RCC->CFGR|=0x0;    // HABILITAR HSI CLOCK
//RCC->CFGR|=0x1; //HABILITAR HSE CLOCK 

// EXTI PULSADOR PA4
RCC->AHB1ENR |= 0x9;
RCC->APB2ENR |=(1<<14);
SYSCFG->EXTICR[1] |= 0x0;
EXTI->IMR |= (1<<4);
EXTI->RTSR |= (1<<4);
NVIC_EnableIRQ(EXTI4_IRQn);
SysTick_Init();		
	
//DISPLAY PD0-PD6
GPIOD->MODER|=0x1555;
GPIOD->OTYPER|=0x0;
GPIOD->OSPEEDR|=0x0;
GPIOD->PUPDR|=0x0;
		
//PG0, PG1, PULSADORES 
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
GPIOG->MODER |= 0x0;
GPIOG->OTYPER |= 0x0;
GPIOG->OSPEEDR |= 0x0;
GPIOG->PUPDR |= 0x0;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= 0x0;
	GPIOA->OTYPER &= 0x0;
	GPIOA->OSPEEDR &= 0x0;
	GPIOA->PUPDR |= 0x0;
	
while(1){
			switch(cont){
				case 0:
					GPIOD->ODR=frec[0];
					frecuencia = 250; // 4Hz
					break;
				case 1:
					GPIOD->ODR=frec[1];
					frecuencia = 1000; //  1Hz
					break;
				case 2:
					GPIOD->ODR=frec[2];
					frecuencia = 2000; //  0.5Hz
					break;
				case 3:
					GPIOD->ODR=frec[3];
					frecuencia = 4000;  //  0.25Hz
					break;
				case 4:
					cont=0;
					break;
			}
			//CONTADOR ASCENDENTE
			while(((GPIOG->IDR)&(1<<0))==0x1){
				for(int i = a; i < 16;i++){
					GPIOD->ODR=num[i];
					SysTick_Wait1ms(frecuencia);
					if(i==15){
						i=0;
						cont=0;
					}
				}
			}
			//CONTADOR DESCENDENTE
			while(((GPIOG->IDR)&(1<<1))==0x2){
				for(int i = a; i >= 0;i--){
					GPIOD->ODR=num[i];
					SysTick_Wait1ms(frecuencia);
					if(i==0){
						i=16;
						cont=0;
					}
				}
			}
	}
}
extern "C"{
	void EXTI4_IRQHandler(void){
		cont++;
		a = (rand()%15);
		SysTick_Wait1ms(500);
		EXTI->PR |= (1<<4); //BAJA LA BANDERA, PONE UN 0 PARA AVISAR QUE TERMINO LA INTERRUPCION
	}
}

void SysTick_Init(void){
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->CTRL = 0x00000005;
}

void SysTick_Wait(uint32_t n){
	SysTick->LOAD = n-1;
	SysTick->VAL = 0;
	while((SysTick->CTRL&(1<<16))== 0){};
}
// HSI CLOCK
void SysTick_Wait1ms(uint32_t delay){
	for(uint32_t i=0; i<delay;i++){
		SysTick_Wait(16000);
	}
}
// AQUI SE HABILITA EL HSE CLOCK
/*
void SysTick_Wait1ms(uint32_t delay){
	for(uint32_t i=0; i<delay;i++){
		SysTick_Wait(8000);
	}
}
*/