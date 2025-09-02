#include <stm32f7xx.h>
#include <random>
#include <time.h>

void GPIO_SETUP();
void USART_SETUP();
void RCC_SETUP();
void SysTick_Init(void);
void SysTick_Wait(uint32_t n);
void SysTick_Wait1ms(uint32_t delay);
int USART3_SendChar(int value);	
int Usuario = 0; // Boton de Usuario
int Letras = 0; // Variable para funcion aleatoria
char envio='a'; // Variable para envio de letras aleatorias
int main(void)
{
	
SysTick_Init();
RCC->AHB1ENR |= 0x7;
RCC->APB2ENR |= (1<<14);
GPIOB->MODER |= ((1<<0) | (1<<28) | (1<<14));
GPIOC->PUPDR|=(1<<27);
	
SYSCFG->EXTICR[3] |= 0x20; // Se selecciona el puerto C como fuente de la EXTI13
EXTI->IMR |= 1<<13; //Desenmascarar interrupción
EXTI->RTSR |= 1<<13; //Rising desabilitado
EXTI->FTSR |= 0x0; //Falling habilitado
NVIC_EnableIRQ(EXTI15_10_IRQn); 
	
	RCC_SETUP();
	GPIO_SETUP();
	USART_SETUP();		
	SysTick_Init();
	

	
	while(1){
		
		if(Usuario == 1){

		}
}
	}

void RCC_SETUP()
{
	RCC->AHB1ENR |= 0xA;
	RCC->APB1ENR |= (1<<18); 
}
 
void GPIO_SETUP()
{
	//To USART 3 by using PD8(TX) and PD9(RX)
	GPIOD->MODER |= 0XA0000; 
	GPIOD->AFR[1] |=0X77; 	
	//Turn On LEDs
	GPIOB->MODER |= (1<<28)|(1<<14);
}
 
void USART_SETUP()
{
	
	USART3->BRR |= 0x683;
	USART3->CR1 |=((0x2D) | (0<<15));
	NVIC_EnableIRQ(USART3_IRQn);
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
 SysTick_Wait(16000);
 }
}

 
int USART3_SendChar(int value) 
	{ 
  USART3->TDR = value;
  while(!(USART3->ISR & USART_ISR_TXE));
  return 0;
  }
	
	char ABC(int L ) // Funcion para contner el abecedario [A-Z] "en 25 casos"
{
char Aux = '0'; //Variable Aux para guardar información del valor de cada letra del abecedario//
	
switch(L)
	{
	
	case 0:
		Aux = 'A';
	//return Aux;
	break;
	
	case 1:
		Aux = 'B';
	//return Aux;
	break;
	
	case 2:
		Aux = 'C';
	//return Aux;
	break;
	
	case 3:
		Aux = 'D';
	//return Aux;
	break;
	
	case 4:
		Aux = 'E';
	//return Aux;
	break;
	
	case 5:
		Aux = 'F';
	//return Aux;
	break;
	
	case 6:
		Aux = 'G';
	//return Aux;
	break;
	
	case 7:
		Aux = 'H';
	//return Aux;
	break;
	
	case 8:
		Aux = 'I';
	//return Aux;
	break;
	
	case 9:
		Aux = 'J';
	//return Aux;
	break;
	
	case 10:
		Aux = 'K';
	//return Aux;
	break;
	
	case 11:
		Aux = 'L';
	//return Aux;
	break;
	
	case 12:
		Aux = 'M';
	//return Aux;
	break;
	
	case 13:
		Aux = 'N';
	//return Aux;
	break;
	
	case 14:
		Aux = 'O';
	//return Aux;
	break;
	
	case 15:
		Aux = 'P';
	//return Aux;
	break;
	
	case 16:
		Aux = 'Q';
	//return Aux;
	break;
	
	case 17:
		Aux = 'R';
	//return Aux;
	break;
	
	case 18:
		Aux = 'S';
	//return Aux;
	break;
	case 19:
		Aux = 'T';
	//return Aux;
	break;
	
	case 20:
		Aux = 'U';
	//return Aux;
	break;
	
	case 21:
		Aux = 'V';
	//return Aux;
	break;
	
	case 22:
		Aux = 'W';
	//return Aux;
	break;
	
	case 23:
		Aux = 'X';
	//return Aux;
	break;
	
	case 24:
		Aux = 'Y';
	//return Aux;
	break;
	
	case 25:
		Aux = 'Z';
	//return Aux;
	break;
	
	}	
	return Aux;
	
	
}

extern "C"{
	
	void EXTI15_10_IRQHandler(void)	{

	SysTick_Wait1ms(200); //Delay for debounce
	EXTI->PR |= 1<<13;  //Put down the EXTI13 flag
	
		Letras = rand() % 26; // Letras del alfabeto aleatorias
		
   	envio =  ABC(Letras);
			
		USART3_SendChar(envio);
		SysTick_Wait1ms(200);
		
	}
	
	void USART3_IRQHandler(void){

		while (USART3->ISR & USART_ISR_RXNE){
		char rx = (char)(USART3->RDR & 0xFF);
			
		}	
		 	
	}
	
}