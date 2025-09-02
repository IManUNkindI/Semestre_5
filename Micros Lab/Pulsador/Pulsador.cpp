#include <stm32f767xx.h>

unsigned long BTN1;

int main()
{
	RCC->AHB1ENR |= (0x1<<1);
	
	GPIOB->MODER |= (0x1<<14);
	GPIOB->MODER &= ~(0x1<<15);
	
	GPIOB->OTYPER &= ~(0x1<<7);
	
	GPIOB->OSPEEDR &= ~(0x1<<14);
	GPIOB->OSPEEDR |= (0x1<<15);
	
	GPIOB->PUPDR &= ~(0x1<<14);
	GPIOB->PUPDR &= ~(0x1<<15);
	
	RCC->AHB1ENR |= (0x1<<2);
	
	GPIOC->OSPEEDR &= ~(0x1<<26);
	GPIOC->OSPEEDR |= (0x1<<27);
	
	GPIOC->PUPDR |= (0x1<<27);
	GPIOC->PUPDR &= ~(0x1<<26);
	
	while(true)
	{
		//GPIOB->ODR |= (0x1<<7);
		BTN1 = (GPIOC->IDR) & 0x2000;
		if(BTN1==0)
		{
			GPIOB->BSRR |= (0x1<<23);
		}else
		{
			GPIOB->BSRR |= (0x1<<7);
		}
	}
}