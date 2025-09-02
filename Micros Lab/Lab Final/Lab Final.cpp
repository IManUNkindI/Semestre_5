#include <stm32f767xx.h>
#include "SysTick.h"
#include "LCD-Comm-Write.h"
#include "TMatriz.h"
#include "Timer.h"

char matriz[4][4] = {'1','2','3','A',
										'4','5','6','B',
										'7','8','9','C',
										'*','0','#','D'};
int count;
char tecla;
int reset = 0;										
			
int temp = 0;

int error = 0;										
int i = 0;
int k = 0;

char claveF[4];
char claveIn[4];
int open = 0;
int cambio = 0;								
		
										
 int main(){
	SysTick_Init();
	config_LCD();
	Config_GPIO();	
	config_TMatriz();
	Config_Timer4();
	Config_Timer3();
	Config_Timer5();
	claveF[0] = '9';
	claveF[1] = '8';
	claveF[2] = '7';
	claveF[3] = '6';
	
	RCC->AHB1ENR |= (0x1<<4);
	GPIOE->MODER |= (0x5<<0);
	while(true){
		if(tecla == '*' && cambio == 1){
			tecla = 0;
			reset = 0;
			k = 0;
			claveIn[0] = 0;
			claveIn[1] = 0;
			claveIn[2] = 0;
			claveIn[3] = 0;
			cambio = 0;
			error = 0;
		}
		if(tecla == '*' && cambio == 0){
			if(claveIn[0] != claveF[0] || claveIn[1] != claveF[1] || claveIn[2] != claveF[2] || claveIn[3] != claveF[3]){
				if(error<3){
					error++;
				}else{
					error = 3;
				}
			}else if(open == 0){
				error = 0;
				open = 1;
			}else if(open == 1){
				error = 0;
				open = 0;
			}
			tecla = 0;
			reset = 0;
			k = 0;
			claveIn[0] = 0;
			claveIn[1] = 0;
			claveIn[2] = 0;
			claveIn[3] = 0;
		}
		if(tecla == '#'){
			error = 4;
			cambio = 1;
			tecla = 0;
			reset = 0;
			k = 0;
			claveIn[0] = 0;
			claveIn[1] = 0;
			claveIn[2] = 0;
			claveIn[3] = 0;
		}
		if(tecla == 'A' && cambio == 1){
			if(claveIn[0] != claveF[0] || claveIn[1] != claveF[1] || claveIn[2] != claveF[2] || claveIn[3] != claveF[3]){
				if(error<7){
					error++;
				}else{
					error = 7;
				}
			}else if(cambio == 1){
				error = 8;
			}
			tecla = 0;
			reset = 0;
			k = 0;
			claveIn[0] = 0;
			claveIn[1] = 0;
			claveIn[2] = 0;
			claveIn[3] = 0;
		}
		if(tecla == 'B' && error == 8){
			error = 0;
			claveF[0] = claveIn[0];
			claveF[1] = claveIn[1];
			claveF[2] = claveIn[2];
			claveF[3] = claveIn[3];	
			tecla = 0;
			reset = 0;
			k = 0;
			claveIn[0] = 0;
			claveIn[1] = 0;
			claveIn[2] = 0;
			claveIn[3] = 0;
			cambio = 0;
		}
		while(reset == 0){
			LCD_COM(clean);
			LCD_COM(home);
			LCD_COM(set);
			LCD_COM(LCD_ON);
			LCD_COM(LCD_Mode);
			switch(error){
				case 0:{
					LCD_COM(LINE1);
					i = 0;
					char mensaje[16] = "Clave:";
					while(mensaje[i]!= 0){
						LCD_W(mensaje[i]);
						i++;
					}
					break;
				}
				case 1:{
					LCD_COM(LINE1);
					i = 0;
					char mensaje[16] = "Clave:   x";
					while(mensaje[i]!= 0){
						LCD_W(mensaje[i]);
						i++;
					}
					break;
				}
				case 2:{
					LCD_COM(LINE1);
					i = 0;
					char mensaje[16] = "Clave:   xx";
					while(mensaje[i]!= 0){
						LCD_W(mensaje[i]);
						i++;
					}
					break;
				}
				case 3:{
					LCD_COM(LINE1);
					i = 0;
					char mensaje[16] = "Clave:   xxx";
					while(mensaje[i]!= 0){
						LCD_W(mensaje[i]);
						i++;
					}
					break;
				}
				case 4:{
					LCD_COM(LINE1);
					i = 0;
					char mensaje[16] = "Clave Actual:";
					while(mensaje[i] != 0){
						LCD_W(mensaje[i]);
						i++;
					}
					break;
				}
				case 5:{
					LCD_COM(LINE1);
					i = 0;
					char mensaje[16] = "Clave Act: x";
					while(mensaje[i] != 0){
						LCD_W(mensaje[i]);
						i++;
					}
					break;
				}
				case 6:{
					LCD_COM(LINE1);
					i = 0;
					char mensaje[16] = "Clave Act: xx";
					while(mensaje[i] != 0){
						LCD_W(mensaje[i]);
						i++;
					}
					break;
				}
				case 7:{
					LCD_COM(LINE1);
					i = 0;
					char mensaje[16] = "Clave Act: xxx";
					while(mensaje[i] != 0){
						LCD_W(mensaje[i]);
						i++;
					}
					break;
				}
				case 8:{
					LCD_COM(LINE1);
					i = 0;
					char mensaje[16] = "Clave Nueva:";
					while(mensaje[i] != 0){
						LCD_W(mensaje[i]);
						i++;
					}
					break;
				}
			}
			LCD_COM(LINE2);
			reset = 1;
		}
		if(error == 3 || error == 7){
			open = 0;
			GPIOE->BSRR |= 0xFFFE0001;
			TIM5->CCR1 = 2500;		//CHICHARRA
			TIM3->CCR1 = 1800;
			TIM3->CCR2 = 4500;
		}
		if(error == 0 && open == 1){
			GPIOE->BSRR |= 0xFFFF0000;
			TIM3->CCR1 = 700;   	//SERVO
			TIM5->CCR1 = 0;
			TIM3->CCR2 = 0;
		}else if(error == 0 && open == 0){
			GPIOE->BSRR |= 0xFFFF0000;
			TIM3->CCR1 = 1800;   	//SERVO
			TIM5->CCR1 = 0;
			TIM3->CCR2 = 0;
		}
		if(error == 8){
			GPIOE->BSRR |= 0xFFFF0000;
			TIM3->CCR1 = 1800;   	//SERVO
			TIM5->CCR1 = 0;
			TIM3->CCR2 = 0;
		}
		

		
		GPIOD->BSRR |= 0xFFEF0010;
		count = 0;
		SysTick_Wait1ms(10);
		GPIOD->BSRR |= 0xFFDF0020;
		count = 1;
		SysTick_Wait1ms(10);
		GPIOD->BSRR |= 0xFFBF0040;
		count = 2;
		SysTick_Wait1ms(10);
		GPIOD->BSRR |= 0xFF7F0080;
		count = 3;
		SysTick_Wait1ms(10);		
	}
}
extern "C"{
	void EXTI0_IRQHandler (void){
			tecla=matriz[count][0];
			if(k<4){
				claveIn[k] = tecla;
				LCD_W(claveIn[k]);
				k++;
			}
			SysTick_Wait1ms(250);
			EXTI->PR |= (0x1<<0);
	}
		void EXTI1_IRQHandler (void){
			tecla=matriz[count][1];
			if(k<4){
				claveIn[k] = tecla;
				LCD_W(claveIn[k]);
				k++;
			}
			SysTick_Wait1ms(250);
			EXTI->PR |= (0x1<<1);
	}
		void EXTI2_IRQHandler (void){
			tecla=matriz[count][2];
			if(k<4){
				claveIn[k] = tecla;
				LCD_W(claveIn[k]);
				k++;
			}
			SysTick_Wait1ms(250);
			EXTI->PR |= (0x1<<2);
	}
		void EXTI3_IRQHandler (void){
			tecla=matriz[count][3];
			if(k<4){
				claveIn[k] = tecla;
				LCD_W(claveIn[k]);
				k++;
			}
			SysTick_Wait1ms(250);
			EXTI->PR |= (0x1<<3);
	}
}