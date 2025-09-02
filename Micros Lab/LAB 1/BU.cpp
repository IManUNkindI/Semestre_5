#include <stm32f767xx.h>

//declaracion de variables
unsigned long SWT1;
unsigned long SWT2;
unsigned long BTNUSER;
unsigned long BTNPB8;
bool breaker = false;
bool start = false;
int count = 0;
int enablec = 0;
int delaym = 5;
int mode = 0;

//Metodos del selsccionador
void NamesHalloween();
void LedsHalloween();
void ContadorDisplay();
void Motor(int);
void Display(int);

//Metodo para delay
void delay(int32_t k){
	int32_t i, j ;
	for(i = 0; i < k ; i++){
		for(j = 0; j < 1000; j++);
	}
}

int main(){
	//Configurar Salidas
		//Habilitar CLK GPIOB, CLK GPIOD, CLK GPIOE
	RCC->AHB1ENR |= (0x1<<1);
	RCC->AHB1ENR |= (0x1<<3);
	RCC->AHB1ENR |= (0x1<<4);
	
		//Configurar Salidas
	GPIOB->MODER |= (0x1<<0);
	GPIOB->MODER &= ~(0x1<<1);
	GPIOB->MODER |= (0x1<<28);
	GPIOB->MODER &= ~(0x1<<29);
	GPIOB->MODER |= (0x1<<14);
	GPIOB->MODER &= ~(0x1<<15);
	
	GPIOD->MODER |= (0x1555);
	
	GPIOE->MODER |= (0x1540);
	
		// Push-pull mode
	GPIOB->OTYPER &= ~(0x1<<0);
	GPIOB->OTYPER &= ~(0x1<<7);
	GPIOB->OTYPER &= ~(0x1<<14);

	GPIOD->OTYPER &= ~(0x7F);
	
	GPIOE->OTYPER &= ~(0x78);
	
		// High Speed
	GPIOB->OSPEEDR &= ~(0x1<<0);
	GPIOB->OSPEEDR |= (0x1<<1);
	GPIOB->OSPEEDR &= ~(0x1<<14);
	GPIOB->OSPEEDR |= (0x1<<15);
	GPIOB->OSPEEDR &= ~(0x1<<28);
	GPIOB->OSPEEDR |= (0x1<<29);
	
	GPIOD->OSPEEDR |= (0x2AAA);
	
	GPIOE->OSPEEDR |= (0x2A80);
	
		// Disable pull-up and pull-down resistors
	GPIOB->PUPDR |= (0x00000000);
	GPIOD->PUPDR |= (0x00000000);
	GPIOE->PUPDR |= (0x00000000);
	// Salidas configuradas
	
	//Configuracion Entradas
		//Enable CLK GPIOA , CLK GPIOF, CLK GPIOC, CLK GPIOB
	RCC->AHB1ENR |= (0x1);
	RCC->AHB1ENR |= (0x1<<5);
	RCC->AHB1ENR |= (0x1<<2);
	
		//High Speed
	GPIOA->OSPEEDR |= (0x1<<7);
	GPIOA->OSPEEDR &= ~(0x1<<6);
	GPIOF->OSPEEDR |= (0x1<<25);
	GPIOF->OSPEEDR &= ~(0x1<<24);
	GPIOC->OSPEEDR |= (0x1<<27);
	GPIOC->OSPEEDR &= ~(0x1<<26);
	GPIOB->OSPEEDR |= (0x1<<17);
	GPIOB->OSPEEDR &= ~(0x1<<16);
	
	
		//Enable PULL-DOWN resistor
	GPIOA->PUPDR |= (0x1<<7);
	GPIOA->PUPDR &= ~(0x1<<6);
	GPIOF->PUPDR |= (0x1<<25);
	GPIOF->PUPDR &= ~(0x1<<24);
	GPIOC->PUPDR |= (0x1<<27);
	GPIOC->PUPDR &= ~(0x1<<26);
	
		//Enable PULL-UP resistor;
	GPIOB->PUPDR |= (0x1<<17);
	GPIOB->PUPDR &= ~(0x1<<16);
	
	//Entradas Configuradas
	
while(true){
	
	
	SWT1 = (GPIOA->IDR) & 0x0008;
	SWT2 = (GPIOF->IDR) & 0x1000;
	
	if( SWT1 == 0 && SWT2 == 0){
		mode = 1;

		GPIOB->BSRR |= (0x1<<0);
		GPIOB->BSRR |= (0x1<<23);
		GPIOB->BSRR |= (0x1<<30);
		NamesHalloween();
	}else if( SWT1 == 0 && SWT2 != 0){
		mode = 2;

		GPIOB->BSRR |= (0x1<<16);
		GPIOB->BSRR |= (0x1<<7);
		GPIOB->BSRR |= (0x1<<30);
		LedsHalloween();
	}else if( SWT1 != 0 && SWT2 == 0){
		mode = 3;
	
		GPIOB->BSRR |= (0x1<<16);
		GPIOB->BSRR |= (0x1<<23);
		GPIOB->BSRR |= (0x1<<14);
		ContadorDisplay();
	}else if( SWT1 != 0 && SWT2 != 0){
		mode = 4;

		GPIOB->BSRR |= (0x1<<0);
		GPIOB->BSRR |= (0x1<<7);
		GPIOB->BSRR |= (0x1<<14);
		Display(count);
		BTNUSER = (GPIOC->IDR) & 0x2000;
	while (BTNUSER != 0){
		BTNUSER = (GPIOC->IDR) & 0x2000;
		if (BTNUSER == 0){
			count++;
		}
		delay(100);
	}
		BTNPB8 = (GPIOB->IDR) & 0x100;
	while (BTNPB8 == 0){
		BTNPB8 = (GPIOC->IDR) & 0x100;
		if (BTNPB8 != 0){
			start = true;
		}
	}
		if(start == true){
			count = count * 512;
			delay(500);
			Motor(count);
			GPIOE->BSRR |= (0x1<<19);
			GPIOE->BSRR |= (0x1<<20);
			GPIOE->BSRR |= (0x1<<21);
			GPIOE->BSRR |= (0x1<<22);
			count = 0;
			breaker = false;
			start = false;
		}
	}
}
}
void NamesHalloween(){
	while(true){
		//DrACULA
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(13);//D
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(27);//R
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(10);//A
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(12);//C
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(30);//U
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(21);//L
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(10);//A
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(36);//Off
		delay(750);
		//JIgSAw
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(19);//J
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(18);//I
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(16);//G
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(28);//S
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(10);//A
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(32);//W
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(36);//Off
		delay(750);
		//CtHULHU
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(12);//C
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(29);//T
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(20);//H
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(30);//U
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(21);//L
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(20);//H
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(30);//U
		delay(750);
		SWT1 = (GPIOA->IDR) & 0x0008;
		SWT2 = (GPIOF->IDR) & 0x1000;
		if( SWT1 != 0 || SWT2 != 0){
		 break;
		}
		Display(36);//Off
		delay(750);
	}
	Display(36);
}
void LedsHalloween(){
}
void ContadorDisplay(){
	Display(count);
			BTNUSER = (GPIOC->IDR) & 0x2000;
	while (BTNUSER != 0){
		BTNUSER = (GPIOC->IDR) & 0x2000;
		if (BTNUSER == 0 && count < 15){
			count++;
		}else if(count == 15){
			count = 15;
		}
		delay(100);
	}
		BTNPB8 = (GPIOB->IDR) & 0x100;
	while(BTNPB8 == 0){
		BTNPB8 = (GPIOB->IDR) & 0x100;
		if (BTNPB8 != 0){	
			count = 0;
		}
	}
}
void Motor(int){
			for(count; count > 0; count--){
				
				BTNPB8 = (GPIOB->IDR) & 0x100;
				while (BTNPB8 == 0){
				BTNPB8 = (GPIOC->IDR) & 0x100;
					if (BTNPB8 != 0){
						breaker = true;
					}
				}
				if (breaker == true){
					break;
				}
				
					Display((count/512)+1);
				
				GPIOE->BSRR |= (0x1<<3);
				GPIOE->BSRR |= (0x1<<20);
				GPIOE->BSRR |= (0x1<<21);
				GPIOE->BSRR |= (0x1<<22);
				delay(delaym);
				
				GPIOE->BSRR |= (0x1<<19);
				GPIOE->BSRR |= (0x1<<4);
				GPIOE->BSRR |= (0x1<<21);
				GPIOE->BSRR |= (0x1<<22);
				delay(delaym);
				
				GPIOE->BSRR |= (0x1<<19);
				GPIOE->BSRR |= (0x1<<20);
				GPIOE->BSRR |= (0x1<<5);
				GPIOE->BSRR |= (0x1<<22);
				delay(delaym);
				
				GPIOE->BSRR |= (0x1<<19);
				GPIOE->BSRR |= (0x1<<20);
				GPIOE->BSRR |= (0x1<<21);
				GPIOE->BSRR |= (0x1<<6);
				delay(delaym);
			}
			Display(0);
		}
void Display(int count){
	switch(count){
		case 0://0
			GPIOD->BSRR |= (0x0040003F);
		break;
		case 1://
			GPIOD->BSRR |= (0x00790006);
		break;
		case 2://2
			GPIOD->BSRR |= (0x0024005B);
		break;
		case 3://3
			GPIOD->BSRR |= (0x0030004F);
		break;
		case 4://4
			GPIOD->BSRR |= (0x00190066);
		break;
		case 5://5
			GPIOD->BSRR |= (0x0012006D);
		break;
		case 6://6
			GPIOD->BSRR |= (0x0002007D);
		break;
		case 7://7
			GPIOD->BSRR |= (0x00780007);
		break;
		case 8://8
			GPIOD->BSRR |= (0x0000007F);
		break;
		case 9://9
			GPIOD->BSRR |= (0x00180067);
		break;
		case 10://A
			GPIOD->BSRR |= (0x00080077);
		break;
		case 11://B
			GPIOD->BSRR |= (0x0003007C);
		break;
		case 12://C
			GPIOD->BSRR |= (0x00460039);
		break;
		case 13://D
			GPIOD->BSRR |= (0x0021005E);
		break;
		case 14://E
			GPIOD->BSRR |= (0x00060079);
		break;
		case 15://F
			GPIOD->BSRR |= (0x000E0071);
		break;
		case 16://G
			GPIOD->BSRR |= (0x0010006F);
		break;
		case 17://H
			GPIOD->BSRR |= (0x00090076);
		break;
		case 18://I
			GPIOD->BSRR |= (0x004F0030);
		break;
		case 19://J
			GPIOD->BSRR |= (0x0061001E);
		break;
		case 20://K
			GPIOD->BSRR |= (0x00090076);
		break;
		case 21://L
			GPIOD->BSRR |= (0x00470038);
		break;
		case 22://M
			GPIOD->BSRR |= (0x006A0015);
		break;
		case 23://N
			GPIOD->BSRR |= (0x002B0054);
		break;
		case 24://O
			GPIOD->BSRR |= (0x0040003F);
		break;
		case 25://P
			GPIOD->BSRR |= (0x000C0073);
		break;
		case 26://Q
			GPIOD->BSRR |= (0x00180067);
		break;
		case 27://R
			GPIOD->BSRR |= (0x002F0050);
		break;
		case 28://S
			GPIOD->BSRR |= (0x0012006D);
		break;
		case 29://T
			GPIOD->BSRR |= (0x00070078);
		break;
		case 30://U
			GPIOD->BSRR |= (0x0041003E);
		break;
		case 31://V
			GPIOD->BSRR |= (0x0063001C);
		break;
		case 32://W
			GPIOD->BSRR |= (0x0055002A);
		break;
		case 33://X
			GPIOD->BSRR |= (0x00090076);
		break;
		case 34://Y
			GPIOD->BSRR |= (0x0011006E);
		break;
		case 35://Z
			GPIOD->BSRR |= (0x0024005B);
		break;
		case 36://Off
			GPIOD->BSRR |= (0xFFFF0000);
		default:
			GPIOD->ODR |= (0xFFFF0000);
	}
}