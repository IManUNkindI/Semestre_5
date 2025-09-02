#include <stm32f767xx.h>

// Declaracion de variables
	unsigned long pulso;

int main(){
	
	// Configuracion de salidas
	// Enable CLK GPIOB
	RCC->AHB1ENR |= (0x1<<1);

	// Configure PB(14,7,0) as output
	GPIOB->MODER |= (0x1<<0);
	GPIOB->MODER &= ~(0x1<<1);
	GPIOB->MODER |= (0x1<<28);
	GPIOB->MODER &= ~(0x1<<29);
	GPIOB->MODER |= (0x1<<14);
	GPIOB->MODER &= ~(0x1<<15);
	
	// Push-pull mode
	GPIOB->OTYPER &= ~(0x1<<0);
	GPIOB->OTYPER &= ~(0x1<<7);
	GPIOB->OTYPER &= ~(0x1<<14);
	
	// High Speed
	GPIOB->OSPEEDR &= ~(0x1<<0);
	GPIOB->OSPEEDR |= (0x1<<1);
	GPIOB->OSPEEDR &= ~(0x1<<14);
	GPIOB->OSPEEDR |= (0x1<<15);
	GPIOB->OSPEEDR &= ~(0x1<<28);
	GPIOB->OSPEEDR |= (0x1<<29);
	
	// Disable pull-up and pull-down resistors
	GPIOB->PUPDR |= (0x00000000);
	// Salidas configuradas
	
	
	// Configuracion de entradas
	// Enable CLK GPIOC
	RCC->AHB1ENR |= (0x1<<2);
	
	// High Speed
	GPIOC->OSPEEDR |= (0x1<<27);
	GPIOC->OSPEEDR &= ~(0x1<<26);
	
	// Enable pull-up resistor
	GPIOC->PUPDR |= (0x1<<27);
	GPIOC->PUPDR &= ~(0x1<<26);
	// Entradas configuradas

while(true){

		pulso = (GPIOC->IDR) & 0x2000;

		if(pulso == 0){

			GPIOB->BSRR |= (0x1<<7);
			GPIOB->BSRR |= (0x1<<16);
			GPIOB->BSRR |= (0x1<<30);			

		}else{
			
			GPIOB->BSRR |= (0x1<<23);
			GPIOB->BSRR |= (0x1<<0);
			GPIOB->BSRR |= (0x1<<14);
			
		}
	}
}