#include<stm32f7xx.h>
#include<stdio.h>
#include <string.h>
#include <stdio.h>
//variables de utilidad general
int  d=0,data=0,dt=0, mot=0,ARRGEN=100;
char control=0xA0;
//variable para delay
static volatile uint32_t ticks;

//funciones de delay
	inline uint32_t millis(void) {
		return ticks;
		}
	//  delay ms
	void delay_ms(uint32_t t) {
		uint32_t elapsed;
		uint32_t start = millis();
			do {
				elapsed = millis() - start;
			} while (elapsed < t) ;
		}

//funciones interrupcion
extern "C"
	{
	//funciones Systick
	void SysTick_Handler(void) {
	ticks++;//cuenta cada ms
}
extern "C"
{
void UART7_IRQHandler(void){
		if(UART7->ISR & 0x20){
			
				data=UART7->RDR;
	if(data==255){
		mot=1;
		}
	if(data==254){
		mot=2;
		}
	if(data==253){
		control &=0x30;
		control |=0x80;
		data=0;
		
	}
	if(data==252){
		control &=0x30;
		control |=0x40;
		data=0;
	}
		if(data==250){
		control &=0xC0;
		control |=0x20;
		data=0;
		
	}
	if(data==251){
		control &=0xC0;
		control |=0x10;
		data=0;
		
	}
}}
}
}
int main(){
	//habilitar clocks
	RCC->AHB1ENR|=0xFF;//Activamos los puertos A,B,C,D,E,F,G,H 
	RCC->APB2ENR |= (1UL<<14);//habilitamos Sysconfig
	//SYSTICK
		SystemCoreClockUpdate();
		SysTick_Config(SystemCoreClock / 1000);//1ms
	//Timer 2 pwm (PA1 CH2-PA0 CH1)
		GPIOA->MODER|=0xA; //PA0 PA1 ALTER
		GPIOA->AFR[0]|=0x11;//PA0 PA1- AF1
		RCC->APB1ENR|=0x1; //habilitar timer 2
	//pwm freq= Fclk/PSC/ARR= 16MHz/300/10000 =53Hz
		TIM2->PSC=10000; 
		TIM2->ARR=ARRGEN; 
		TIM2->CNT=0;
		TIM2->CCMR1=0x6060; // PWM mode1
		TIM2->CCER|=(1<<0)|(1<<4); //enable channels
		TIM2->CR1=(1<<0); //enable timer
	//pines sentido motor (pg4,pg5-pg6,pg7)
		GPIOG->MODER|=0x5500; //PG4,PG5,PG6,PG7 OUTPUTS
		GPIOG->OSPEEDR |=0x5500;//velocidad media
		GPIOG->OTYPER =0x0;
		
		TIM2->CCR1=50;
//Timer 3
		GPIOC->MODER|=0x2000; //PC6 ALTER
		GPIOC-|>AFR[0]|=0x2000000;//PC6 - AF2
		RCC->APB1ENR|=0x2; //habilitar timer 3
		//pwm freq= Fclk/PSC/ARR= 16MHz/300/1000 =53Hz
		TIM3->PSC=1000; 
		TIM3->ARR=ARRGEN; 
		TIM3->CNT=0;
		TIM3->CCMR1=0x60; // PWM mode1
		TIM3->CCER|=(1<<0); //enable channel
		TIM3->CR1=(1<<0); //enable timer
		TIM3->CCR1=50;
		GPIOG->ODR =0xA0;//INICIAR EN 0 1, 0 1
//CONFIG bluetooth
	RCC->APB1ENR |= (1UL<<30);//habilitamos UART7 
	GPIOF->MODER |=0xA000;//definir PF6(RX) ,PF7(TX) como ALTERNO
	GPIOF->AFR[0]|=0x88000000;//AF8
	UART7->BRR = 0x683; //9600 BAUDIOS
	UART7->CR1 |= 0x2C;//HABILITA TE, RE, ,RXNE
	UART7->CR1 |= 0x1;//UE=1
	NVIC_EnableIRQ(UART7_IRQn);
while(1){
	delay_ms(80);	
		GPIOG->ODR =control;//asigna sentido de giro
		UART7->TDR = ARRGEN;// enviar ARR (VMAX conteo)
				while((UART7->ISR &= 0X80)==0);//bandera para transmision del dato
	if(mot==1){
	GPIOG->ODR =control;//asigna sentido de giro
	TIM2->CCR1=data;	
	}
	if(mot==2){
	GPIOG->ODR =control;//asigna sentido de giro
	TIM3->CCR1=data;
	}
}
}