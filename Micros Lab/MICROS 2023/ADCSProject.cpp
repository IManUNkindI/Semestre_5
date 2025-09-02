#include <stm32f746xx.h>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>

float pi=3.141592;
bool temp,dist,ang,esc;
bool mod1,mod2,mod3;
volatile float voltaje =0;
volatile float voltaje2 =0;
volatile float voltaje3 =0;

int boton=0;
int fila, columna;
int prim=1;
int limite[3];
char sens1[8],sens2[8],sens3[8];

char cel[8],kel[8],fah[8];
char mm[8],cm[8],pulg[8],met[8];
char rad[8],grad[8],rev[8];

float valcel,valkel,valfah;
float valmm,valcm,valpulg,valmet;
float valrad,valgrad,valrev;

volatile int IDR_D,ODR_D=0;

int p=0;

char umbral;
char num;
char rx[8];
int mui=0;
char limit[32];
char mostrar[32];

int n=0, len=0, j=0,c=0;

char numero;
	
//COMANDOS LCD
char clear=0x01; //0b00000001:
char home =0x02; //0b00000010;
char set= 0x3C; //0b00111100; //Bus a 8 bits, LCD 2 lineas, caracter 5x10

char disp_on= 0x0F; //ob00001111; //Display ON, cursor ON, parpadeo

char mode_set1= 0x06; //0b00000110: //Incremento del cursor y modo normal

char pos_LCD=0;
char w_linea1 = (0x80+ pos_LCD); //Ob1000000 posicion cero primera fila
char w_linea2 = (0xC0+ pos_LCD); //Ob1100000 posicion cero segunda fila

void SysTick_Init (void){
	SysTick->LOAD=0x00FFFFFF;
	SysTick->CTRL=0x5;
}
void SysTick_Wait(uint32_t n){
		SysTick->LOAD=n-1;
		SysTick->VAL=0;
		while((SysTick->CTRL&(1<<16))==0){}
}
void wait(uint32_t delay){//1ms
		for(uint32_t i=0; i<delay; i++){
			SysTick_Wait(16000);		
		}
}

void comando_lcd(char);
void dato_lcd(char);

void comando_lcd(char b){
SysTick_Init();
	
GPIOD->ODR = b;
GPIOD->ODR &= ~(1UL <<12); //RS=0
GPIOD->ODR |= (1UL << 13); //Enable 1
wait(250);
GPIOD->ODR &= ~(1UL <<13); //Enable 0
}

void dato_lcd(char c){
SysTick_Init();

GPIOD->ODR = c;
GPIOD->ODR |= (1UL <<12); //RS=1 PARA ENVIAR DATOS (CARACTER)
GPIOD->ODR |= (1UL << 13); //Enable 1
wait(250);
GPIOD->ODR &= ~(1UL <<13); //Enable 0
}
void datos(char env){
dato_lcd(env);
wait(1000);//1s
GPIOD->ODR=0X0;
}

void cadena(char *cad, char ll){
for(int i=0;i<ll;i++){
datos(*(cad+i));
}}

void iniciar_lcd(){
comando_lcd(clear);
comando_lcd(home);
comando_lcd(set);
comando_lcd(disp_on);
comando_lcd(mode_set1);
comando_lcd(w_linea1);
}


extern "C"{
	void EXTI2_IRQHandler(void){
		EXTI->PR |= (1<<2);
}}

 int main(){
	SysTick_Init();
	
	RCC->APB2ENR |= 0x700; //Enciendo todos los ADC
	RCC->AHB1ENR|= 0XFFFFF; 
	 
	GPIOC->MODER |=0x10000; // Filas como salidas (0 1 2 3) -- Columnas como entradas ( 5 6 7 8)
	GPIOC->PUPDR |=0x0; // entradas como pull down
	 
	
	GPIOD->MODER|= 0x5555555;    //Pines de PE0 a PE11 salida (PE0-PE7 seran pines. PE8 sera RS y PE9 sera Enable)
	GPIOD->OSPEEDR|= 0x5555555;	//media vel.
	GPIOD->OTYPER|= 0x0;
	GPIOD->PUPDR|= 0x5155555; //pull up
	
	GPIOC-> MODER |= 0XF3;
	ADC->CCR |= 0X0;//8MHZ
	 
	RCC->APB2ENR |= (1<<14); //SE ACTIVA LA INTERRUPCION 
	GPIOB->MODER |= (1<<2); // PB2
	GPIOB->PUPDR |= (1<<4); //SE PONE EN PULL DOWN, PORQUE LA INTERRUPCION SE ACTIVARA CON UN FLANCO DE SUBIDA
	SYSCFG->EXTICR[0] |= 0x100; 
	EXTI->IMR |= (1<<2);
	EXTI->RTSR |= (1<<2); //Rising 
	
	NVIC_EnableIRQ(EXTI2_IRQn); //PERMITE HABILITAR LA INTERRUPCION
	
	GPIOA->MODER |= (1<<3);
	GPIOA->PUPDR |= (1<<6); // PA3 //SE PONE EN PULL DOWN, PORQUE LA INTERRUPCION SE ACTIVARA CON UN FLANCO DE SUBIDA
	SYSCFG->EXTICR[0] |= 0x0; 
	EXTI->IMR |= (1<<1);
	EXTI->FTSR |= (1<<1); //Rising 
	
	NVIC_EnableIRQ(EXTI1_IRQn); //PERMITE HABILITAR LA INTERRUPCION
	
  //ADC1 IN10 PC0
    ADC1->CR2 |=0X3 | 1<<11;
    ADC1->CR1 |=0X1000000;
    ADC1->SMPR1 |=0X1; // 15 ciclos reloj
    ADC1->SQR3 =10;
   
	//ADC2 IN13 PC3
    ADC2->CR2 |=0X3 | 1<<11;
    ADC2->CR1 |=0X1000000;
    ADC2->SMPR1 |=0X1; // 15 ciclos reloj
    ADC2->SQR3 =13;
   
	 //ADC3 IN12 PC2
		ADC3->CR2 |=0X3 | 1<<11;
    ADC3->CR1 |=0X1000000;
    ADC3->SMPR1 |=0X1; // 15 ciclos reloj
    ADC3->SQR3 =12;
		
		iniciar_lcd();
		
while(1){
	
		ADC1->CR2 |=0X40000000; // INICIO CONVERSION
    while((ADC1->SR & ADC_SR_EOC)==0){ //TERMOCUPLA
    ADC1->SR =0;
    }
    voltaje=(ADC1->DR>>4)*3.3/4096.0;
       
    ADC2->CR2 |=0X40000000; // INICIO CONVERSION
    while((ADC2->SR & ADC_SR_EOC)==0){//SHARP
    ADC2->SR =0;
    }
    voltaje2=(ADC2->DR>>4)*3.3/4096.0; 
       
		ADC3->CR2 |=0X40000000; // INICIO CONVERSION
    while((ADC3->SR & ADC_SR_EOC)==0){//POTENCIOMETRO 
    ADC3->SR =0;
    }
    voltaje3=(ADC3->DR>>4)*3.3/4096.0; 
		
while(prim==1){	
comando_lcd(clear);
comando_lcd(w_linea1);
dato_lcd('B');dato_lcd('I');dato_lcd('E');dato_lcd('N');dato_lcd('V');dato_lcd('E');dato_lcd('N');dato_lcd('I');dato_lcd('D');dato_lcd('O');
											
prim=0;	
}   
  
  comando_lcd(clear);
	comando_lcd(w_linea1);
	dato_lcd('E');dato_lcd('L');dato_lcd('I');dato_lcd('J');dato_lcd('A');dato_lcd(' ');
	dato_lcd('S');dato_lcd('E');dato_lcd('N');dato_lcd('S');dato_lcd('O');dato_lcd('R');
	wait(1000);	
	
	 if((GPIOA->IDR & ((1<<3))) != (1<<3)){
		temp=true;
		dist=false;
		ang=false;
		wait(1000);
	}
	 
	if((GPIOB->IDR & ((1<<2))) != (1<<2)){
		temp=false;
		dist=true;
		ang=false;
		wait(1000);
	}
	
	if(voltaje3>=0.07){
		GPIOC->ODR |=(1<<8);
	}
	else{
		GPIOC->ODR &=~(1<<8);
	}
// ----------------------------------------------------------
// CASO A - TEMPERATURA C°

if(temp==true){
	num = numero;
	valcel=(voltaje*10)+12; //equivalencia en celsius
	sprintf(cel,"%.4f ",valcel); //pasa a char
		
	
	//IMPRIME EL DE REFERENCIA (C°)
	comando_lcd(clear);
	comando_lcd(w_linea1);				
	dato_lcd('T');dato_lcd('E');dato_lcd('M');dato_lcd('P');dato_lcd(':');dato_lcd(' ');
	cadena(cel,4);dato_lcd('°');dato_lcd('C');
									
	comando_lcd(w_linea2);
	dato_lcd('T');dato_lcd('E');dato_lcd('M');dato_lcd('P');dato_lcd('=');dato_lcd(' ');
	dato_lcd('A');dato_lcd('D');dato_lcd('C');dato_lcd('*');dato_lcd('1');dato_lcd('0');dato_lcd('0');
	
}

//		Verificar();

// -----------------------------------------------------------------------
// CASO B - DISTANCIA CM
if(dist==true){

		//Distancia cm = 12,521(Voltaje)^-1,038
		valcm=12.521*(pow(voltaje2,-1.038)); //equivalencia en cm segun voltaje
		sprintf(cm,"%.4f ",valcm); //pasa a char
			
		//IMPRIME EL DE REFERENCIA (CM)
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('D');dato_lcd('I');dato_lcd('S');dato_lcd('T');dato_lcd(':');dato_lcd(' ');cadena(cm,4);dato_lcd('c');dato_lcd('m');
									
		comando_lcd(w_linea2);
		dato_lcd('C');dato_lcd('M');dato_lcd('=');dato_lcd(' ');dato_lcd('1');dato_lcd('2');dato_lcd('.');dato_lcd('5');
		dato_lcd('(');dato_lcd('V');dato_lcd(')');dato_lcd('^');dato_lcd('-');dato_lcd('1');dato_lcd('.');dato_lcd('3');
		
	}

}}
