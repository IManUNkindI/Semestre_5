void config_TMatriz(){
	
	RCC->AHB1ENR |= 0x8;
	GPIOD->MODER |= 0x5500;
	GPIOD->OTYPER |= 0x0F;
	GPIOD->OSPEEDR |= 0xAAAA;
	GPIOD->PUPDR |= 0x00AA;
	RCC->APB2ENR |= (0x1<<14);
	SYSCFG->EXTICR[0] |= 0x3333;
	EXTI->IMR |= 0xF;
	EXTI->RTSR |= 0xF;
	NVIC_SetPriority(EXTI0_IRQn,2);
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI1_IRQn,2);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_SetPriority(EXTI2_IRQn,2);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_SetPriority(EXTI3_IRQn,2);
	NVIC_EnableIRQ(EXTI3_IRQn);
}
/*	Recorrido de la matriz (while(true))
		GPIOD->BSRR |= 0xFFEF0010;
		count = 0;
		SysTick_Wait1ms(10);
		GPIOd->BSRR |= 0xFFDF0020;
		count = 1;
		SysTick_Wait1ms(10);
		GPIOD->BSRR |= 0xFFBF0040;
		count = 2;
		SysTick_Wait1ms(10);
		GPIOD->BSRR |= 0xFF7F0080;
		count = 3;
		SysTick_Wait1ms(10);
*/