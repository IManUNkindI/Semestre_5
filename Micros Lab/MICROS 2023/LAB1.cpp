#include "stm32f7xx.h"
#include <stdio.h>

	//LISTA DE PINES LED
	//1 PC6    //2 PB15    //3 PB13    //4 PB12    //5  PF3
	//6 PC7    //7 PB5     //8 PA3     //9 PA4     //10 PC0
	//11 PB8   //12 PB9    //13 PA5    //14 PA6    //15 PA7
	//16 PD14 - PF7
					
  //17 PD15 - PF8  //18 PF12   //19 PF13   //20 PF5 //21 PF10 
	//22 PF14  //23 PF2    //24 PF15   //25 PC12 //26 PC9   
	//27 PF1   //28 PF0    //29 PD0- PB0  //30 PD1- PB1  // 31 PC10
	
	int display[10]={0x5F,0x6,0x3B,0x2F,0x66,0x6D,0x7D,0x27,0x7F, 0x6F};  
	int nombres[17] = {0xE,0x77,0x5E,0x5,0x77,0x6D,0x79,0x7C,0x77,0x6D, 0x77,0xE,0x71,0x5,0x79,0x3E,0x5F};
  int time;
	
	void conteo(int x){
	for(int f = 0; f < x;f++){
			time = f;
		}
}

int main()
{	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= 0x5540;
	GPIOA->OTYPER &= 0x0;
	GPIOA->OSPEEDR &= 0x0;
	GPIOA->PUPDR |= 0x0;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER |= 0x55555555;
	GPIOB->OTYPER &= 0x0;
	GPIOB->OSPEEDR &= 0x0;
	GPIOB->PUPDR |= 0x0;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER |= 0x1145001;
	GPIOC->OTYPER &= 0x0;
	GPIOC->OSPEEDR &= 0x0;
	GPIOC->PUPDR |= 0x8000000; //PULL DOWN
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= 0x1555;
	GPIOD->OTYPER &= 0x0;
	GPIOD->OSPEEDR &= 0x0;
	GPIOD->PUPDR |= 0x1555;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
	GPIOF->MODER |= 0x55555555;
	GPIOF->OTYPER &= 0x0;
	GPIOF->OSPEEDR &= 0x0;
	GPIOF->PUPDR |= 0x0;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	GPIOG->MODER |= 0x0;
	GPIOG->OTYPER &= 0x0;
	GPIOG->OSPEEDR &= 0x0;
	GPIOG->PUPDR |= 0xA; // PULL DOWN PG0 - PG1
	
	while(1){
		if((GPIOG->IDR & 0xF)==0x0){ // ESTADO 0-0 Fr 1Hz
		GPIOD->ODR = 0x5F;
		for(int f=0; f<100000; f++){ 
			  GPIOA->ODR|=((1<<4)|(1<<6)|(1<<7)|(1<<5)|(1<<3));
				GPIOB->ODR|= ((1<<7)|(1<<15)|(1<<13)|(1<<12)|(1<<5)|(1<<8)|(1<<9));
				GPIOC->ODR|=((1<<6)|(1<<7)|(1<<0));
				//GPIOD->ODR|=(1<<14);
				//GPIOD->ODR&=~((1<<15)|(1<<0)|(1<<1));
				GPIOF->ODR|=(1<<3);
				GPIOF->ODR&=~((1<<8)|(1<<12)|(1<<13)|(1<<14)|(1<<15)|(1<<5)|(1<<10)|(1<<2)|(1<<1)|(1<<0));
			  GPIOB->ODR&=~((1<<0)|(1<<1));
			  GPIOC->ODR&=~((1<<12)|(1<<9)|(1<<10));
				}
		    for(int f=0; f<100000; f++){ 
				GPIOA->ODR&=~((1<<4)|(1<<6)|(1<<7)|(1<<5)|(1<<3));
				GPIOB->ODR&=~((1<<15)|(1<<13)|(1<<12)|(1<<5)|(1<<8)|(1<<9));
				GPIOC->ODR&=~((1<<6)|(1<<7)|(1<<0));
				GPIOF->ODR&=~(1<<3);
				//GPIOD->ODR|=((1<<14)|(1<<15)|(1<<0)|(1<<1));
				GPIOB->ODR|=((1<<0)|(1<<1));
				GPIOF->ODR|=((1<<7)|(1<<8)|(1<<12)|(1<<13)|(1<<14)|(1<<15)|(1<<5)|(1<<10)|(1<<2)|(1<<1)|(1<<0));
				GPIOC->ODR|=((1<<12)|(1<<9)|(1<<10));
		}
	}
		//////////////////////////////////////////// SECUENCIA SAN VALENTIN -------------------------------------------------------
		if((GPIOG->IDR & 0xF)==0x1) // PG0 - PULSADOR ON
			 {
				GPIOD-> ODR |= 0x6;
				GPIOA->ODR&=~((1<<4)|(1<<6)|(1<<7)|(1<<5)|(1<<3));
				GPIOB->ODR&=~((1<<0)|(1<<1)|(1<<15)|(1<<13)|(1<<12)|(1<<5)|(1<<8)|(1<<9));
				GPIOC->ODR&=~((1<<6)|(1<<7)|(1<<0));
				//GPIOD->ODR&=~((1<<14)|(1<<15));
				GPIOF->ODR&=~((1<<7)|(1<<8)|(1<<12)|(1<<13)|(1<<14)|(1<<15)|(1<<3)|(1<<5)|(1<<10)|(1<<2)|(1<<1)|(1<<0));
				GPIOC->ODR&=~((1<<12)|(1<<9)|(1<<10));
	      ///////////////////////////////////////////////////////////////////////////////////////////////////////
		if((GPIOG->IDR & 0xF)==0x1 && (GPIOC->IDR&(1<<13)) == 0x2000) // PULSADOR ON & Boton Usuario ON
			  {
					
					for(int f=0; f<100000; f++){
					GPIOA->ODR|=((1<<6)|(1<<7));
					//GPIOD->ODR|=((1<<14)|(1<<15));
					GPIOF->ODR|=((1<<7)|(1<<8));
					}
					for(int f=0; f<100000; f++){
					GPIOA->ODR&=~((1<<6)|(1<<7));
					GPIOF->ODR&=~((1<<7)|(1<<8));
					}
				  for(int f=0; f<100000; f++){
					GPIOA->ODR|=((1<<5)|(1<<6)|(1<<7));
					GPIOB->ODR|=(1<<9);
					//GPIOD->ODR|=(1<<15);
					GPIOF->ODR|=((1<<8)|(1<<5)|(1<<12)|(1<<13));
					}
					for(int f=0; f<100000; f++){
					GPIOA->ODR&=~((1<<5)|(1<<6)|(1<<7));
					GPIOB->ODR&=~(1<<9);
					//GPIOD->ODR&=~(1<<15);
					GPIOF->ODR&=~((1<<8)|(1<<5)|(1<<12)|(1<<13));
					}
					for(int f=0; f<100000; f++){
					GPIOA->ODR|=((1<<5)|(1<<6));
					GPIOB->ODR|=((1<<8)|(1<<9));
					GPIOF->ODR|=((1<<5)|(1<<10)|(1<<12)|(1<<13));
					}
					for(int f=0; f<100000; f++){
					GPIOA->ODR&=~((1<<5)|(1<<6));
					GPIOB->ODR&=~((1<<8)|(1<<9));
					GPIOF->ODR&=~((1<<5)|(1<<10)|(1<<12)|(1<<13));
					}
					for(int f=0; f<100000; f++){
					GPIOA->ODR|=(1<<5);
					GPIOB->ODR|=((1<<8)|(1<<9));
					GPIOC->ODR|=(1<<0);
					GPIOF->ODR|=((1<<5)|(1<<10)|(1<<14)|(1<<13));
					}
					for(int f=0; f<100000; f++){
					GPIOA->ODR&=~(1<<5);
					GPIOB->ODR&=~((1<<8)|(1<<9));
					GPIOC->ODR&=~(1<<0);
					GPIOF->ODR&=~((1<<5)|(1<<10)|(1<<14)|(1<<13));
					}

					for(int f=0; f<200000; f++){
					GPIOB->ODR|=((1<<8)|(1<<9));
					GPIOC->ODR|=(1<<0);
					GPIOA->ODR|=(1<<4);
					GPIOF->ODR|=((1<<5)|(1<<10)|(1<<14)|(1<<2));
					}
					for(int f=0; f<200000; f++){
					GPIOB->ODR&=~((1<<8)|(1<<9));
					GPIOC->ODR&=~(1<<0);
					GPIOA->ODR&=~(1<<4);
					GPIOF->ODR&=~((1<<5)|(1<<10)|(1<<14)|(1<<2));
					}
					for(int f=0; f<200000; f++){
					GPIOA->ODR|=((1<<3)|(1<<4));
					GPIOB->ODR|=(1<<8);
					GPIOC->ODR|=(1<<0);
					GPIOF->ODR|=((1<<15)|(1<<10)|(1<<14)|(1<<2));
					}
					for(int f=0; f<200000; f++){
					GPIOA->ODR&=~((1<<3)|(1<<4));
					GPIOB->ODR&=~(1<<8);
					GPIOC->ODR&=~(1<<0);
					GPIOF->ODR&=~((1<<15)|(1<<10)|(1<<14)|(1<<2));
					}
									
					for(int f=0; f<200000; f++){
					GPIOC->ODR|=((1<<0)|(1<<12));
					GPIOA->ODR|=((1<<3)|(1<<4));
					GPIOB->ODR|=(1<<5);
					GPIOF->ODR|=((1<<15)|(1<<2)|(1<<14));
					}
					for(int f=0; f<200000; f++){
					GPIOC->ODR&=~((1<<0)|(1<<12));
					GPIOA->ODR&=~((1<<3)|(1<<4));
					GPIOB->ODR&=~(1<<5);
					GPIOF->ODR&=~((1<<15)|(1<<2)|(1<<14));
					}
					for(int f=0; f<200000; f++){
					GPIOC->ODR|=((1<<7)|(1<<12)|(1<<9));
					GPIOA->ODR|=((1<<3)|(1<<4));
					GPIOB->ODR|=(1<<5);
					GPIOF->ODR|=((1<<15)|(1<<2));
					}
					for(int f=0; f<200000; f++){
					GPIOC->ODR&=~((1<<7)|(1<<12)|(1<<9));
					GPIOA->ODR&=~((1<<3)|(1<<4));
					GPIOB->ODR&=~(1<<5);
					GPIOF->ODR&=~((1<<15)|(1<<2));
					}
					for(int f=0; f<200000; f++){
					GPIOA->ODR|=(1<<3);
					GPIOB->ODR|=(1<<5);
					GPIOC->ODR|=((1<<7)|(1<<12)|(1<<9));
					GPIOF->ODR|=((1<<15)|(1<<1)|(1<<3));
					}
					for(int f=0; f<200000; f++){
					GPIOA->ODR&=~(1<<3);
					GPIOB->ODR&=~(1<<5);
					GPIOC->ODR&=~((1<<7)|(1<<12)|(1<<9));
					GPIOF->ODR&=~((1<<15)|(1<<1)|(1<<3));
					}		
					for(int f=0; f<200000; f++){
					GPIOB->ODR|=((1<<5)|(1<<12));
					GPIOC->ODR|=((1<<7)|(1<<12)|(1<<9));
					GPIOF->ODR|=((1<<1)|(1<<0)|(1<<3));
					}
					for(int f=0; f<200000; f++){
					GPIOB->ODR&=~((1<<5)|(1<<12));
					GPIOC->ODR&=~((1<<7)|(1<<12)|(1<<9));
					GPIOF->ODR&=~((1<<1)|(1<<0)|(1<<3));
					}
					for(int f=0; f<200000; f++){
					//GPIOD->ODR|=(1<<0);
					GPIOB->ODR|=((1<<0)|(1<<12)|(1<<13));
					GPIOC->ODR|=((1<<7)|(1<<9));
					GPIOF->ODR|=((1<<1)|(1<<0)|(1<<3));
					}
					for(int f=0; f<200000; f++){
					//GPIOD->ODR&=~(1<<0);
					GPIOB->ODR&=~((1<<0)|(1<<12)|(1<<13));
					GPIOC->ODR&=~((1<<7)|(1<<9));
					GPIOF->ODR&=~((1<<1)|(1<<0)|(1<<3));
					}
					for(int f=0; f<200000; f++){
					//GPIOD->ODR|=((1<<0)|(1<<1));
					GPIOB->ODR|=((1<<0)|(1<<1)|(1<<12)|(1<<13)|(1<<15));
					GPIOF->ODR|=((1<<1)|(1<<0)|(1<<3));
					}
					for(int f=0; f<200000; f++){
					//GPIOD->ODR&=~((1<<0)|(1<<1));
					GPIOB->ODR&=~((1<<0)|(1<<1)|(1<<12)|(1<<13)|(1<<15));
					GPIOF->ODR&=~((1<<1)|(1<<0)|(1<<3));
					}
					for(int f=0; f<200000; f++){
					GPIOC->ODR|=((1<<6)|(1<<10));
					//GPIOD->ODR|=((1<<0)|(1<<1));
					GPIOB->ODR|=((1<<0)|(1<<1)|(1<<12)|(1<<13)|(1<<15));
					GPIOF->ODR|=(1<<0);
					}
					for(int f=0; f<200000; f++){
					GPIOC->ODR&=~((1<<6)|(1<<10));
					//GPIOD->ODR&=~((1<<0)|(1<<1));
					GPIOB->ODR&=~((1<<0)|(1<<1)|(1<<12)|(1<<13)|(1<<15));
					GPIOF->ODR&=~(1<<0);
					}
					
					/////// CORRIMIENTO 2 REGRESO
					for(int f=0; f<100000; f++){
					GPIOC->ODR|=((1<<7)|(1<<9));
					//GPIOD->ODR|=(1<<0);
					GPIOF->ODR|=((1<<0)|(1<<1)|(1<<3));
					GPIOB->ODR|=((1<<0)|(1<<13)|(1<<12));
					}
					for(int f=0; f<100000; f++){
					GPIOC->ODR&=~((1<<7)|(1<<9));
					//GPIOD->ODR&=~(1<<0);
					GPIOF->ODR&=~((1<<0)|(1<<1)|(1<<3));
					GPIOB->ODR&=~((1<<0)|(1<<13)|(1<<12));
					}
					for(int f=0; f<100000; f++){
					GPIOF->ODR|=((1<<15)|(1<<1)|(1<<3));
					GPIOC->ODR|=((1<<7)|(1<<9)|(1<<12));
					GPIOB->ODR|=(1<<5);
					GPIOA->ODR|=(1<<3);
					}
					for(int f=0; f<500000; f++){
					GPIOF->ODR&=~((1<<15)|(1<<1)|(1<<3));
					GPIOC->ODR&=~((1<<7)|(1<<9)|(1<<12));
					GPIOB->ODR&=~(1<<5);
					GPIOA->ODR&=~(1<<3);
					}
					for(int f=0; f<100000; f++){
					GPIOF->ODR|=((1<<15)|(1<<14)|(1<<2));
					GPIOC->ODR|=((1<<0)|(1<<12));
					GPIOB->ODR|=(1<<5);
					GPIOA->ODR|=((1<<3)|(1<<4));
					}
					for(int f=0; f<100000; f++){
					GPIOF->ODR&=~((1<<15)|(1<<14)|(1<<2));
					GPIOC->ODR&=~((1<<0)|(1<<12));
					GPIOB->ODR&=~(1<<5);
					GPIOA->ODR&=~((1<<3)|(1<<4));
					}
					for(int f=0; f<100000; f++){
					GPIOF->ODR|=((1<<10)|(1<<14)|(1<<2)|(1<<5));
					GPIOC->ODR|=(1<<0);
					GPIOB->ODR|=((1<<8)|(1<<9));
					GPIOA->ODR|=(1<<4);
					}
					for(int f=0; f<100000; f++){
					GPIOF->ODR&=~((1<<10)|(1<<14)|(1<<2)|(1<<5));
					GPIOC->ODR&=~(1<<0);
					GPIOB->ODR&=~((1<<8)|(1<<9));
					GPIOA->ODR&=~(1<<4);
					}
					for(int f=0; f<100000; f++){
					GPIOF->ODR|=((1<<10)|(1<<13)|(1<<12)|(1<<5));
					GPIOB->ODR|=((1<<8)|(1<<9));
					GPIOA->ODR|=((1<<5)|(1<<6));
					}
					for(int f=0; f<100000; f++){
					GPIOF->ODR&=~((1<<10)|(1<<13)|(1<<12)|(1<<5));
					GPIOB->ODR&=~((1<<8)|(1<<9));
					GPIOA->ODR&=~((1<<5)|(1<<6));
					}
					for(int f=0; f<100000; f++){
					GPIOA->ODR|=((1<<6)|(1<<7));
					//GPIOD->ODR|=((1<<14)|(1<<15));
					GPIOF->ODR|=((1<<7)|(1<<8)|(1<<10)|(1<<13));
					//GPIOD->ODR|=(1<<15);
					}
					for(int f=0; f<100000; f++){
					GPIOA->ODR&=~((1<<6)|(1<<7));
					//GPIOD->ODR&=~((1<<14)|(1<<15));
					GPIOF->ODR&=~((1<<7)|(1<<8)|(1<<12)|(1<<13));
					//GPIOD->ODR&=~(1<<15);
					}
				}
				 // INICIO TRAMA 2Hz
		 for(int f=0; f<3; f++){
				for(int f=0; f<200000; f++){ 
						GPIOC->ODR|=(1<<6);
						GPIOB->ODR|=((1<<15)|(1<<13));
				}
				for(int f=0; f<200000; f++){ 
						GPIOC->ODR&=~(1<<6);
						GPIOB->ODR&=~((1<<15)|(1<<13));
				}
				for(int f=0; f<200000; f++){ 
						GPIOB->ODR|=((1<<13)|(1<<12));
						GPIOF->ODR|=(1<<3);
				}
				for(int f=0; f<200000; f++){ 
						GPIOB->ODR&=~((1<<13)|(1<<12));
						GPIOF->ODR&=~(1<<3);
				}
				for(int f=0; f<200000; f++){ 
					  GPIOF->ODR|=(1<<3);
						GPIOC->ODR|=(1<<7);
						GPIOB->ODR|=(1<<5);
				}
				for(int f=0; f<200000; f++){ 
						GPIOF->ODR&=~(1<<3);
						GPIOC->ODR&=~(1<<7);
						GPIOB->ODR&=~(1<<5);
				}
				for(int f=0; f<200000; f++){ 
						GPIOA->ODR|=((1<<3)|(1<<4));
					  GPIOB->ODR|=(1<<5);
				}
				for(int f=0; f<200000; f++){ 
						GPIOA->ODR&=~((1<<3)|(1<<4));
						GPIOB->ODR&=~(1<<5);
				}
				for(int f=0; f<200000; f++){
					  GPIOA->ODR|=(1<<4);					
						GPIOC->ODR|=(1<<0);
						GPIOB->ODR|=(1<<8);
				}
				for(int f=0; f<200000; f++){ 
						GPIOA->ODR&=~(1<<4);					
						GPIOC->ODR&=~(1<<0);
						GPIOB->ODR&=~(1<<8);
				}
				for(int f=0; f<200000; f++){ 	
						GPIOB->ODR|=((1<<8)|(1<<9));
						GPIOA->ODR|=(1<<5);
				}
				for(int f=0; f<200000; f++){ 
						GPIOB->ODR&=~((1<<8)|(1<<9));
						GPIOA->ODR&=~(1<<6);
				}
				for(int f=0; f<200000; f++){ 
						GPIOA->ODR|=((1<<5)|(1<<6)|(1<<7));
				}
				for(int f=0; f<200000; f++){ 
						GPIOA->ODR&=~((1<<5)|(1<<6)|(1<<7));
				}
				for(int f=0; f<200000; f++){ 	
						//GPIOD->ODR|=((1<<14)|(1<<15));
					  GPIOF->ODR|=((1<<7)|(1<<8));
						GPIOA->ODR|=(1<<7);
				}
				for(int f=0; f<200000; f++){ 
					  GPIOF->ODR&=~((1<<7)|(1<<8));
						//GPIOD->ODR&=~((1<<14)|(1<<15));
						GPIOA->ODR&=~(1<<7);
				}
				for(int f=0; f<200000; f++){ 
            //GPIOD->ODR|=(1<<15);					
						GPIOF->ODR|=((1<<8)|(1<<12)|(1<<13));
				}
				for(int f=0; f<200000; f++){
            //GPIOD->ODR&=~(1<<15);					
						GPIOF->ODR&=~((1<<8)|(1<<12)|(1<<13));
				}
				for(int f=0; f<200000; f++){ 	
						GPIOF->ODR|=((1<<15)|(1<<10)|(1<<13));
				}
				for(int f=0; f<200000; f++){ 
						GPIOF->ODR&=~((1<<15)|(1<<10)|(1<<13));
				}
				for(int f=0; f<200000; f++){ 	
						GPIOF->ODR|=((1<<14)|(1<<2)|(1<<10));
				}
				for(int f=0; f<200000; f++){ 
						GPIOF->ODR&=~((1<<14)|(1<<2)|(1<<10));
				} 
				for(int f=0; f<200000; f++){ 	
						GPIOF->ODR|=(1<<1);
						GPIOC->ODR|=((1<<12)|(1<<9));
				}
				for(int f=0; f<200000; f++){ 
						GPIOF->ODR&=~(1<<1);
						GPIOC->ODR&=~((1<<12)|(1<<9));
				}
				for(int f=0; f<200000; f++){ 
            GPIOB->ODR|=(1<<0);					
						//GPIOD->ODR|=(1<<0);
						GPIOF->ODR|=((1<<1)|(1<<0));
				}
				for(int f=0; f<200000; f++){ 
					  GPIOB->ODR&=~(1<<0);
						//GPIOD->ODR&=~(1<<0);
						GPIOF->ODR&=~((1<<1)|(1<<0));
				}
				for(int f=0; f<200000; f++){ 	
						//GPIOD->ODR|=((1<<1)|(1<<0));
					  GPIOB->ODR|=((1<<1)|(1<<0));
						GPIOC->ODR|=(1<<10);
				}
				for(int f=0; f<200000; f++){ 
						//GPIOD->ODR&=~((1<<1)|(1<<0));
					  GPIOB->ODR&=~((1<<1)|(1<<0));
						GPIOC->ODR&=~(1<<10);
				}					
			 // ---------------- REGRESO --------------------
				for(int f=0; f<200000; f++){ 
						//GPIOD->ODR|=(1<<0);
					  GPIOB->ODR|=(1<<0);
						GPIOF->ODR|=((1<<1)|(1<<0));
				}
				for(int f=0; f<200000; f++){ 
						//GPIOD->ODR&=~(1<<0);
					  GPIOB->ODR&=~(1<<0);
						GPIOF->ODR&=~((1<<1)|(1<<0));
				}
				for(int f=0; f<200000; f++){ 	
						GPIOF->ODR|=(1<<1);
						GPIOC->ODR|=((1<<12)|(1<<9));
				}
				for(int f=0; f<200000; f++){ 
						GPIOF->ODR&=~(1<<1);
						GPIOC->ODR&=~((1<<12)|(1<<9));
				}
				for(int f=0; f<200000; f++){ 	
						GPIOF->ODR|=((1<<14)|(1<<2)|(1<<10));
				}
				for(int f=0; f<200000; f++){ 
						GPIOF->ODR&=~((1<<14)|(1<<2)|(1<<10));
				}
				for(int f=0; f<200000; f++){ 	
						GPIOF->ODR|=((1<<15)|(1<<10)|(1<<13));
				}
				for(int f=0; f<200000; f++){ 
						GPIOF->ODR&=~((1<<15)|(1<<10)|(1<<13));
				}
				for(int f=0; f<200000; f++){ 
            //GPIOD->ODR|=(1<<15);					
						GPIOF->ODR|=((1<<8)|(1<<12)|(1<<13));
				}
				for(int f=0; f<200000; f++){
            //GPIOD->ODR&=~(1<<15);					
						GPIOF->ODR&=~((1<<8)|(1<<12)|(1<<13));
				}
				for(int f=0; f<200000; f++){ 	
						//GPIOD->ODR|=((1<<14)|(1<<15));
					  GPIOF->ODR|=((1<<7)|(1<<8));
						GPIOA->ODR|=(1<<7);
				}
				for(int f=0; f<200000; f++){ 
						//GPIOD->ODR&=~((1<<14)|(1<<15));
					  GPIOF->ODR&=~((1<<7)|(1<<8));
						GPIOA->ODR&=~(1<<7);
				}
				for(int f=0; f<200000; f++){ 
						GPIOA->ODR|=((1<<5)|(1<<6)|(1<<7));
				}
				for(int f=0; f<200000; f++){ 
						GPIOA->ODR&=~((1<<5)|(1<<6)|(1<<7));
				}
				for(int f=0; f<200000; f++){ 	
						GPIOB->ODR|=((1<<8)|(1<<9));
						GPIOA->ODR|=(1<<5);
				}
				for(int f=0; f<200000; f++){ 
						GPIOB->ODR&=~((1<<8)|(1<<9));
						GPIOA->ODR&=~(1<<6);
				}
				for(int f=0; f<200000; f++){
					  GPIOA->ODR|=(1<<4);					
						GPIOC->ODR|=(1<<0);
						GPIOB->ODR|=(1<<8);
				}
				for(int f=0; f<200000; f++){ 
						GPIOA->ODR&=~(1<<4);					
						GPIOC->ODR&=~(1<<0);
						GPIOB->ODR&=~(1<<8);
				}
				for(int f=0; f<200000; f++){ 
						GPIOA->ODR|=((1<<3)|(1<<4));
					  GPIOB->ODR|=(1<<5);
				}
				for(int f=0; f<200000; f++){ 
						GPIOA->ODR&=~((1<<3)|(1<<4));
						GPIOB->ODR&=~(1<<5);
				}
				for(int f=0; f<200000; f++){ 
					  GPIOF->ODR|=(1<<3);
						GPIOC->ODR|=(1<<7);
						GPIOB->ODR|=(1<<5);
				}
				for(int f=0; f<200000; f++){ 
						GPIOF->ODR&=~(1<<3);
						GPIOC->ODR&=~(1<<7);
						GPIOB->ODR&=~(1<<5);
				}
				for(int f=0; f<200000; f++){ 
						GPIOB->ODR|=((1<<13)|(1<<12));
						GPIOF->ODR|=(1<<3);
				}
				for(int f=0; f<200000; f++){ 
						GPIOB->ODR&=~((1<<13)|(1<<12));
						GPIOF->ODR&=~(1<<3);
				}
				for(int f=0; f<200000; f++){ 
						GPIOC->ODR|=(1<<6);
						GPIOB->ODR|=((1<<15)|(1<<13));
				}
				for(int f=0; f<200000; f++){ 
						GPIOC->ODR&=~(1<<6);
						GPIOB->ODR&=~((1<<15)|(1<<13));
				}
			} // FIN FOR SECUENCIA
		
		  for(int f=0; f<3; f++){ // SE ENCIENDE UN SOLO LED ------
				for(int f=0; f<200000; f++){ 
					GPIOC->ODR|=(1<<6);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOC->ODR&=~(1<<6);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR|=(1<<13);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR&=~(1<<13);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<3);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<3);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR|=(1<<5);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR&=~(1<<5);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR|=(1<<4);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR&=~(1<<4);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR|=(1<<8);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR&=~(1<<8);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR|=(1<<5);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR&=~(1<<5);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR|=(1<<7);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR&=~(1<<7);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOD->ODR|=(1<<15);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOD->ODR&=~(1<<15);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<13);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<13);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<10);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<10);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<2);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<2);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOC->ODR|=(1<<9);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOC->ODR&=~(1<<9);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<0);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<0);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR|=(1<<1);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR&=~(1<<1);
			 }
				// REGRESO
			 for(int f=0; f<200000; f++){ 
					GPIOB->ODR|=(1<<1);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR&=~(1<<1);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<0);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<0);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOC->ODR|=(1<<9);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOC->ODR&=~(1<<9);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<2);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<2);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<2);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<2);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<10);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<10);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<13);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<13);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOD->ODR|=(1<<15);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOD->ODR&=~(1<<15);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR|=(1<<7);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR&=~(1<<7);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR|=(1<<5);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR&=~(1<<5);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR|=(1<<8);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR&=~(1<<8);
			 }
			 for(int f=0; f<200000; f++){ 
					GPIOA->ODR|=(1<<4);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOA->ODR&=~(1<<4);
			 }
			 for(int f=0; f<200000; f++){ 
					GPIOB->ODR|=(1<<5);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR&=~(1<<5);
				}
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR|=(1<<3);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOF->ODR&=~(1<<3);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR|=(1<<13);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOB->ODR&=~(1<<13);
			 }
			 	for(int f=0; f<200000; f++){ 
					GPIOC->ODR|=(1<<6);
			 }
				for(int f=0; f<200000; f++){ 
					GPIOC->ODR&=~(1<<6);
			 }
	   }
	}
	//////////////////// CONTADOR ---------------------------------------------------------------------------
				if((GPIOG->IDR & 0xF)==0x2){
					GPIOD->ODR = display[2];
					for(int f = 0; f < 10;){
		      GPIOD->ODR = display[f];
		      if((GPIOC->IDR&(1<<13)) == 0x2000){
			    f++;
			    conteo(1000000);
		      }
	       }
			  }
	//////////////////// NOMBRES --------------------------------------------------------------------------
				if((GPIOG->IDR & 0xF)==0x3)
				{ GPIOD->ODR = 0x66;
					for(int f=0; f<17; f++){
					GPIOD->ODR = nombres[f]; // Aqui van nombres
					conteo(1000000);
					}
				}
}}