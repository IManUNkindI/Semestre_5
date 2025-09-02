#include <stm32f401xe.h>

unsigned long pulsador1;
unsigned long pulsador2;
unsigned long pulsador3;
unsigned long pulsador4;
unsigned long pulsador5;
unsigned long pulsador6;


void irrupt();
//DELAY FUNTION
void delay(int32_t k){
	int32_t i, j ;
	for(i = 0; i < k ; i++){
		for(j = 0; j < 3000; j++);
	}
}

int main (){
RCC -> AHB1ENR|=(0x1 << 1); 

  GPIOB->MODER|=(0x1 << 12); //PB 6 OUTPUT
GPIOB->MODER&=~(0x1 << 13);

	GPIOB->MODER|=(0x1 << 14); //PB 7 OUTPUT
GPIOB->MODER&=~(0x1 << 15);
	
	GPIOB->MODER|=(0x1 << 16); //PB 8 OUTPUT
GPIOB->MODER&=~(0x1 << 17);
	
	GPIOB->MODER|=(0x1 << 18); //PB 9 OUTPUT
GPIOB->MODER&=~(0x1 << 19);
	
GPIOB->OTYPER&=~(0x0000); // NO HAY NECESIDAD DE PONERLO EN ESTE CASO YA QUE ES PUSH-PULL
	
GPIOB->OSPEEDR|=(0x00000000);	// NO HAY NECESIDAD DE PONERLO EN ESTE CASO YA QUE LA VELOCIDAD DEBE SER BAJA
	
GPIOB->PUPDR|=(0x00000000); // NO HAY NECESIDAD DE PONERLO EN ESTE CASO YA QUE AL TENER PUSH-PULL, TENEMOS TANTO PULL UP COMO PULLDOWN

// ENTRADAS

RCC -> AHB1ENR|=(0x1 << 2);

GPIOC->OTYPER |=(0x0000);

GPIOC->OSPEEDR|=(0x00000000);

	while(true){
	  pulsador1=(GPIOC->IDR)& 0x0001;
		pulsador2=(GPIOC->IDR)& 0x0002;
		pulsador4=(GPIOC->IDR)& 0x0008;
		pulsador5=(GPIOC->IDR)& 0x0010;
		pulsador6=(GPIOC->IDR)& 0x0020;
		
		irrupt();
		if(pulsador1!=0 && pulsador4!=0){
	

			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 25);
			delay(20);
			
			
			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 25);
			delay(20);

			
			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 9);
			delay(20);

			
			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 9);
			delay(10);
			
		}else if(pulsador1!=0 && pulsador5!=0){
			

			
			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 25);
			delay(10);
			
			
			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 25);
			delay(10);

			
			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 9);
			delay(10);

			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 9);
			delay(10);
	
		}else if(pulsador1!=0 && pulsador6!=0){
			
			pulsador3=(GPIOC->IDR)& 0x0004;
			if(pulsador3!=0){
				break;
			}
			
			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 25);
			delay(5);
			

			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 25);
			delay(5);

			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 9);
			delay(5);
			
			
			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 9);
			delay(5);
		
		}else if(pulsador2!=0 && pulsador4!=0){
			
			
			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 25);
			delay(20);
			
			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 9);
			delay(20);
			

			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 9);
			delay(20);

			
			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 25);
			delay(20);
			
		}else if(pulsador2!=0 && pulsador5!=0){
			

			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 25);
			delay(10);

			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 9);
			delay(10);
			
			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 9);
			delay(10);
			
			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 25);
			delay(10);
			
		}else if(pulsador2!=0 && pulsador4!=0){
			
			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 25);
			delay(10);

			GPIOB->BSRR|=(0x1 << 6);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 24);
			GPIOB->BSRR|=(0x1 << 9);
			delay(5);
	
			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 23);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 9);
			delay(5);
		
			GPIOB->BSRR|=(0x1 << 22);
			GPIOB->BSRR|=(0x1 << 7);
			GPIOB->BSRR|=(0x1 << 8);
			GPIOB->BSRR|=(0x1 << 25);
			delay(5);
			
		}
		GPIOB->BSRR|=(0x1 << 22);
		GPIOB->BSRR|=(0x1 << 23);
		GPIOB->BSRR|=(0x1 << 24);
		GPIOB->BSRR|=(0x1 << 25);
}
}
void irrupt(){
	SYSCFG->EXTICR[0] &= 0xFFFFFF0F; // No es necesario, pero es mejor limpiar. 
	SYSCFG->EXTICR[0] |= 0x00000020; // Se selecciona el puerto C como fuente de la EXTI1
	EXTI->IMR |= 1<<1; //Desenmascarar interrupción
	EXTI->RTSR |= 1<<1; //Rising habilitado
	EXTI->FTSR |= 0<<1; //Falling deshabilitado
	NVIC_EnableIRQ(EXTI3_IRQn);
}
void EXTI3_IRQHandler(void)	{

	delay(50);//Antirrebote
	EXTI->PR |= 1<<1;  //baja bandera EXTI8
	GPIOB->ODR=0x0;
}