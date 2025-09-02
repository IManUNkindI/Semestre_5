#include <stm32f767xx.h>
#include <random>
#include <cstdlib>
#include <ctime>
#define FREQTRABAHB 16000000
unsigned long int BAUDRATE,Over8;
void SysTick_Init(void);
void SysTick_Wait(uint32_t n);
void SysTick_Wait1ms(uint32_t delay);
int USART_SendChar2(int);
int vec[]={1<<5,1<<6,1<<8,1<<9};
int a=0,modo=0,cont=0,pin=0,o=0,l=0,y=0;
int randomNumber=0;
char val=' ',rx[]={'0','0','0','0','0','0','0','0'},mens[11];
int main(){
	//USART2 configuración
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;//Habilitar USART2
	RCC->AHB1ENR|=1<<3;//Encender Puerto D
	GPIOD->MODER |= 0x2800; //Función alternante PD5 y PD6
	GPIOD->AFR[0] |=0x77<<20;//AF7 en PD5 y PD6
	Over8=0;
	BAUDRATE= 0x683;
	
	USART2->BRR |= BAUDRATE;
	USART2->CR1 |=(USART_CR1_TE | USART_CR1_UE | USART_CR1_RE | USART_CR1_RXNEIE | Over8<<15);
	NVIC_EnableIRQ(USART2_IRQn);
	
	//Configuración de TIMERS
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN | RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM3EN ;
	//TIM5 Paso a paso 20ms 50Hz
	//I. Cargar valor en PSC
	TIM5->PSC = 319;
	//II. Cargar valor en ARR
	TIM5->ARR = 999;
	//III. Configurar DIER y CR1
	TIM5->DIER |= TIM_DIER_UIE;
	TIM5->CR1 |=  TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM5_IRQn);
	//TIM4 Contraseña 40s
	//I. Cargar valor en PSC
	TIM4->PSC = 63999;
	//II. Cargar valor en ARR
	TIM4->ARR = 9999;
	//III. Configurar DIER y CR1
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->CR1 |=  TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM4_IRQn);
	//TIM3 LED 0.5Hz
	//I. Cargar valor en PSC
	TIM3->PSC = 31999;
	//II. Cargar valor en ARR
	TIM3->ARR = 999;
	//III. Configurar DIER y CR1
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->CR1 |=  TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM3_IRQn);
	
	//Configuración del Step
	RCC->AHB1ENR|=1<<2;
	GPIOC->MODER|=1<<10|1<<12|1<<16|1<<18;//IN1 PC5, IN2 PC6, IN3 PC8, IN4 PC9
	SysTick_Init();
	//LED
	GPIOC->MODER|=1<<14;
	// Inicializar la semilla de generación de números aleatorios
    std::srand(std::time(nullptr));
	while(1){
		
	}
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
 int USART_SendChar2(int value) { 
   USART2->TDR = value;
   while(!(USART2->ISR & USART_ISR_TXE));
   return 0;
 }
 extern "C"{
	void USART2_IRQHandler(void){
		
		while (USART2->ISR & USART_ISR_RXNE){
			val=(char)(USART2->RDR & 0xFF);
			if(val=='E'){
				o=1;
				USART_SendChar2('1');
				USART_SendChar2('.');
				USART_SendChar2('4');
				USART_SendChar2('6');
			}
			if(val=='P'){
				o=0;
				USART_SendChar2('0');
				USART_SendChar2('.');
				USART_SendChar2('0');
				
			}
			if(val=='M'){
				modo=1;
				
				val=' ';
			}if(val=='m'){
				modo=2;
				
				val=' ';
			}
			if(modo==2){
				if(val=='1'||val=='2'||val=='3'||val=='4'||val=='5'||val=='6'||val=='7'||val=='8'||val=='9'||val=='0'){
					rx[a]=val;
					a++;
				}
				if(val=='B'){
					pin=0;
					for(int i=0; i<8 ;i++){
						rx[i]='0';
					}
				}
				if(val=='S'){
					a=0;
					pin=atoi(rx);
					if(pin==randomNumber){
						l=0;
						strcpy(mens, "CORRECTO");
						for(int j=0;j<10;j++){
							USART_SendChar2(mens[j]);
						}
							for(int k=0;k<64;k++){
								for(int i=0;i<4;i++){
									GPIOC->ODR|=vec[i];
									GPIOC->ODR&=vec[i];
									SysTick_Wait1ms(10);
									}
							}
							
					}else{
						l=1;
						strcpy(mens, "INCORRECTO");
						for(int j=0;j<10;j++){
							USART_SendChar2(mens[j]);
						}
					}
				}
			}
		}
	}
}
 extern  "C" {
void TIM5_IRQHandler(void){      // Interrupción Timer 
	TIM5->SR &= ~ (1<<0); //Atendemos la interrupción
		if(modo==1&&o==1){
			GPIOC->ODR|=vec[cont];
			GPIOC->ODR&=vec[cont];
			cont++;
			if(cont>3){
				cont=0;
			}
		}
	
	}
void TIM4_IRQHandler(void){      // Interrupción Timer 
	TIM4->SR &= ~ (1<<0); //Atendemos la interrupción
		// Generar un número aleatorio de 8 dígitos
    randomNumber = std::rand() % 90000000 + 10000000;
	}
void TIM3_IRQHandler(void){      // Interrupción Timer 
	
	TIM3->SR &= ~ (1<<0); //Atendemos la interrupción
		if(l==1){
			if(y==0){
				GPIOC->ODR|=1<<7;
				y++;
			}else{
				GPIOC->ODR&=~(1<<7);
				y--;
			}
		}
	}

}
