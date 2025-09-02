#include <stm32f767xx.h>
#include "SysTick.h"

char matriz[4][4] = {'1','2','3','A',
										 '4','5','6','B',
										 '7','8','9','C',
										 '*','0','#','D'};
int count;
char tecla;
										 
unsigned long int Over8;
unsigned long int BAUDRATE;
										 
int USART_SendChar(int value);

int main(){
	SysTick_Init();
	RCC->AHB1ENR |= (0x1<<3);
	GPIOD->MODER |= 0x5500;
	GPIOD->OTYPER |= 0x0F;
	GPIOD->OSPEEDR |= 0xAAAA;
	GPIOD->PUPDR |= 0x00AA;
	
	RCC->APB2ENR |= (0x1<<14);
	SYSCFG->EXTICR[0] |= 0x3333;
	EXTI->IMR |= 0xF;
	EXTI->RTSR |= 0xF;
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);		
	
	//USART 3 PD8(TX) y PD9(RX) 
	RCC->APB1ENR |= (0x1<<18); //USART3
	Over8=0;
	BAUDRATE= 0x683;           //9600
	GPIOD->MODER |= 0XA0000;   //Alternante 8 y 9 
	GPIOD->AFR[1] |=0X77;      //Alternant funtion high register (0111 x 8 y 9)
	USART3->BRR |= BAUDRATE;
	USART3->CR1 |= 0xF;				 //Transmision, recepcion, stop mode, enable USART
	USART3->CR1 |= (0x1<<5);	 //Interrupcion (recepcion)
	USART3->CR1 |= (0x1<<15);	 //OVER8
	NVIC_EnableIRQ(USART3_IRQn);
	
	while(true){
		
		USART_SendChar(tecla);
		tecla = 0;
		GPIOD->BSRR |= 0xFFEF0010;
		count = 0;
		SysTick_Wait1ms(10);
		GPIOD->BSRR |= 0xFFDF0020;
		count = 1;
		SysTick_Wait1ms(10);
		GPIOD->BSRR |= 0xFFBF0040;
		count = 2;
		SysTick_Wait1ms(10);
		GPIOD->BSRR |= 0xFF7F0080;
		count = 3;
		SysTick_Wait1ms(10);
	}
}
 int USART_SendChar(int value) { 
   USART3->TDR = value;
   while(!(USART3->ISR & USART_ISR_TXE));
   return 0;
 }
extern "C"{
	void EXTI0_IRQHandler (void){
		tecla=matriz[count][0];

		SysTick_Wait1ms(250);
		EXTI->PR |= (0x1<<0);
	}
	void EXTI1_IRQHandler (void){
		tecla=matriz[count][1];

		SysTick_Wait1ms(250);
		EXTI->PR |= (0x1<<1);
	}
	void EXTI2_IRQHandler (void){
		tecla=matriz[count][2];
		SysTick_Wait1ms(250);
		EXTI->PR |= (0x1<<2);
	}
	void EXTI3_IRQHandler (void){
		tecla=matriz[count][3];

		SysTick_Wait1ms(250);
		EXTI->PR |= (0x1<<3);
	}
	void USART3_IRQHandler(void){
	}
}
