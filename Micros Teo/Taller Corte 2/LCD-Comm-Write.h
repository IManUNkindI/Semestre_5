//Variables
char clean = 0x01;                // 0b00000001 Limpieza LCD
char home = 0x02;                 // 0b00000010 Modo home LCD
char set = 0x38;    	            // 0b00111100 Define: BUS as 8 bits, LCD 2 lines, Caracter 5x8
char LCD_ON = 0x0C;               // 0b00001100 Display ON, cursor OFF, Blink OFF
char LCD_Mode = 0x06;             // 0b00000110 Cursor increment, NO blink display
char LCD_pos = 0;                 // Count position cursor
char LINE1 = (0x80 + LCD_pos);    // 0b10000000 Position 0:0 Display
char LINE2 = (0xC0 + LCD_pos);    // 0b11000000 Position 1:0 Display

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
void I2C_LCD_COM(char com){
	SysTick_Init();
	GPIOD->ODR = com;
	GPIOD->ODR &= ~(1UL <<8); //RS=0
	GPIOD->ODR |= (1UL << 9); //Enable 1
	SysTick_Wait1us(2000);
	GPIOD->ODR &= ~(1UL <<9); //Enable 0
}
void I2C_LCD_W(char write){
	SysTick_Init();

	GPIOD->ODR = write;
	GPIOD->ODR |= (1UL <<8); //RS=1 PARA ENVIAR DATOS (CARACTER)
	GPIOD->ODR |= (1UL << 9); //Enable 1
	SysTick_Wait1us(10);
	GPIOD->ODR &= ~(1UL <<9); //Enable 0
}