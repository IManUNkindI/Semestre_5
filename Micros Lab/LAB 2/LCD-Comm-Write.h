void LCD_COM(char com){
//RS-PD8, Enable-PD9, DATA PD0 (LSB)- PD7 (MSB)
	SysTick_Init();
	
	GPIOD->ODR = com;
	GPIOD->ODR &= ~(1UL <<8); //RS=0
	GPIOD->ODR |= (1UL << 9); //Enable 1
	SysTick_Wait1us(2000);
	GPIOD->ODR &= ~(1UL <<9); //Enable 0
}
void LCD_W(char write){
//RS-PD8, Enable-PD9, DATA PD0 (LSB)- PD7 (MSB)
	SysTick_Init();

	GPIOD->ODR = write;
	GPIOD->ODR |= (1UL <<8); //RS=1 PARA ENVIAR DATOS (CARACTER)
	GPIOD->ODR |= (1UL << 9); //Enable 1
	SysTick_Wait1us(10);
	GPIOD->ODR &= ~(1UL <<9); //Enable 0
}