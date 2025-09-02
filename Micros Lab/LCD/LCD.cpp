#include <stm32f767xx.h>
#include <cstdlib>
#include <string.h>
#include "SysTick.h"
#include "LCD-Comm-Write.h"


//Variables
char clean = 0x01;                // 0b00000001 Limpieza LCD
char home = 0x02;                 // 0b00000010 Modo home LCD
char set = 0x38;    	            // 0b00111100 Define: BUS as 8 bits, LCD 2 lines, Caracter 5x8
char LCD_ON = 0x0C;               // 0b00001111 Display ON, cursor OFF, Blink OFF
char LCD_Mode = 0x06;             // 0b00000110 Cursor increment, NO blink display
char LCD_pos = 0;                 // Count position cursor
char LINE1 = (0x80 + LCD_pos);    // 0b10000000 Position 0:0 Display
char LINE2 = (0xC0 + LCD_pos);    // 0b11000000 Position 1:0 Display


int reset = 1;
int count = 0;
int frecuencia;

//Metodos

int main(){
	SysTick_Init();
	//Configurar salidas
		//Habilitar CLK
	RCC->AHB1ENR |= (0x1<<3);
		//MODER
	GPIOD->MODER |= 0x55555;
		//PUSH-PULL mode
	GPIOD->OTYPER |= 0x0000;
		//OSPEEDR
	GPIOD->OSPEEDR |= 0xAAAAAAAA;
		//No pull-up, no pull-down
	GPIOD->PUPDR |= 0x00000000;
	//Salidas configuradas
	
	//Configurar entradas
		//Habilitar CLK
	
		//MODER
	
		//OSPEEDR
	
		//PUPDR

	//Entradas configuradas

	//EXTI PULSADOR PC13

	RCC->AHB1ENR |= (0x1<<2);
	RCC->APB2ENR |= (0x1<<14);
	SYSCFG->EXTICR[3] |= 0x20;
	EXTI->IMR |= (0x1<<13);
	EXTI->RTSR |= (0x1<<13);
  NVIC_EnableIRQ(EXTI15_10_IRQn); //13

	while(true){
		while(reset == 1){
			LCD_COM(clean);
			LCD_COM(home);
			LCD_COM(set);
			LCD_COM(LCD_ON);
			LCD_COM(LCD_Mode);
			reset = 0;
		}
		switch(count){
			case 0:
			LCD_COM(LINE1);

			LCD_W('M');
			LCD_W('o');
			LCD_W('d');
			LCD_W('o');
			LCD_W(':');
			LCD_W(' ');
			LCD_W('K');
			LCD_W('(');
			LCD_W('1');
			LCD_W('H');		
			LCD_W('z');	
			LCD_W(')');			
			frecuencia = 1000; // 1Hz
			break;
			case 1:
			LCD_COM(LINE1);

			LCD_W('M');
			LCD_W('o');
			LCD_W('d');
			LCD_W('o');
			LCD_W(':');
			LCD_W(' ');
			LCD_W('J');
			LCD_W('(');
			LCD_W('5');
			LCD_W('H');		
			LCD_W('z');	
			LCD_W(')');	
			frecuencia = 200; // 5Hz
			break;
			case 2:
			LCD_COM(LINE1);

			LCD_W('M');
			LCD_W('o');
			LCD_W('d');
			LCD_W('o');
			LCD_W(':');
			LCD_W(' ');
			LCD_W('I');
			LCD_W('(');
			LCD_W('1');
			LCD_W('0');
			LCD_W('H');		
			LCD_W('z');	
			LCD_W(')');	
			frecuencia = 100; // 10Hz
			break;
			case 3:
			LCD_COM(LINE1);

			LCD_W('M');
			LCD_W('o');
			LCD_W('d');
			LCD_W('o');
			LCD_W(':');
			LCD_W(' ');
			LCD_W('H');
			LCD_W('(');
			LCD_W('5');
			LCD_W('0');
			LCD_W('H');		
			LCD_W('z');	
			LCD_W(')');	
				frecuencia = 20; // 50Hz
			break;
		}
		SysTick_Wait1ms(10);
	}
}
extern "C"{
	void EXTI15_10_IRQHandler(void){
		if(count < 3){
			count++;
		}else{
			count = 0;
		}
		LCD_COM(clean);
		SysTick_Wait1ms(500);
		EXTI->PR |= (1<<13); //BAJA LA BANDERA, PONE UN 0 PARA AVISAR QUE TERMINO LA INTERRUPCION
	}
}