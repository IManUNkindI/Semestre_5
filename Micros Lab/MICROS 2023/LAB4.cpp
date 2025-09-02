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
char MatrizIn[4][4] = {
														{'1','2','3','A'},
														{'4','5','6','B'},
														{'7','8','9','C'},
														{'*','0','#','D'}							
};

	
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
	
GPIOG->ODR = b;
GPIOG->ODR &= ~(1UL <<8); //RS=0
GPIOG->ODR |= (1UL << 9); //Enable 1
wait(250);
GPIOG->ODR &= ~(1UL <<9); //Enable 0
}

void dato_lcd(char c){
SysTick_Init();

GPIOG->ODR = c;
GPIOG->ODR |= (1UL <<8); //RS=1 PARA ENVIAR DATOS (CARACTER)
GPIOG->ODR |= (1UL << 9); //Enable 1
wait(250);
GPIOG->ODR &= ~(1UL <<9); //Enable 0
}
void datos(char env){
dato_lcd(env);
wait(1000);//1s
GPIOG->ODR=0X0;
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
char Verificar(){
	for(int i=0; i<4; i++){
			for(int j=5; j<9; j++){
				GPIOF->ODR&=~(0xF);
				GPIOF->ODR=(1<<i);	
				
			if(((GPIOF->IDR)&(1<<j)) == (1<<j)){
					numero = MatrizIn[i][j-5];
					fila=i;
					columna=j-5;
					}
				wait(2);	
			}
	}
	return numero;
}


 int main(){
	SysTick_Init();
	
	RCC->APB2ENR |= 0x700; //Enciendo todos los ADC
	RCC->AHB1ENR|= 0XFFFFF; 
	 
	GPIOF->MODER |=0x55; // Filas como salidas (0 1 2 3) -- Columnas como entradas ( 5 6 7 8)
	GPIOF->PUPDR |=0x2AA00; // entradas como pull down
	
	GPIOG->MODER|= 0x55555;    //Pines de PE0 a PE11 salida (PE0-PE7 seran pines. PE8 sera RS y PE9 sera Enable)
	GPIOG->OSPEEDR|= 0x55555;	//media vel.
	GPIOG->OTYPER|= 0x0;
	GPIOG->PUPDR|= 0x55555; //pull up
	
	GPIOC-> MODER |= 0XF3;
	ADC->CCR |= 0X0;//8MHZ
	
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
											
comando_lcd(w_linea2);
dato_lcd('E');dato_lcd('L');dato_lcd('I');dato_lcd('J');dato_lcd('A');dato_lcd(' ');
dato_lcd('L');dato_lcd('I');dato_lcd('M');dato_lcd('I');dato_lcd('T');dato_lcd('E');
wait(1000);
	 
prim=0;	
}   
  
  comando_lcd(clear);
	comando_lcd(w_linea1);
	dato_lcd('E');dato_lcd('L');dato_lcd('I');dato_lcd('J');dato_lcd('A');dato_lcd(' ');
	dato_lcd('S');dato_lcd('E');dato_lcd('N');dato_lcd('S');dato_lcd('O');dato_lcd('R');
	wait(1000);	
	
	
		 //Verificar();
	
if(mod1==true){
	  if(numero!='*'){
			n=1;
			if(n==1){ 	
		  umbral=numero;
			rx[0] = umbral;
			limit[j] = Verificar();
			j++;
			n=0;
			}
		}
		if(numero=='*'){
		mui = atoi(rx);
		limit[j] = Verificar();
		j++;
		temp=true;
		dist=false;
		ang=false;
		esc=false;
		}
}
   //Verificar();

if(mod2==true){
	  if(numero!='*'){
			n=1;
			if(n==1){ 	
		  umbral=numero;
			rx[0] = umbral;
			limit[j] = Verificar();
			j++;
			n=0;
			}
		}
		if(numero=='*'){
		mui = atoi(rx);
		limit[j] = Verificar();
		j++;
		temp=false;
		dist=true;
		ang=false;
		esc=false;
		}
}

   //Verificar();

if(mod3==true){
	  if(numero!='*'){
			n=1;
			if(n==1){ 	
		  umbral=numero;
			rx[0] = umbral;
			limit[j] = Verificar();
			j++;
			n=0;
			}
		}
		if(numero=='*'){
		mui = atoi(rx);
		limit[j] = Verificar();
		j++;
		temp=false;
		dist=false;
		ang=true;
		esc=false;
		}
}

   Verificar();

	/*if(n==1){
		c=1;
		if(c==1){
		umbral=numero;
		rx[0] = umbral;
		mui = atoi(rx);
		limit[j] = Verificar();
		j++;
		c=0;
	}*/

// ----------------------------------------------------------
// CASO A - TEMPERATURA CASE 1, 4, 7

if(temp==true){
	num = numero;
	valcel=voltaje*10; //equivalencia en celsius
	sprintf(cel,"%.4f ",valcel); //pasa a char
			
	valkel=valcel+273.15;//celsius a kelvin
	sprintf(kel,"%.4f ",valkel); //pasa a char
			
	valfah=(valcel*1.8)+32;//celsius a fahrenheit
	sprintf(fah,"%.4f ",valfah);//pasa a char
	
	
	if(voltaje>=mui){ //umbral 1v=10°c
	comando_lcd(clear);
	comando_lcd(w_linea1);		
	dato_lcd('A');dato_lcd('L');dato_lcd('E');dato_lcd('R');dato_lcd('T');dato_lcd('A');
  comando_lcd(w_linea2);
	dato_lcd('U');dato_lcd('M');dato_lcd('B');dato_lcd('R');dato_lcd('A');dato_lcd('L');
	//cadena(mostrar,4);
	}
	
		comando_lcd(clear);
		comando_lcd(w_linea1);		
		dato_lcd('T');dato_lcd('E');dato_lcd('M');dato_lcd('P');dato_lcd(':');dato_lcd(' ');
	  cadena(cel,4);dato_lcd('°');dato_lcd('C');
									
		comando_lcd(w_linea2);
		dato_lcd('T');dato_lcd('E');dato_lcd('M');dato_lcd('P');dato_lcd('=');dato_lcd(' ');dato_lcd('A');dato_lcd('D');dato_lcd('C');
		dato_lcd('*');dato_lcd('1');dato_lcd('0');dato_lcd('0');
		
		comando_lcd(clear);
	
	
	if(num=='1'){		// TEMP EN C°
	comando_lcd(clear);
			
	comando_lcd(w_linea1);				
	dato_lcd('T');dato_lcd('E');dato_lcd('M');dato_lcd('P');dato_lcd(':');dato_lcd(' ');
	cadena(cel,4);dato_lcd('°');dato_lcd('C');
									
	comando_lcd(w_linea2);
	dato_lcd('T');dato_lcd('E');dato_lcd('M');dato_lcd('P');dato_lcd('=');dato_lcd(' ');
	dato_lcd('A');dato_lcd('D');dato_lcd('C');dato_lcd('*');dato_lcd('1');dato_lcd('0');dato_lcd('0');
	
	limit[j] = Verificar();
	j++;
	}
	
	if(num== '4'){	
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('T');dato_lcd('E');dato_lcd('M');dato_lcd('P');dato_lcd(':');dato_lcd(' ');cadena(kel,6);dato_lcd('K');
									
		comando_lcd(w_linea2);
		dato_lcd('T');dato_lcd('E');dato_lcd('M');dato_lcd('P');dato_lcd('=');dato_lcd(' ');dato_lcd('C');
		dato_lcd('E');dato_lcd('L');dato_lcd('+');dato_lcd('2');dato_lcd('7');dato_lcd('3');dato_lcd('.');	
		dato_lcd('1');dato_lcd('5');				
				
		limit[j] = Verificar();
		j++;	
	}
		
	if(num=='7'){
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('T');dato_lcd('E');dato_lcd('M');dato_lcd('P');dato_lcd(':');dato_lcd(' ');cadena(fah,6);dato_lcd('°');dato_lcd('F');
									
		comando_lcd(w_linea2);
		
		dato_lcd('°');dato_lcd('F');dato_lcd('=');dato_lcd(' ');dato_lcd('(');dato_lcd('C');
		dato_lcd('E');dato_lcd('L');dato_lcd('*');dato_lcd('1');dato_lcd('.');dato_lcd('8');dato_lcd(')');dato_lcd('+');	
		dato_lcd('3');dato_lcd('2');				
		
	  limit[j] = Verificar();
		j++;		
		//(valcel*1.8)+32; FORMULA CEL A FAH
				
		}
}

		Verificar();

// -----------------------------------------------------------------------
// CASO B - DISTANCIA CASE 2, 5, 8

if(dist==true){
			num=numero;
				//Distancia cm = 12,521(Voltaje)^-1,038
			valcm=12.521*(pow(voltaje2,-1.038)); //equivalencia en cm segun voltaje
			sprintf(cm,"%.4f ",valcm); //pasa a char
			
			valmm=valcm*10;//cm a mm
			sprintf(mm,"%.4f ",valmm); //pasa a char
			
			valpulg=valcm/2.54;//cm a pulg
			sprintf(pulg,"%.4f ",valpulg);//pasa a char
			
			valmet=valcm/100;//cm a m
			sprintf(met,"%.4f ",valmet); //pasa a char
			
	    limit[j] = Verificar();
			j++;
			Verificar();
			
		if(voltaje2<mui){ //umbral 0.5v=26cm		
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('A');dato_lcd('L');dato_lcd('E');dato_lcd('R');dato_lcd('T');dato_lcd('A');dato_lcd(' ');
		dato_lcd('D');dato_lcd('I');dato_lcd('S');dato_lcd('T');
				
		}
		//IMPRIME EL DE REFERENCIA (METROS)
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('D');dato_lcd('I');dato_lcd('S');dato_lcd('T');dato_lcd(':');dato_lcd(' ');cadena(met,4);dato_lcd('m');
											
		comando_lcd(w_linea2);
		
		
		dato_lcd('D');dato_lcd('I');dato_lcd('S');dato_lcd('T');dato_lcd(' ');dato_lcd('M');dato_lcd('T');dato_lcd('=');dato_lcd(' ');
		dato_lcd('C');dato_lcd('M');dato_lcd('/');dato_lcd('1');dato_lcd('0');dato_lcd('0');
	
		if(num== '2'){
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('D');dato_lcd('I');dato_lcd('S');dato_lcd('T');dato_lcd(':');dato_lcd(' ');cadena(mm,4);dato_lcd('m');dato_lcd('m');
									
		comando_lcd(w_linea2);
		
		dato_lcd('D');dato_lcd('I');dato_lcd('S');dato_lcd('T');dato_lcd(' ');dato_lcd('M');dato_lcd('M');dato_lcd('=');dato_lcd(' ');
		dato_lcd('C');dato_lcd('M');dato_lcd('*');dato_lcd('1');dato_lcd('0');
		
		
		}
		if(num== '5'){			
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('D');dato_lcd('I');dato_lcd('S');dato_lcd('T');dato_lcd(':');dato_lcd(' ');cadena(cm,4);dato_lcd('c');dato_lcd('m');
									
		comando_lcd(w_linea2);
		dato_lcd('C');dato_lcd('M');dato_lcd('=');dato_lcd(' ');dato_lcd('1');dato_lcd('2');dato_lcd('.');dato_lcd('5');
		dato_lcd('(');dato_lcd('V');dato_lcd(')');dato_lcd('^');dato_lcd('-');dato_lcd('1');dato_lcd('.');dato_lcd('3');
		
//Distancia cm = 12,521(Voltaje)^-1,038		
		}
		if(num=='8'){		
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('D');dato_lcd('I');dato_lcd('S');dato_lcd('T');dato_lcd(':');dato_lcd(' ');cadena(pulg,4);dato_lcd('i');dato_lcd('n');
									
		comando_lcd(w_linea2);
		
		dato_lcd('D');dato_lcd('I');dato_lcd('S');dato_lcd('T');dato_lcd(' ');dato_lcd('I');dato_lcd('N');dato_lcd('=');dato_lcd(' ');
		dato_lcd('C');dato_lcd('M');dato_lcd('/');dato_lcd('2');dato_lcd('.');dato_lcd('5');dato_lcd('4');
		
		}
			
	}
		Verificar();

// ----------------------------------------------------------------------
// CASO C - POTENCIOMETRO CASE 3,6,9
if(ang==true){
		num = numero;
		valgrad=(97.859*voltaje3)+5.0903; //equivalencia en grados segun v	
		sprintf(grad,"%.4f ",valgrad); //pasa a char
			
		valrad=(valgrad*pi)/180;//° a rad
		sprintf(rad,"%.4f ",valrad); //pasa a char
			
		valrev=valgrad/360;//° a rev
		sprintf(rev,"%.4f ",valrev);//pasa a char
		
		if(voltaje3>=mui){ //umbral 1v=10°c
		comando_lcd(clear);
		comando_lcd(w_linea1);		
		dato_lcd('A');dato_lcd('L');dato_lcd('E');dato_lcd('R');dato_lcd('T');dato_lcd('A');
		comando_lcd(w_linea2);
		dato_lcd('U');dato_lcd('M');dato_lcd('B');dato_lcd('R');dato_lcd('A');dato_lcd('L');
		}
		
		comando_lcd(clear);
		comando_lcd(w_linea1);	
		dato_lcd('A');dato_lcd('N');dato_lcd('G');dato_lcd(':');dato_lcd(' ');
		cadena(rad,4);dato_lcd('r');dato_lcd('a');dato_lcd('d');							
					
		comando_lcd(w_linea2);
		dato_lcd('R');dato_lcd('A');dato_lcd('D');dato_lcd('=');dato_lcd(' ');dato_lcd('(');dato_lcd('°');dato_lcd('*'); 
		dato_lcd('P');dato_lcd('I');dato_lcd(')');dato_lcd('/');dato_lcd('1');dato_lcd('8');dato_lcd('0');
	  
		if(num == '3'){	
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('A');dato_lcd('N');dato_lcd('G');dato_lcd(':');dato_lcd(' ');
		cadena(rad,4);dato_lcd('r');dato_lcd('a');dato_lcd('d');							
					
		comando_lcd(w_linea2);
		dato_lcd('R');dato_lcd('A');dato_lcd('D');dato_lcd('=');dato_lcd(' ');dato_lcd('(');dato_lcd('°');dato_lcd('*'); 
		dato_lcd('P');dato_lcd('I');dato_lcd(')');dato_lcd('/');dato_lcd('1');dato_lcd('8');dato_lcd('0');
		
		limit[j] = Verificar();
		j++;
		}
		
		//Grados ° = 97,859(V) + 5,0903
		if(num =='6'){		
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('A');dato_lcd('N');dato_lcd('G');dato_lcd(':');dato_lcd(' ');
		cadena(grad,4);dato_lcd('°');
	
		comando_lcd(w_linea2);
		
		dato_lcd('G');dato_lcd('R');dato_lcd('A');dato_lcd('D');dato_lcd('=');dato_lcd(' '); 
		dato_lcd('(');dato_lcd('9');dato_lcd('8');dato_lcd('*');dato_lcd('A');dato_lcd('D');dato_lcd('C');dato_lcd(')');
		dato_lcd('+');dato_lcd('5');
				
		limit[j] = Verificar();
		j++;
		}
		
		//Revoluciones = 97,859(V) + 5,0903
		if(num =='9'){
		comando_lcd(clear);
		comando_lcd(w_linea1);
				
		dato_lcd('A');dato_lcd('N');dato_lcd('G');dato_lcd(':');dato_lcd(' ');
		cadena(rev,4);
		dato_lcd('r');dato_lcd('e');dato_lcd('v');							
					
		comando_lcd(w_linea2);
		dato_lcd('R');dato_lcd('E');dato_lcd('V');dato_lcd('=');dato_lcd(' ');
		dato_lcd('G');dato_lcd('R');dato_lcd('A');dato_lcd('D');dato_lcd('O');dato_lcd('S');
		dato_lcd('/');dato_lcd('3');dato_lcd('6');dato_lcd('0');
		
				
		limit[j] = Verificar();
		j++;
		}
		
} 
		Verificar();

if(esc==true){
   comando_lcd(clear);
	 for(int i=0; i<32; i++){
		 dato_lcd(limit[i]);
		 wait(500);
		 dato_lcd(' ');
		 dato_lcd(' ');
		 if(i == 15){
		 comando_lcd(w_linea2);
		 //dato_lcd(limit[i]);
		 }
	 }		
	 j=0;
}

	 if(numero == 'A'){
		comando_lcd(clear);
	 	comando_lcd(w_linea1);
		dato_lcd('L');dato_lcd('I');dato_lcd('M');dato_lcd('I');dato_lcd('T');dato_lcd('E');dato_lcd(' ');
	  dato_lcd('T');dato_lcd('E');dato_lcd('M');dato_lcd('P');dato_lcd(':');
		mod1=true;
		mod2=false;
		mod3=false;
		wait(1000);

	}
	   //Verificar();
	 
	if(numero == 'B'){
		comando_lcd(clear);
	 	comando_lcd(w_linea1);
		dato_lcd('L');dato_lcd('I');dato_lcd('M');dato_lcd('I');dato_lcd('T');dato_lcd('E');dato_lcd(' ');
		dato_lcd('D');dato_lcd('I');dato_lcd('S');dato_lcd('T');dato_lcd(':');
		mod1=false;
		mod2=true;
		mod3=false;
		wait(1000);
	}
		 // Verificar();
	
  if(numero == 'C'){
		comando_lcd(clear);
	 	comando_lcd(w_linea1);
		dato_lcd('L');dato_lcd('I');dato_lcd('M');dato_lcd('I');dato_lcd('T');dato_lcd('E');dato_lcd(' ');
		dato_lcd('A');dato_lcd('N');dato_lcd('G');dato_lcd(':');
		mod1=false;
		mod2=false;
		mod3=true;
		wait(1000);
	}
	   // Verificar();
	
	if(numero == 'D'){
		temp=false;
		dist=false;
		ang=false;
		esc=true;
	}
	
}}
