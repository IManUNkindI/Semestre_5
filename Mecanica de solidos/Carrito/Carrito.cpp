#include <stm32f767xx.h>
int val;
void Config_Timer5(){ //PA0  CHICHARRA
	RCC->APB1ENR |= (0x1<<3);
	
	TIM5->ARR = 5000 - 1;
	TIM5->PSC = 16 - 1;
	TIM5->EGR = 0x1;
	TIM5->CCMR1 = 0x6060;
	TIM5->CCMR2	 = 0x6060;
	TIM5->CCER = 0x1111;
	TIM5->CR1 = 0x1;
}
void Config_GPIO(){
	RCC->AHB1ENR |= (0x15 << 0);
	GPIOA->MODER |= 0xAAAAAAAA;
	GPIOB->MODER |= 0xAAAAAAAA;
	GPIOA->AFR[0] |= 0x22222222;
	GPIOB->AFR[0] |= 0x22222222;
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
int main(){
	Config_GPIO();
	Config_ADC();
	Config_Timer5();
	while(true){
		ADC2->CR2 |= (0x1<<30); 					// Inicia la conversion ADC2
		while (!(ADC2->SR & ADC_SR_EOC));	// Espera a que ambas conversiones finalicen
		val = ADC2->DR;
		TIM5->CCR1 = 500;
	}	
}
