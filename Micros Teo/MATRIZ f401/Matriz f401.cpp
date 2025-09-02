#include <stm32f401xc.h>

unsigned long int BTNUSER;

void SysTick_Init(void);
void SysTick_Wait(uint32_t n);
void SysTick_Wait1ms(uint32_t delay); // mS
int main(){
	SysTick_Init();
	
	RCC->AHB1ENR |= 0xC;
	GPIOD->MODER |= 0x55555555;
	GPIOD->OTYPER |= 0x0000;
	GPIOD->OSPEEDR |= 0xAAAAAAAA;
	GPIOD->PUPDR |= 0x00000000;
	
	//EXIT
	RCC->APB2ENR |= (0x1<<14);
	SYSCFG->EXTICR[3] |= 0x20;
	EXTI->IMR |= (0x1<<13);
	EXTI->RTSR |= (0x1<<13);
  NVIC_EnableIRQ(EXTI15_10_IRQn); //13
	while(true){
		GPIOD->BSRR |= 0xFFFF0000;
	}
}
void SysTick_Init(void){
	SysTick->LOAD =0xFFFFFF;
	SysTick->CTRL =0x0000005;
}

void SysTick_Wait(uint32_t n){
	SysTick->LOAD = n-1;
	SysTick->VAL =0;
	while((SysTick->CTRL&0x00010000)==0);
}

void SysTick_Wait1ms(uint32_t delay){ // mS
	for(uint32_t i=0; i<delay ; i++){
	SysTick_Wait(16000);
	}
}
extern "C"{
	void EXTI15_10_IRQHandler(void){
		//GPIOD->BSRR|= 0xFE0001FF;
		GPIOD->ODR |= 0xFEFF;
		//GPIOD->BSRR|= 0xF7F0080F;
		GPIOD->ODR |= 0xFD01;
		SysTick_Wait1ms(20);
		//GPIOD->BSRR|= 0xF6FE09FE;
		SysTick_Wait1ms(5000);
		EXTI->PR |= (1<<13); //BAJA LA BANDERA, PONE UN 0 PARA AVISAR QUE TERMINO LA INTERRUPCION
	}
}
	