#include <stm32f7xx.h>
#define FREQTRABAHB 16000000

unsigned long int BAUDRATE,Over8;

void SysTick_Init(void);
void SysTick_Wait(uint32_t n);
void SysTick_Wait1ms(uint32_t delay);
int envioDato(int value);	
char rx;

int main(void)
{
	RCC->AHB1ENR |=RCC_AHB1ENR_GPIOGEN;
	RCC->APB1ENR |= 0x40000;
	GPIOG->MODER |=0x0;
	GPIOG->IDR |=0x1; 
	
	//USART 3 PD8(TX) y PD9(RX)
  RCC->AHB1ENR |=RCC_AHB1ENR_GPIODEN;	
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	Over8=0;
	BAUDRATE= 0x683; //9600
	GPIOD->MODER |= 0XA0000; 
	GPIOD->AFR[1] |=0X77;
	USART3->BRR |= BAUDRATE;
	USART3->CR1 |=(USART_CR1_TE | USART_CR1_UE | USART_CR1_RE | USART_CR1_RXNEIE | Over8<<15);
	NVIC_EnableIRQ(USART3_IRQn);

	SysTick_Init();
	
	int BT1;	
	
	while(1){
		BT1=GPIOG->IDR&(0x1);
		
		if(BT1!=0){
		envioDato('M');
		SysTick_Wait1ms(2000);	
		}
		if(BT1==0){
		envioDato('F');
		SysTick_Wait1ms(2000);	
		}
}}

int envioDato(int value) { 
   USART3->TDR = value;
   while(!(USART3->ISR & 1<<7)); 
   return 0;
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