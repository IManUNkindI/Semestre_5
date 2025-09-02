#include <stm32f767xx.h>
#include "7-Segmentos.h"

//declaracion de variables
unsigned long SWT1;
unsigned long SWT2;
unsigned long BTNUSER;
unsigned long BTNPB8;
bool breaker = false;
bool start = false;
int count = 0;
int enablec = 0;
int delaym = 20;
int mode = 0;

//Metodos del selsccionador
void NamesHalloween();
void LedsHalloween();
void ContadorDisplay();
void Motor(int);
void Display(int);

//Metodo para delay
void delay(int32_t k){
	int32_t i, j ;
	for(i = 0; i < k ; i++){
		for(j = 0; j < 1000; j++);
	}
}

int main(){
	//Configurar Salidas
		//Habilitar CLK GPIOB, CLK GPIOD, CLK GPIOE, CLK GPIOF, CLK GPIOA, CLK GPIOC
	RCC->AHB1ENR |= (0x1<<1);
	RCC->AHB1ENR |= (0x1<<3);
	RCC->AHB1ENR |= (0x1<<4);
	RCC->AHB1ENR |= (0x1);
	RCC->AHB1ENR |= (0x1<<5);
	RCC->AHB1ENR |= (0x1<<2);
	
		//Configurar Salidas
	
	GPIOA->MODER|=(0X55555515);
	GPIOA->MODER &=~(0XAAAAAA2A);
	
	GPIOB->MODER |= (0x1<<0);
	GPIOB->MODER &= ~(0x1<<1);
	GPIOB->MODER |= (0x1<<28);
	GPIOB->MODER &= ~(0x1<<29);
	GPIOB->MODER |= (0x1<<14);
	GPIOB->MODER &= ~(0x1<<15);

	GPIOC->MODER|=(0X51555555);
	GPIOC->MODER &=~(0XA2AAAAAA);
	
	GPIOD->MODER |= (0x1555);
	
	GPIOE->MODER |= (0x1540);

	GPIOF->MODER|=(0X1<<0);
	GPIOF->MODER &=~(0X1<<1);
	
		// Push-pull mode
	GPIOB->OTYPER &= ~(0x1<<0);
	GPIOB->OTYPER &= ~(0x1<<7);
	GPIOB->OTYPER &= ~(0x1<<14);

	GPIOD->OTYPER &= ~(0x7F);
	
	GPIOE->OTYPER &= ~(0x78);
	
	GPIOA->OTYPER &= ~(0X0000);
	GPIOC->OTYPER &= ~(0X0000);
	
		// High Speed
	GPIOB->OSPEEDR &= ~(0x1<<0);
	GPIOB->OSPEEDR |= (0x1<<1);
	GPIOB->OSPEEDR &= ~(0x1<<14);
	GPIOB->OSPEEDR |= (0x1<<15);
	GPIOB->OSPEEDR &= ~(0x1<<28);
	GPIOB->OSPEEDR |= (0x1<<29);
	
	GPIOD->OSPEEDR |= (0x2AAA);
	
	GPIOE->OSPEEDR |= (0x2A80);
	
		// Disable pull-up and pull-down resistors
	GPIOB->PUPDR |= (0x00000000);
	GPIOD->PUPDR |= (0x00000000);
	GPIOE->PUPDR |= (0x00000000);
	GPIOA->PUPDR |= (0X00000080);
	GPIOC->PUPDR |= (0X08000000);
	// Salidas configuradas
	
	//Configuracion Entradas
		//Enable CLK con las salidas
	
		//High Speed
	GPIOA->OSPEEDR |= (0x1<<7);
	GPIOA->OSPEEDR &= ~(0x1<<6);
	GPIOF->OSPEEDR |= (0x1<<25);
	GPIOF->OSPEEDR &= ~(0x1<<24);
	GPIOC->OSPEEDR |= (0x1<<27);
	GPIOC->OSPEEDR &= ~(0x1<<26);
	GPIOB->OSPEEDR |= (0x1<<17);
	GPIOB->OSPEEDR &= ~(0x1<<16);
	
	
		//Enable PULL-DOWN resistor
	GPIOA->PUPDR |= (0x1<<7);
	GPIOA->PUPDR &= ~(0x1<<6);
	GPIOF->PUPDR |= (0x1<<25);
	GPIOF->PUPDR &= ~(0x1<<24);
	GPIOC->PUPDR |= (0x1<<27);
	GPIOC->PUPDR &= ~(0x1<<26);
	
		//Enable PULL-UP resistor;
	GPIOB->PUPDR |= (0x1<<17);
	GPIOB->PUPDR &= ~(0x1<<16);
	
	//Entradas Configuradas
	
while(true){
	
	
	SWT1 = (GPIOA->IDR) & 0x0008;
	SWT2 = (GPIOF->IDR) & 0x1000;
	
	if( SWT1 == 0 && SWT2 == 0){
		mode = 1;

		GPIOB->BSRR |= (0x1<<0);
		GPIOB->BSRR |= (0x1<<23);
		GPIOB->BSRR |= (0x1<<30);
		NamesHalloween();
	}else if( SWT1 == 0 && SWT2 != 0){
		mode = 2;

		GPIOB->BSRR |= (0x1<<16);
		GPIOB->BSRR |= (0x1<<7);
		GPIOB->BSRR |= (0x1<<30);
		LedsHalloween();
	}else if( SWT1 != 0 && SWT2 == 0){
		mode = 3;
	
		GPIOB->BSRR |= (0x1<<16);
		GPIOB->BSRR |= (0x1<<23);
		GPIOB->BSRR |= (0x1<<14);
		ContadorDisplay();
	}else if( SWT1 != 0 && SWT2 != 0){
		mode = 4;

		GPIOB->BSRR |= (0x1<<0);
		GPIOB->BSRR |= (0x1<<7);
		GPIOB->BSRR |= (0x1<<14);
		Display(count);
		BTNUSER = (GPIOC->IDR) & 0x2000;
	while (BTNUSER != 0){
		BTNUSER = (GPIOC->IDR) & 0x2000;
		if (BTNUSER == 0){
			count++;
		}
		delay(100);
	}
		BTNPB8 = (GPIOB->IDR) & 0x100;
	while (BTNPB8 == 0){
		BTNPB8 = (GPIOC->IDR) & 0x100;
		if (BTNPB8 != 0){
			start = true;
		}
	}
		if(start == true){
			count = count * 512;
			delay(500);
			Motor(count);
			GPIOE->BSRR |= (0x1<<19);
			GPIOE->BSRR |= (0x1<<20);
			GPIOE->BSRR |= (0x1<<21);
			GPIOE->BSRR |= (0x1<<22);
			count = 0;
			breaker = false;
			start = false;
		}
	}
}
}
void NamesHalloween(){
	while(true){
		//DrACULA
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(13);//D
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(27);//R
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(10);//A
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(12);//C
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(30);//U
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(21);//L
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(10);//A
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(36);//Off
		delay(750);
		//JIgSAw
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(19);//J
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(18);//I
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(16);//G
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(28);//S
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(10);//A
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(32);//W
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(36);//Off
		delay(750);
		//CtHULHU
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(12);//C
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(29);//T
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(20);//H
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(30);//U
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(21);//L
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(20);//H
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(30);//U
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(36);//Off
		delay(750);
	}
	Display(36);
}
void LedsHalloween(){
	BTNUSER = (GPIOC->IDR) & 0x2000;
	while(BTNUSER != 0){

			GPIOF->BSRR |= (0x1<<16);
		
	}
	while(BTNUSER == 0){
		GPIOF->BSRR |= (0x1);
		for(int i = 0; i < 16; i++){
			if(i==3){
				i=4;
			}
			GPIOA->BSRR |= (0x1<<i);
			delay(1000);
			GPIOA->BSRR |= (0x1<<(i+16));	
		}
	}
}
void ContadorDisplay(){
	Display(count);
			BTNUSER = (GPIOC->IDR) & 0x2000;
	while (BTNUSER != 0){
		BTNUSER = (GPIOC->IDR) & 0x2000;
		if (BTNUSER == 0 && count < 15){
			count++;
		}else if(count == 15){
			count = 15;
		}
		delay(100);
	}
		BTNPB8 = (GPIOB->IDR) & 0x100;
	while(BTNPB8 == 0){
		BTNPB8 = (GPIOB->IDR) & 0x100;
		if (BTNPB8 != 0){	
			count = 0;
		}
	}
}
void Motor(int){
			for(count; count > 0; count--){
				
				BTNPB8 = (GPIOB->IDR) & 0x100;
				while (BTNPB8 == 0){
				BTNPB8 = (GPIOC->IDR) & 0x100;
					if (BTNPB8 != 0){
						breaker = true;
					}
				}
				if (breaker == true){
					break;
				}
				
					Display((count/512)+1);
				
				GPIOE->BSRR |= (0x1<<3);
				GPIOE->BSRR |= (0x1<<20);
				GPIOE->BSRR |= (0x1<<21);
				GPIOE->BSRR |= (0x1<<22);
				delay(delaym);
				
				GPIOE->BSRR |= (0x1<<19);
				GPIOE->BSRR |= (0x1<<4);
				GPIOE->BSRR |= (0x1<<21);
				GPIOE->BSRR |= (0x1<<22);
				delay(delaym);
				
				GPIOE->BSRR |= (0x1<<19);
				GPIOE->BSRR |= (0x1<<20);
				GPIOE->BSRR |= (0x1<<5);
				GPIOE->BSRR |= (0x1<<22);
				delay(delaym);
				
				GPIOE->BSRR |= (0x1<<19);
				GPIOE->BSRR |= (0x1<<20);
				GPIOE->BSRR |= (0x1<<21);
				GPIOE->BSRR |= (0x1<<6);
				delay(delaym);
			}
			Display(0);
		}
