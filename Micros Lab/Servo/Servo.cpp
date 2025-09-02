#include <stm32f767xx.h>

int main (){

	RCC->AHB1ENR|=1<<1;                                           // Puerto B
	GPIOB->MODER|=1<<1;                                           // Alternante PB0
	GPIOB->AFR[0]|=1<<1;                 						               // Funcion 2
	RCC->APB1ENR|=(1<<1);                                          // TIMER3
	TIM3->ARR=20000; 
	TIM3->PSC=16 - 1 ;                                               // Ftim -> 0.5 T -> 2seg
	TIM3->EGR|=0x1;
	TIM3->CCMR2|=0x6060;                                            // PWM 3
	TIM3->CCER|=0x100;
	TIM3->CR1|=0x1;
	//TIM3->CCR3=0;
	
	while(true){
		
		TIM3->CCR3=1500;
	}
}