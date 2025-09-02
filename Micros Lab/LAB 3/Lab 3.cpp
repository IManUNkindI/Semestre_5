#include <stm32f767xx.h>
#include <string.h>
#include "SysTick.h"
#include "TMatriz.h"
#include "LCD-Comm-Write.h"
#include "USART.h"

int reset = 0;
int pos = 0;

int en = 0;

int i, j;
char data;
char cadena[32];
int l = 0;

char matriz[4][4] = {'A','I','E','O',
										'H','L','S','M',
										'N','B','T','P',
										'*','R','#',' '};
int count;
char mensaje[32];
char tecla;
int k = 0;
int p = 0;
int m = 0;

										
int main(){
	SysTick_Init();
	//config_USART(int USARTX, char Puerto, int Pines xXxX, float BitsStop, BAUDRATE XXXX)
	config_USART(3, 'D', 89, 1, 28800);
	config_USART(6, 'G', 914, 2, 9600);
	
	config_TMatriz();
	config_LCD();
	
	SYSCFG->EXTICR[3] |= 0x0020;
	EXTI->IMR |= 0x2000;
	EXTI->RTSR |= 0x2000;
	NVIC_SetPriority(EXTI15_10_IRQn,1);
	NVIC_EnableIRQ(EXTI15_10_IRQn); //13
	
	while (true){
		// Limpieza incical LCD y Posicion del cursor
		while(reset == 0){
			LCD_COM(clean);
			LCD_COM(home);
			LCD_COM(set);
			LCD_COM(LCD_ON);
			LCD_COM(LCD_Mode);
			LCD_COM(LINE1);
			if(cadena != 0 && en == 1){
				while(cadena[l]!= 0 && l<16){
					LCD_W(cadena[l]);
					l++;
					}
				LCD_COM(LINE2);
				while(cadena[l]!= 0 && en == 1){
					LCD_W(cadena[l]);
					l++;
					}
				en = 2;
			}
			LCD_COM(LINE1);
			reset = 1;
		}
		if (en == 2){
				SysTick_Wait1ms(2500);
				reset = 0;
			en = 0;
		}
		
		if(pos==1){
			LCD_COM(LINE2);
		}
		//Recepcion USART3
		
		
		//Inicializacion de variables
		l = 0;
		tecla = 0;
		i = 0;
		j = 0;
		while(j<32){
			cadena[j]=0;
			j++;
		}
		
		//Recorrido de la matriz
		GPIOD->BSRR |= 0xFFEF0010;
		count = 0;
		SysTick_Wait1ms(25);
		GPIOD->BSRR |= 0xFFDF0020;
		count = 1;
		SysTick_Wait1ms(25);
		GPIOD->BSRR |= 0xFFBF0040;
		count = 2;
		SysTick_Wait1ms(25);
		GPIOD->BSRR |= 0xFF7F0080;
		count = 3;
		SysTick_Wait1ms(25);
		
		//Envio PC USART3
		m = 0;
		if(mensaje[k-1] == '#')
		{
			USART3->CR1 |= (0x1<<2);
			USART6->CR1 &= ~(0x1<<2);
			mensaje[k-1]= ' ';
			USART3_SendChain(mensaje);
			USART3_SendChar('x');
			while(m<32){
			mensaje[m]=' ';
			m++;
			}
			k = 0;
			pos = 0;
			reset = 0;
		}
		if(mensaje[k-1] == '*')
		{
			USART3->CR1 &= ~(0x1<<2);
			USART6->CR1 |= (0x1<<2);
			mensaje[k-1]= ' ';
			USART6_SendChain(mensaje);
			USART6_SendChar('x');
			while(m<32){
			mensaje[m]=' ';
			m++;
			}
			k = 0;
			pos = 0;
			reset = 0;
		}
		SysTick_Wait1ms(100);
	}
}
 extern "C"{
	 	void USART1_IRQHandler(void){
			if(USART1->CR1 == 0x2D){
				while (USART1->ISR & USART_ISR_RXNE){
				data =(USART1->RDR & 0xFF); 				// RXNE ACTIVA RECIBE CHAR
				cadena[i] = data;
				i++;
				}
				reset = 0;
				en = 1;
				k = 0;
			}
			en = en;
	}
		void USART2_IRQHandler(void){
			if(USART2->CR1 == 0x2D){
				while (USART2->ISR & USART_ISR_RXNE){
				data =(USART2->RDR & 0xFF); 				// RXNE ACTIVA RECIBE CHAR
				cadena[i] = data;
				i++;
				}
				reset = 0;
				en = 1;
				k = 0;
			}
			en = en;
		}
		void USART3_IRQHandler(void){
			if(USART3->CR1 == 0x2D){
				while (USART3->ISR & USART_ISR_RXNE){
				data =(USART3->RDR & 0xFF); 				// RXNE ACTIVA RECIBE CHAR
				cadena[i] = data;
				i++;
				}
				reset = 0;
				en = 1;
				k = 0;
			
			}
			en = en;
	}
		void USART6_IRQHandler(void){
			if(USART6->CR1 == 0x2D){
				while (USART6->ISR & USART_ISR_RXNE){
				data =(USART6->RDR & 0xFF); 				// RXNE ACTIVA RECIBE CHAR
				cadena[i] = data;
				i++;
				}
				reset = 0;
				en = 1;
				k = 0;
			}
			en = en;
	}
		void EXTI0_IRQHandler (void){
			tecla=matriz[count][0];
			mensaje[k] = tecla;
			LCD_W(tecla);
			k++;
			if(k>15){
				if (pos == 0){
					pos = 1;
				}else{
					pos = 2;
				}
			}
			SysTick_Wait1ms(250);
			EXTI->PR |= (0x1<<0);
	}
		void EXTI1_IRQHandler (void){
			tecla=matriz[count][1];
			mensaje[k] = tecla;
			LCD_W(tecla);
			k++;
			if(k>15){
				if (pos == 0){
					pos = 1;
				}else{
					pos = 2;
				}
			}
			SysTick_Wait1ms(250);
			EXTI->PR |= (0x1<<1);
	}
		void EXTI2_IRQHandler (void){
			tecla=matriz[count][2];
			mensaje[k] = tecla;
			LCD_W(tecla);
			k++;
			if(k>15){
				if (pos == 0){
					pos = 1;
				}else{
					pos = 2;
				}
			}
			SysTick_Wait1ms(250);
			EXTI->PR |= (0x1<<2);
	}
		void EXTI3_IRQHandler (void){
			tecla=matriz[count][3];
			mensaje[k] = tecla;
			LCD_W(tecla);
			k++;
			if(k>15){
				if (pos == 0){
					pos = 1;
				}else{
					pos = 2;
				}
			}
			SysTick_Wait1ms(250);
			EXTI->PR |= (0x1<<3);
	}
		void EXTI15_10_IRQHandler(void){
			
			reset = 0;
			pos = 0;
			en = 0;
			i = 0;
			j = 0;
			data = 0;
			l = 0;
			count = 0;
			tecla = 0;
			k = 0;
			p = 0;
			m = 0;
			
			SysTick_Wait1ms(250);
			EXTI->PR |= (0x1<<13);
	}
}