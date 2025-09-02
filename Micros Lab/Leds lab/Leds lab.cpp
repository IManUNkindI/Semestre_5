#include <stm32f767xx.h>

unsigned long BTNUSER;

int main(){
	RCC->AHB1ENR|=(0x1<<0);
	RCC->AHB1ENR|=(0x1<<2);
	RCC->AHB1ENR|=(0x1<<5);
	
	GPIOA->MODER|=(0X55555515);
	GPIOA->MODER &=~(0XAAAAAA2A);
	GPIOC->MODER|=(0X51555555);
	GPIOC->MODER &=~(0XA2AAAAAA);
	GPIOF->MODER|=(0X1<<0);
	GPIOF->MODER &=~(0X1<<1);
	
	GPIOA->OTYPER &= ~(0X0000);
	GPIOC->OTYPER &= ~(0X0000);
	
	GPIOA->PUPDR |=(0XAAAAAA2A);
	GPIOA->PUPDR &=~(0X55555515);
	GPIOC->PUPDR |= (0X51555555);
	GPIOC->PUPDR &=~(0XA2AAAAAA);
	GPIOF->PUPDR |= (0X1<<1);
	GPIOF->PUPDR &=~(0X1<<0);
	
	while(true){
		
		if(BTNUSER != 0){
		GPIOF->BSRR = GPIO_BSRR_BS0;
			for (int i=0;i<16;i++){
				if(i==3){
					i=4;}
				GPIOA->BSRR = (1<<i);
				for (int delay = 0;	delay < 1000000; delay++);
				GPIOA->BSRR = (0X1<<(i+16));
			}
			/*for (int i=16; 1>0; i--){
				if(i==5){
					i=4;}
				GPIOA->BSRR = (1<<i);
				for (int delay = 0; delay < 1000000; delay++);
				GPIOA->BSRR = (0X1<<(i+16));
			}*/

		}
				
	}
}
