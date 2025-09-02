#include <stm32f767xx.h>
#include <math.h>
#include "SysTick.h"
#include "LCD-Comm-Write.h"
#include "I2C.h"

void Config_ADCTemp();
void Config_ADC();
void Vent();
void In();

int dif;
int val;
int main(){
	SysTick_Init();
	In();
	Config_ADCTemp();
	Config_ADC();
	Vent();
	while(true){
		
    ADC2->CR2 |= (0x1<<30); 					// Inicia la conversion ADC2
		while (!(ADC2->SR & ADC_SR_EOC));	// Espera a que ambas conversiones finalicen
		val = ADC2->DR;
		while((GPIOD->IDR & 0x1) != 0){
			ADC1->CR2 |= (0x1<<30);						// Inicia la conversion ADC1
			while (!(ADC1->SR & ADC_SR_EOC));	// Espera a que ambas conversiones finalicen
			
			dif = (ADC2->DR)-(ADC1->DR);
			
			if(dif<=0){
				dif = 15;
			}
			DAC->DHR12R1 = abs(dif);
		}
	}
}
void Config_ADCTemp(){
	RCC->AHB1ENR |= (0x1<<0); // Puerto A Enable
	RCC->APB2ENR |= (0x1<<8); // ADC1 Enable
	
	GPIOA->MODER |= (0x2<<0); // PA0 Analog mode
	
	ADC->CCR |= (0x1<<23);		// Sensor de tempratura interno
	
	ADC1->SQR1 &= ~(0x1<<0);  // L = 0;
	ADC1->SQR3 |= (0x12<<0);  // SQR1 IN18
	ADC1->SMPR1 |= (0x7<<24);	// 480 Cycles
	ADC1->CR2 |= (0x1<<0);		// ADC ON
}
void Config_ADC(){
	RCC->AHB1ENR|=(1<<0); // GPIO A habilitado
  GPIOA->MODER|=(1<<14)|(1<<15); // PA7 configurado como analógico<
	
  // Configuración del ADC
  RCC->APB2ENR|=(1<<9);// ADC2 habilitado
  ADC->CCR |= (0<<16); // PCLK2 dividido por 2
  ADC2->CR1 |= (0<<24); // 12 bits de resolución
  ADC2->CR2 |= (1<<0)|(1<<1)|(1<<5)|(0<<11); // ADC encendido, conversión continua, EOCS habilitado, alineación a la derecha.
  ADC2->SMPR1 |= (0<<0); // Sin tiempo de muestreo configurado (por defecto)
  ADC2->SMPR2 |= (7<<12); // ADC2 IN4 con 480 ciclos (ajusta según tus necesidades)
  ADC2->SQR1 |= (0<<0); // L=0, número de conversiones = 1
  ADC2->SQR2 |= (0<<0); // Nada
  ADC2->SQR3 |= (7<<0); // ADC2 IN4 como primera conversión
}
void Vent(){
	RCC->AHB1ENR |= (0x1<<1);	// Puerto B Enable
	GPIOB->MODER |= 0x3;			// PB0 Analog Mode
	RCC->APB1ENR |= (0x1<<29);// DAC Enable
	DAC->CR |= 0x1;
}
void In(){
	RCC->AHB1ENR |= (0x1<<3);
	GPIOD->MODER |= (0x1<<0);
	GPIOD->PUPDR |= (0x1<<0);
}