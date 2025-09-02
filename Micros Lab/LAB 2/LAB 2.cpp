#include <stm32f767xx.h>
#include <cstdlib>
#include "SysTick.h"
#include "LCD-Comm-Write.h"

//Variables
char clean = 0x01;                // 0b00000001 Limpieza LCD
char home = 0x02;                 // 0b00000010 Modo home LCD
char set = 0x38;    	            // 0b00111100 Define: BUS as 8 bits, LCD 2 lines, Caracter 5x8
char LCD_ON = 0x0C;               // 0b00001100 Display ON, cursor OFF, Blink OFF
char LCD_Mode = 0x06;             // 0b00000110 Cursor increment, NO blink display
char LCD_pos = 0;                 // Count position cursor
char LINE1 = (0x80 + LCD_pos);    // 0b10000000 Position 0:0 Display
char LINE2 = (0xC0 + LCD_pos);    // 0b11000000 Position 1:0 Display

int reset = 1;
int count = 0;
int frecuencia;
int i;
int numeroAleatorio = 250000;
char muestraNumero[6];
int aux[6];
unsigned long int cont;
unsigned long int desc;

//Metodos
void NUMERO();                    // Operaciones y muestra

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
	RCC->AHB1ENR |= (0x1<<0);
		//MODER
	GPIOA->MODER |= (0x05);
		//OSPEEDR
	GPIOA->OSPEEDR |= 0xAAAAAAAA;
		//PUPDR
	GPIOA->PUPDR |= 0x05;
	//Entradas configuradas

	//EXTI PULSADOR PB6

	RCC->AHB1ENR |= (0x1<<1);
	RCC->APB2ENR |= (0x1<<14);
	SYSCFG->EXTICR[1] |= 0x100;
	EXTI->IMR |= (0x1<<6);
	EXTI->RTSR |= (0x1<<6);
  NVIC_EnableIRQ(EXTI9_5_IRQn); //6

	while(true){
		while(reset == 1){
			NUMERO();
			LCD_COM(clean);
			LCD_COM(home);
			LCD_COM(set);
			LCD_COM(LCD_ON);
			LCD_COM(LCD_Mode);
			LCD_COM(LINE2);
			for(int j = 5; j >= 0; j--){
				LCD_W(muestraNumero[j]);
			}
			reset = 0;
		}
		switch(count){
			case 0:{
			char modoFrec[16] = "Modo: K(1Hz)";
			i = 0;
			LCD_COM(LINE1);
			while(modoFrec[i]!= 0){
				LCD_W(modoFrec[i]);
				i++;
			}
			frecuencia = 1000; // 1Hz
			break;}
			case 1:{
			char modoFrec[16] = "Modo: J(5Hz)";
			i = 0;
			LCD_COM(LINE1);
			while(modoFrec[i]!= 0){
				LCD_W(modoFrec[i]);
				i++;
			}	
			frecuencia = 200; // 5Hz
			break;}
			case 2:{
			char modoFrec[16] = "Modo: I(10Hz)";
			i = 0;
			LCD_COM(LINE1);
			while(modoFrec[i]!= 0){
				LCD_W(modoFrec[i]);
				i++;
			}			
			frecuencia = 100; // 10Hz
			break;}
			case 3:{
			char modoFrec[16] = "Modo: H(50Hz)";
			i = 0;
			LCD_COM(LINE1);
			while(modoFrec[i]!= 0){
				LCD_W(modoFrec[i]);
				i++;
			}			
				frecuencia = 20; // 50Hz
			break;}
		}
		SysTick_Wait1ms(10);
		cont = (GPIOA->IDR) & 0x0002;
		desc = (GPIOA->IDR) & 0x0001;
		if(cont!=0){
			NUMERO();
			LCD_COM(LINE2);
			for(int j = 5; j >= 0; j--){
				LCD_W(muestraNumero[j]);
			}
			SysTick_Wait1ms(frecuencia);
			if(numeroAleatorio<500000){
				numeroAleatorio++;
			}else{
				numeroAleatorio = 500000;
			}
		}
			if(desc!=0){
			NUMERO();
			LCD_COM(LINE2);
			for(int j = 5; j >= 0; j--){
				LCD_W(muestraNumero[j]);
			}
			SysTick_Wait1ms(frecuencia);
			if(numeroAleatorio>0){
				numeroAleatorio--;
			}else{
				numeroAleatorio = 0;
			}
		}
	}
}
extern "C"{
	void EXTI9_5_IRQHandler(void){
		if(count < 3){
			count++;
		}else{
			count = 0;
		}
		numeroAleatorio = rand()%500001;
		reset = 1;
		NUMERO();
		SysTick_Wait1ms(500);
		EXTI->PR |= (1<<6); //BAJA LA BANDERA, PONE UN 0 PARA AVISAR QUE TERMINO LA INTERRUPCION
	}
}
void NUMERO(){
		aux[5] = numeroAleatorio/100000;
		muestraNumero[5] = '0' + aux[5];
		aux[4] = (numeroAleatorio/10000)%10;
		muestraNumero[4] = '0' + aux[4];
		aux[3] = (numeroAleatorio/1000)%10;
		muestraNumero[3] = '0' + aux[3];
		aux[2] = (numeroAleatorio/100)%10;
		muestraNumero[2] = '0' + aux[2];
		aux[1] = (numeroAleatorio/10)%10;
		muestraNumero[1] = '0' + aux[1];
		aux[0] = numeroAleatorio%10;
		muestraNumero[0] = '0' + aux[0];
}