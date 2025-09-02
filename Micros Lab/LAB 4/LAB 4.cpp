#include <stm32f767xx.h>
#include "SysTick.h"
#include "Display_7x4.h"
#include "Timer.h"

int count[4];
int color[4];
int conteo = 1199;
int en = 0;
int ADC_IN;

int main(){

	SysTick_Init();
	Init7x4();
	
	Config_Timer3();
	Config_Timer7();
	
	//ADC1
	RCC->APB2ENR |= (0x1 << 8);
	RCC->AHB1ENR |= (0x1 << 0) | (0x1 << 1);
	GPIOA->MODER |= (0x1 << 7) | (0x1 << 6); //pa3
	GPIOA->MODER |= (0x1 << 14) | (0x0 << 15); //pa7
	ADC->CCR |= (0x1 << 16);
	ADC1->CR1 |= (0x1 << 24);
  ADC1->CR2 |= (0x1 << 0) | (0x1 << 1) | (0x0<<11) | (0x0<<10);
	ADC1->SMPR1 |= (0x7 << 16);
	ADC1->SQR3 |= (0x3 << 0);
	
	//EXTI PULSADOR PB8
	RCC->AHB1ENR |= (0x1<<1);
	RCC->APB2ENR |= (0x1<<14);
	SYSCFG->EXTICR[2] |= 0x1;
	EXTI->IMR |= (0x1<<8);
	EXTI->FTSR |= (0x1<<8);
	NVIC_EnableIRQ(EXTI9_5_IRQn); 
	NVIC_SetPriority(EXTI9_5_IRQn, 1);
	
	count[0] = 0;
	count[1] = 0;
	count[2] = 0;
	count[3] = 0;
	
	while(true){
		if(en == 1){
			TIM3->CR1 &= ~(0x1<<0);
			TIM7->CR1 |= (0x1<<0);
			GPIOA->ODR = (0x1<<7);
			
				// GRIS     VERDE      NARANJA     BLANCO	
			ADC1->CR2|=(1UL<<30);
			while((ADC1->SR &(1UL<<1))==0){
				ADC1->SR=0;
			}
			ADC_IN = (ADC1->DR);
			 
			if((502 <= ADC_IN) && (ADC_IN <= 510)){	//Blanco
				color[0] = 11;
				color[1] = 21;
				color[2] = 7;
				color[3] = 8;
			}else if((515 <= ADC_IN) && (ADC_IN <= 522)){	//Naranja
				color[0] = 23;
				color[1] = 19;
				color[2] = 5;
				color[3] = 6;
			}else if((460 <= ADC_IN) && (ADC_IN <= 465)){	//Verde
				color[0] = 31;
				color[1] = 13;
				color[2] = 3;
				color[3] = 4;
			}else if((260 <= ADC_IN) && (ADC_IN <= 265)){	//Gris
				color[0] = 16;
				color[1] = 27;
				color[2] = 1;
				color[3] = 2;
			}
			Mostrar(color);
			
		}else if(en == 0){
			ADC_IN = 0;
			
			TIM3->CR1 |= (0x1<<0);
			TIM7->CR1 &= ~(0x1<<0); 
			
			GPIOA->ODR = ~(0x1<<7);
			
			if(conteo==10000){
				conteo = 0;
			}
			count[0]= conteo/1000;
			count[1]= (conteo/100)%10;
			count[2]= (conteo/10)%10;
			count[3]= conteo%10;
			Mostrar(count);
		}
	}
}
extern "C"{
	void TIM3_IRQHandler(void){
		TIM3->SR &= ~(0x1<<0);
		conteo++;	
  }
	void TIM7_IRQHandler(void){
		TIM7->SR &= ~(0x1<<0);
		en = 0;	
  }
	void EXTI9_5_IRQHandler(void){
		en = 1;
		EXTI->PR |= (0x1<<8);
		Display(37);
	}
}