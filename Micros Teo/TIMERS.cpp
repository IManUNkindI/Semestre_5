#include <stdio.h>
#include <stm32f746xx.h>
void TIMER5_1_CONFIG();
void Clock_HSE_TIMER5_1();
void TIMER5_2_CONFIG();
		
void TIMER6_1_CONFIG();
void Clock_PLL_TIMER6_1();
void TIMER6_2_CONFIG();
void Clock_PLL_TIMER6_2();
	
void TIMER9_1_CONFIG();
void Clock_PLL_TIMER9_1();
void TIMER9_2_CONFIG();
void Clock_PLL_TIMER9_2();
	
void TIMER3_CONFIG();
void TIMER7_CONFIG();

void ADC_CONFIG();
void LED_CONFIG();
void Tomar_Volt();


uint32_t RelojMicro;
uint32_t myARR3,cuenta3,subiendo3,estado3=0;//TIM3
uint32_t myARR7,cuenta7,subiendo7,estado7=0;//TIM7
uint32_t myARR5,cuenta5=0;//TIM5
uint32_t myARR6,cuenta6=0;//TIM6
uint32_t myARR9,cuenta9=0;//TIM9
float VOLT;int CO_ADC1;

extern "C"{
	void TIM3_IRQHandler(void){
		TIM3->SR &= ~ (1<<0); //Atendemos la interrupción
	  subiendo3++;
	  if(subiendo3%2==0){estado3=1;}else{estado3=0;}
	}
	void TIM7_IRQHandler(void){
		TIM7->SR &= ~ (1<<0); //Atendemos la interrupción
	  subiendo7++;
	  if(subiendo7%2==0){estado7=1;}else{estado7=0;}
	}
}

int main(){
	
	  //Clock_HSE_TIMER5_1();
		//TIMER5_1_CONFIG();
	
	  
	  //Clock_PLL_TIMER6_1();
	  //TIMER6_1_CONFIG();
	
	  
	  //Clock_PLL_TIMER9_1();
	  //TIMER9_1_CONFIG();

		//TIMER5_2_CONFIG();
	
		
	  //Clock_PLL_TIMER6_2();
	  //TIMER6_2_CONFIG();
	
		
	  //Clock_PLL_TIMER9_2();
		//TIMER9_2_CONFIG();
	
	  TIMER3_CONFIG();
		//TIMER7_CONFIG();
	
	  ADC_CONFIG();
	  //LED_CONFIG();
	
	  SystemCoreClockUpdate();
	  RelojMicro=SystemCoreClock;
	
	while(1){
		myARR3   = TIM3->ARR;
		cuenta3  = TIM3->CNT;
		
		myARR7   = TIM7->ARR;
		cuenta7  = TIM7->CNT;
		
		myARR5   = TIM5->ARR;
		cuenta5  = TIM5->CNT;
		
		myARR6   = TIM6->ARR;
		cuenta6  = TIM6->CNT;
		
		myARR9   = TIM9->ARR;
		cuenta9  = TIM9->CNT;
		
		if(estado7==1){GPIOB->ODR|=(1<<7);}else if(estado7==0){GPIOB->ODR&=~(1<<7);}
		if(estado3==1){Tomar_Volt();}else if(estado3==0){CO_ADC1=0;VOLT=0;}
	}
}
void TIMER5_1_CONFIG(){//8MHz 2seg   
	//CONFIURACION TIMER
	RCC->APB1ENR|=(1<<3);
	TIM5->PSC=1599;           
	TIM5->ARR=9999;           
	TIM5->CR1|=TIM_CR1_CEN;   
}
void Clock_HSE_TIMER5_1(){
	RCC->CR|=RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY)==0);
	RCC->CFGR|=RCC_CFGR_SW_HSE;
}



void TIMER6_1_CONFIG(){//32MHz 10seg                
	RCC->APB1ENR|=(1<<4);
	TIM6->PSC=31999;             
	TIM6->ARR=9999;              
	TIM6->CR1|=TIM_CR1_CEN; 
}
void Clock_PLL_TIMER6_1(){
// 1. Activar la fuente de reloj
	RCC->CR|=RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY)==0);
//2. Habilitar regulador PWREN
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
//3. Pilas con la latencia	
	FLASH->ACR |= FLASH_ACR_ARTEN | FLASH_ACR_LATENCY_1WS;
//4. Ojito con los perifericos (32 MHz)	
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV1;
// 5. Nos metemos con PLL
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);
	RCC->PLLCFGR |= 8<< 0;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
	RCC->PLLCFGR |= 64 << 6;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
	RCC->PLLCFGR |= 0<< 16;
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSE);
// 6. A encenderla
	RCC->CR|= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY)==0);	
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}








void TIMER9_1_CONFIG(){//70MHz 20seg
	RCC->APB2ENR|=(1<<16);
	TIM9->PSC=34999;
	TIM9->ARR=39999;
	TIM9->CR1|=TIM_CR1_CEN;
}
void Clock_PLL_TIMER9_1(){
// 1. Activar la fuente de reloj
	RCC->CR|=RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY)==0);
//2. Habilitar regulador PWREN
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
//3. Pilas con la latencia	
	FLASH->ACR |= FLASH_ACR_ARTEN | FLASH_ACR_LATENCY_2WS;
//4. Ojito con los perifericos (70 MHz)	
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV2;
// 5. Nos metemos con PLL
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);
	RCC->PLLCFGR |= 8<< 0;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
	RCC->PLLCFGR |= 140 << 6;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
	RCC->PLLCFGR |= 0<< 16;
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSE);
// 6. A encenderla
	RCC->CR|= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY)==0);	
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}






void TIMER5_2_CONFIG(){//16MHz 60seg  
	RCC->APB1ENR|=(1<<3);
	TIM5->PSC=59999;           
	TIM5->ARR=15999;           
	TIM5->CR1|=TIM_CR1_CEN;  
}

void TIMER6_2_CONFIG(){//50MHz 50seg                
	RCC->APB1ENR|=(1<<4);
	TIM6->PSC=49999;             
	TIM6->ARR=49999;             
	TIM6->CR1|=TIM_CR1_CEN;      
}
void Clock_PLL_TIMER6_2(){
// 1. Activar la fuente de reloj
	RCC->CR|=RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY)==0);
//2. Habilitar regulador PWREN
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
//3. Pilas con la latencia	
	FLASH->ACR |= FLASH_ACR_ARTEN | FLASH_ACR_LATENCY_2WS;
//4. Ojito con los perifericos (50 MHz)	
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV1;
// 5. Nos metemos con PLL
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);
	RCC->PLLCFGR |= 8<< 0;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
	RCC->PLLCFGR |= 100 << 6;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
	RCC->PLLCFGR |= 0<< 16;
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSE);
// 6. A encenderla
	RCC->CR|= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY)==0);	
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}




void TIMER9_2_CONFIG(){//90MHz 5seg
	RCC->APB2ENR|=(1<<16);
	TIM9->PSC=44999;
	TIM9->ARR=9999;
  TIM9->CR1|=TIM_CR1_CEN;
}
void Clock_PLL_TIMER9_2(){
// 1. Activar la fuente de reloj
	RCC->CR|=RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY)==0);
//2. Habilitar regulador PWREN
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
//3. Pilas con la latencia	
	FLASH->ACR |= FLASH_ACR_ARTEN | FLASH_ACR_LATENCY_2WS;
//4. Ojito con los perifericos (90 MHz)	
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV2;
// 5. Nos metemos con PLL
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);
	RCC->PLLCFGR |= 8<< 0;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
	RCC->PLLCFGR |= 180 << 6;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
	RCC->PLLCFGR |= 0<< 16;
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSE);
// 6. A encenderla
	RCC->CR|= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY)==0);	
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
	


void TIMER3_CONFIG(){//HSI 16MHz 60seg
	RCC->APB1ENR|=(1<<1);
	                          
	TIM3->PSC=23999;                           
	TIM3->ARR=19999;                           
	TIM3->DIER|=TIM_DIER_UIE;                  
	TIM3->CR1|=TIM_CR1_CEN;                    
	NVIC_EnableIRQ(TIM3_IRQn);                 
}
void TIMER7_CONFIG(){//HSI 16MHz 15seg
	RCC->APB1ENR|=(1<<5);
	TIM7->PSC=23999;
	TIM7->ARR=9999;
	TIM7->DIER|=TIM_DIER_UIE;
	TIM7->CR1|=TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM7_IRQn);
}


void LED_CONFIG(){
	RCC->AHB1ENR|=(1<<1);
	GPIOB->MODER|=0x4000;
	GPIOB->OTYPER|=0;
	GPIOB->OSPEEDR|=0x4000;
	GPIOB->PUPDR|=0x0000;//8000
}
void ADC_CONFIG(){
	RCC->AHB1ENR|=(1<<0);
	RCC->APB2ENR|=(1<<8);
	
	GPIOA->MODER|=0x3;//PA0
	GPIOA->OTYPER|=0;
	GPIOA->OSPEEDR|=0;
	GPIOA->PUPDR|=0;
	
	//CONFIGURACION ADC1_IN0
	ADC->CCR|=0x3000;//2MHz de frecuencia
	ADC1->CR1|=(1<<24);//10BITS DE RESOLUCION
	ADC1->CR2|=(1<<1)|(1<<0);//100011, ENCIENDE ADC1, CONVERSION CONTINUA
	ADC1->SMPR2|=0x38000;//480 CICLOS
	ADC1->SQR3|=0x0;//NO NECESITA ORDEN E IN_0
}
void Tomar_Volt(){
	ADC1->CR2|=(1<<30);
	
	while((ADC1->SR &(1<<1))==0){ADC1->SR=0;}
	CO_ADC1 = ADC1->DR;
	VOLT = CO_ADC1 * (3.3/1023);
}


/*#include <stdio.h>
#include <stm32f746xx.h>
void TIMER5_1_CONFIG();
void Clock_PLL();
void Clock_HSE_TIMER5_1();

uint32_t RelojMicro;
uint32_t myARR5,cuenta5=0;//TIM5

int main(){
	
	  //Clock_PLL();
  	Clock_HSE_TIMER5_1();
		TIMER5_1_CONFIG();
	
	  SystemCoreClockUpdate();
	  RelojMicro=SystemCoreClock;
	
	while(1){
		
		myARR5   = TIM5->ARR;
		cuenta5  = TIM5->CNT;
		
	}
}
void TIMER5_1_CONFIG(){//8MHz 2seg   
	//CONFIURACION TIMER
	RCC->APB1ENR|=(1<<3);
	TIM5->PSC=1599;           
	TIM5->ARR=9999;           
	TIM5->CR1|=TIM_CR1_CEN;   
}
void Clock_PLL(){
// 1. Activar la fuente de reloj
	RCC->CR|=RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY)==0);
	
//2. Habilitar regulador PWREN
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	
//3. Pilas con la latencia	
	FLASH->ACR |= FLASH_ACR_ARTEN | FLASH_ACR_LATENCY_0WS;
	
//4. Ojito con los perifericos (8 MHz)	
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV1;
	
// 5. Nos metemos con PLL
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM);
	RCC->PLLCFGR |= 8<< 0;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN);
	RCC->PLLCFGR |= 16 << 6;
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLP);
	RCC->PLLCFGR |= 0<< 16;
	RCC->PLLCFGR |= (RCC_PLLCFGR_PLLSRC_HSE);

// 6. A encenderla
	RCC->CR|= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY)==0);	
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}
void Clock_HSE_TIMER5_1(){
	RCC->CR|=RCC_CR_HSEON;
	while((RCC->CR & RCC_CR_HSERDY)==0);
	RCC->CFGR|=RCC_CFGR_SW_HSE;
}*/
