#include <stm32f767xx.h>
#include <stdio.h>
#include <math.h>

void SysTick_Init(void);
void SysTick_Wait(uint32_t n);
void SysTick_Wait1ms(uint32_t delay);
void SysTick_Wait1S(uint32_t delay);
void configuracion (void);
void adc(void);
void comando_lcd(char);
void dato_lcd(char);

volatile int IDR_C,ODR_C=0;
int p=0;
int prim=1;
int numero;
char clear=0x01; //0b00000001:
char home =0x02; //0b00000010;
char set= 0x3C; //0b00111100; //Bus a 8 bits, LCD 2 lineas, caracter 5x10
char disp_on= 0x0F; //ob00001111; //Display ON, cursor ON, parpadeo
char mode_set1= 0x06; //0b00000110: //Incremento del cursor y modo normal
char pos_LCD=0;
char w_linea1 = (0x80+ pos_LCD); //Ob1000000 posicion cero primera fila
char w_linea2 = (0xC0+ pos_LCD); //Ob1100000 posicion cero segunda fila

int pt100=0;
int Sharp=0;
int potenciometro=0;
char teclado[4][4] = {
											{'1','2','3','a'},
											{'4','5','6','b'},
											{'7','8','9','c'},
											{'*','0','#','d'}														
										 };

int main(){
	
SysTick_Init();
configuracion();
adc();

			while(1){
	
	ADC1->CR2|=(1UL<<30); // inicia conversion
	while((ADC1->SR & (1UL<<1))==0){ //Wait for EOC bit to become 1
			ADC1->SR=0;
		} 
	pt100=(ADC1->DR);	
		
		ADC2->CR2|=(1UL<<30); // inicia conversion
	while((ADC2->SR & (1UL<<1))==0){ //Wait for EOC bit to become 1
			ADC2->SR=0;
		} 
	Sharp=(ADC2->DR);	
		
ADC3->CR2|=(1UL<<30); // inicia conversion
	while((ADC3->SR & (1UL<<1))==0){ //Wait for EOC bit to become 1
			ADC3->SR=0;
		} 
	potenciometro=(ADC3->DR);	
		
comando_lcd(clear);
comando_lcd(home);
comando_lcd(set);
comando_lcd(disp_on);
comando_lcd(mode_set1);
comando_lcd(w_linea1);

	while(prim==1){
											
		dato_lcd('I');
		dato_lcd('N');
		dato_lcd('T');
		dato_lcd('E');
		dato_lcd('N');
		dato_lcd('S');
		dato_lcd('I');
		dato_lcd('D');
		dato_lcd('A');
		dato_lcd('D');
											
		comando_lcd(w_linea2);
		
		dato_lcd('D');
		dato_lcd('E');
		dato_lcd(' ');
		dato_lcd('B');
		dato_lcd('R');
		dato_lcd('I');
		dato_lcd('L');
		dato_lcd('L');
		dato_lcd('O');
		dato_lcd(' ');
		dato_lcd(':');
		dato_lcd(')');
		SysTick_Wait1ms(500);
		prim=0;
				}
										
		switch(numero){
			case 1:
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea2);	

		break;
		
		case 2:
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea2);	

		break;
		
		case 3:
			
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea2);	

		break;
		
		case 4:
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea2);	

		break;
		
		case 5:
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea2);	

		break;
		
		case 6:
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea2);	

		break;
		
		case 7:
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea2);

		break;
		
		case 8:
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea2);

		break;
		
		case 9:
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea2);	

		break;
		
		case 10:
		TIM13 -> CCR1 = 106;
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea2);	

		break;
		
		case 0:
		comando_lcd(clear);
    comando_lcd(w_linea1);	
		
		dato_lcd('A');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd(' ');
		dato_lcd('-');
		dato_lcd('>');
		dato_lcd(' ');
		dato_lcd('1');
		dato_lcd('0');	
		dato_lcd('6');		
		dato_lcd('5');
		dato_lcd('.');		
    comando_lcd(w_linea1);
		dato_lcd('C');
		dato_lcd('I');	
		dato_lcd('C');		
		dato_lcd('L');
		dato_lcd('O');			
		dato_lcd(' ');
		dato_lcd('U');
		dato_lcd('T');
		dato_lcd('I');
		dato_lcd('L');
		dato_lcd(':');	
		dato_lcd(' ');
		dato_lcd('0');	
		dato_lcd('%');	

		break;
		
		default:
		comando_lcd(clear);
    comando_lcd(w_linea1);
		dato_lcd('E');
		dato_lcd('R');
		dato_lcd('R');
		dato_lcd('O');
		dato_lcd('R');
		break;
		}
	}	
}

void SysTick_Init(void){
	SysTick->LOAD =0xFFFFFF;
	SysTick->CTRL =0x0000005;
	}

void SysTick_Wait(uint32_t n){
SysTick->LOAD = n-1;
SysTick->VAL =0;
while((SysTick->CTRL&0x00010000)==0);
}

void SysTick_Wait1ms(uint32_t delay){
for(uint32_t i=0; i<delay ; i++){
SysTick_Wait(16000);
}
}

void SysTick_Wait1S(uint32_t delay){
for(uint32_t i=0; i<delay ; i++){
SysTick_Wait(16000000);
	}
}
void configuracion(void){ // editar 
	
	RCC->AHB1ENR|=(0x63); // habilitar puerto a, b, f y g
	GPIOA->MODER|=(0xFC); // pines 1, 2 y 3 modo analogico
	GPIOB->MODER|=(0x2); // pines 0 modo alternante
	GPIOF->MODER|=(0x55); // pines 0, 1, 2 y 3 modo salida, 4, 5, 6 y 7 entrada, Teclado
	GPIOF->OSPEEDR|= (0x55555);	// Velocidad media
	GPIOG->MODER|=(0x155555); // pines 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 modo salida, LCD
	
}
void adc(void){ // editar

	RCC->APB2ENR|=(1<<8)|(1<<9);// ADC1 y ADC2 habilitar
	ADC->CCR |= (0<<16); // divisor de ciclos: 2
	ADC1->CR1 |= (1<<24); // Resolucion 10 bits
	ADC1->CR2 |= (1<<0)|(1<<1)|(0<<11); //ADC ON, CONT ON, alineado derecha. 
	ADC1->SMPR1 |= (0<<0); //Nothing
	ADC1->SMPR2 |= (0x38); //ADC1 IN1 con 480 ciclos
	ADC1->SQR1 |= (0<<0); //L=0, # de conversion = 1
	ADC1->SQR2 |= (0<<0); //nada
	ADC1->SQR3 = 1; //ADC1 IN1 como primera conversion
	
}
void comando_lcd(char b){
//RS-PE8, Enable-PE9, DATA PE0 (LSB)-PE7 (MSB)
	
	SysTick_Init();
	
	GPIOE->ODR = b;
	GPIOE->ODR &= ~(1UL <<8); //RS=0
	GPIOE->ODR |= (1UL << 9); //Enable 1
	SysTick_Wait1ms(250);
	GPIOE->ODR &= ~(1UL <<9); //Enable 0
}

void dato_lcd(char c){
//RS-PE8, Enable-PE9, DATA PE0 (LSB)-PE7 (MSB)
	
	SysTick_Init();

	GPIOE->ODR = c;
	GPIOE->ODR |= (1UL <<8); //RS=1 PARA ENVIAR DATOS (CARACTER)
	GPIOE->ODR |= (1UL << 9); //Enable 1
	SysTick_Wait1ms(250);
	GPIOE->ODR &= ~(1UL <<9); //Enable 0
	
}
void datos(char env){

	dato_lcd(env);

	SysTick_Wait1ms(1000);
	GPIOE->ODR=0X0;

}
void cadena(char *cad, char ll){
	for(int i=0;i<ll;i++){

	datos(*(cad+i));

	}
}
