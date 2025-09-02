#include "stm32f7xx.h"
#include <stdio.h>

	int v1=0, v2=0, valor1=0, valor2=0,prom;
	

int main(){
	//ADCS
		//potenciometro 1 ADC1_IN2 P
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
			RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
			GPIOA->MODER |= (3<<4);//analog mode
			GPIOA->PUPDR |= 0x000;//no pull up/dowm
			ADC1->CR1 |= (2<<24);//resolucion 8bits
			ADC->CCR |= 0x10000;//frecuencia 4MHz
			ADC1->SMPR2 |= 0x28000;//112 ciclos
			ADC1->CR2 |= ((0<<11)|(1<<1)|(1<<0));//encender adc, conversion continua, alinear derecha
			ADC1->SQR3 = (2<<0);//canal 2
	
			//potenciometro 2 ADC2_IN0 PA0
			RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
			GPIOA->MODER |=(3<<0);//analog mode
			GPIOA->PUPDR |=0x0000;//no pull up/dowm
			ADC2->CR1 |= 0x1000000;//resolucion 10bits
	    //ADC->CCR |= 0x30000;//frecuencia 2MHz
			ADC2->SMPR2|= 0x1C0000;//480 ciclos
			ADC2->CR2|= ((1<<11)|(1<<1)|(1<<0));//encender adc, conversion continua, alinear izq
			ADC2->SQR3|= (0<<0);//canal 0

	
while(1){
	  //CONVERSIONES ADC
//ADC1
		ADC1->CR2 |= (1<<30); //inicia la conversion
		while((ADC1->SR & (1<<1))==0) //conversion no ha terminado
		{
			ADC1->SR=0; //bandera en 0
		}
		v1=ADC1->DR; //dato conversion
		
//ADC2
		ADC2->CR2 |= (1<<30); //inicia la conversion
		while((ADC2->SR & (1<<1))==0) //conversion no ha terminado
		{
			ADC2->SR=0; //bandera en 0
		}
		v2=((ADC2->DR)); //dato conversion
		prom=(v1+v2)/2;
}
}