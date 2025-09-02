#include <stm32f767xx.h>
#include <string.h>

uint32_t BAUDRATE;
int txi = 0;
char tx[32];
char in = '0';

int val;

void Config_GPIO();
void Config_ADC();
void Config_Timer3();
void Config_Timer4();
void Config_Timer5();
void Config_DAC();
void Config_USART3();
int USART3_SendChar(int value);
void USART3_SendChain(char str[32]);

int main(){
	Config_GPIO();
	Config_ADC();
	Config_Timer3();
	Config_Timer4();
	Config_Timer5();
	Config_DAC();
	Config_USART3();

	while(true){
		ADC1->CR2 |= (0x1<<30);						// Inicia la conversion ADC1
		while (!(ADC1->SR & ADC_SR_EOC));	// Espera a que ambas conversiones finalicen
		ADC2->CR2 |= (0x1<<30);						// Inicia la conversion ADC2
		while (!(ADC2->SR & ADC_SR_EOC));	// Espera a que ambas conversiones finalicen
		ADC3->CR2 |= (0x1<<30);						// Inicia la conversion ADC3
		while (!(ADC3->SR & ADC_SR_EOC));	// Espera a que ambas conversiones finalicen
		val = ADC2->DR;
		if(val<=100){
			DAC->DHR12R1 = 4095 - val;
		}else{
			DAC->DHR12R1 = 0;
		}
		switch(in){
			case '2':{
				TIM3->CCR1 = 2250;
				GPIOC->BSRR |= 0xFED4012B;
				break;
			}
			case '1':{
				TIM3->CCR1 = 2250;
				GPIOC->BSRR |= 0xFFE80017;
				break;
			}
			case '4':{
				TIM3->CCR1 = 2500;
				GPIOC->BSRR |= 0xFFA80057;
				break;
				
			}
			case '3':{
				TIM3->CCR1 = 2000;
				GPIOC->BSRR |= 0xFF680097;
				break;
			}
			default:{
				TIM3->CCR1 = 2250;
				GPIOC->BSRR |= 0xFFFF0000;
				break;
			}
		}	
		

		TIM4->CCR1 = 19999;
		TIM5->CCR1 = 19999;
	}
}
void Config_GPIO(){
	RCC->AHB1ENR |= 0x2F;
	
	GPIOA->MODER |= 0xE3C2;
	GPIOA->OSPEEDR |= 0xAAAAAAAA;
	GPIOA->AFR[0] |= 0x22222222;
	
	GPIOB->MODER |= 0x200F;
	GPIOB->OSPEEDR |= 0xAAAAAAAA;
	GPIOB->AFR[0] |= 0x22222222;
	
	GPIOC->MODER |= 0x15555;
	GPIOC->OSPEEDR |= 0xAAAAAAAA;
	
	GPIOD->MODER |= 0xA0000;
	GPIOD->OSPEEDR |= 0xAAAAAAAA;
	GPIOD->AFR[1] |= 0x77;
}
void Config_ADC(){
  RCC->APB2ENR|= 0x700;// ADC1...2...3 habilitados
  ADC->CCR |= (0<<16); // PCLK2 dividido por 2
	
  ADC1->CR1 |= (0<<24); // 12 bits de resolucion
	ADC2->CR1 |= (0<<24);
	ADC3->CR1 |= (0<<24);
	
  ADC1->CR2 |= (1<<0)|(1<<1)|(1<<5)|(0<<11); 	// ADC encendido, 
	ADC2->CR2 |= (1<<0)|(1<<1)|(1<<5)|(0<<11);	//conversion continua, 
	ADC3->CR2 |= (1<<0)|(1<<1)|(1<<5)|(0<<11);	//EOCS habilitado, 
																							//alineacion a la derecha.
	
	ADC1->SMPR2 |= (7<<27); // ADC1 IN9 con 480 ciclos
	ADC2->SMPR2 |= (7<<12); // ADC2 IN4 con 480 ciclos
  ADC3->SMPR2 |= (7<<9); // ADC3 IN3 con 480 ciclos
	
  ADC1->SQR1 |= (0<<0); // L=0, numero de conversiones = 1
  ADC1->SQR2 |= (0<<0); // Nada
  ADC1->SQR3 |= (1<<0); // ADC1 IN9 como primera conversion
	
	ADC2->SQR1 |= (0<<0); // L=0, numero de conversiones = 1
  ADC2->SQR2 |= (0<<0); // Nada
  ADC2->SQR3 |= (7<<0); // ADC2 IN4 como primera conversion
	
	ADC3->SQR1 |= (0<<0); // L=0, numero de conversiones = 1
  ADC3->SQR2 |= (0<<0); // Nada
  ADC3->SQR3 |= (3<<0); // ADC3 IN3 como primera conversion
}
void Config_Timer3(){//PA6   Garra 1
	RCC->APB1ENR |=(0x1<<1);

	TIM3->ARR = 4999;
	TIM3->PSC = 15;
	TIM3->EGR = 0x1;
	TIM3->CCMR1 = 0x6060;
	TIM3->CCER = 0x1111;
	TIM3->CR1 = 0x1;
}
void Config_Timer4(){//PB6   Garra 2
	RCC->APB1ENR |= (0x1<<2);

	TIM4->ARR = 20000 - 1;
	TIM4->PSC = 16 - 1;
	TIM4->EGR = 0x1;
	TIM4->CCMR1 = 0x6060;
	TIM4->CCMR2 = 0x6060;
	TIM4->CCER = 0x1111;
	TIM4->CR1 = 0x1;
}
void Config_Timer5(){ //PA0  Direccion
	RCC->APB1ENR |= (0x1<<3);
	
	TIM5->ARR = 20000- 1;
	TIM5->PSC = 16 - 1;
	TIM5->EGR = 0x1;
	TIM5->CCMR1 = 0x6060;
	TIM5->CCMR2	 = 0x6060;
	TIM5->CCER = 0x1111;
	TIM5->CR1 = 0x1;
}
void Config_DAC(){
	RCC->APB1ENR |= (0x1<<29);// DAC Enable
	DAC->CR |= 0x1;
}
void Config_USART3(){
	RCC->APB1ENR |= (0x1<<18);
	USART3->CR2 &= ~(0x1<<12);
	USART3->CR2 &= ~(0x1<<13);
	BAUDRATE = (16000000/9600)+1;
	USART3->BRR |= BAUDRATE;
	USART3->CR1 |= 0xD;				 		// Transmision, recepcion, stop mode, enable USART
	USART3->CR1 |= (0x1<<5);	 		// Interrupcion (recepcion)
	USART3->CR1 &= ~(0x1<<15);	// Over8 = 0
		
	NVIC_SetPriority(USART3_IRQn,2);
	NVIC_EnableIRQ(USART3_IRQn);
}
extern "C"{
	void USART3_IRQHandler(void){
			if(USART3->CR1 == 0x2D){
				while (USART3->ISR & USART_ISR_RXNE){
					in =(USART3->RDR & 0xFF);
				}
			}
	}
}