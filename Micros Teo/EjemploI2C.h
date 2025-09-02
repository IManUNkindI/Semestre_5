#include <stdio.h>
#include <stm32f7xx.h>
#define FREQTRAB 16000000
#define PCFdir 0x27
//MPU
#define MPU6050_ADDR         0x68 
#define MPU6050_SMPLRT_DIV   0x19 
#define MPU6050_CONFIG       0x1A
#define MPU6050_GYRO_CONFIG  0x1B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_WHO_AM_I     0x75
#define MPU6050_PWR_MGMT_1   0x6B

int rx_data[1];
int accelXH[1];
int accelXL[1];
int tx_data[1];
void DBG_Pins(void);
void INIT_I2C(void);
void SysTick_Init(void);
void SysTick_Wait(uint32_t n);
void SysTick_Wait1ms(uint32_t delay);
int	I2C1_Lee(int direccion, int reg_dir, int *buffer, int nbytes );
int	I2C1_Escribe( int direccion, int reg_dir, int *buffer, int nbytes );


int dirs=0;
int devs[127];
int	ndev=0;
volatile int W_OK,R_OK;
void configMPU(void);
int16_t accum;
double raw_acc_x;
int main(void)
{ 
	SysTick_Init();
	INIT_I2C();
	SysTick_Wait1ms(1000);
	configMPU();
	SysTick_Wait1ms(1000);

	while(1)
	{
		
// BUSCAR DISPOSITIVOS		
	ndev=0;
	for (dirs=0;dirs<127;dirs++){
	tx_data[0]=0x01;
	W_OK=I2C1_Escribe(dirs, 0x0, tx_data, 1);
	
	if(W_OK==0){	
	devs[ndev]=dirs;
	ndev++;	
	}
	SysTick_Wait1ms(100);
	}		

		
// EJEMPLO PCF85741	
		for (int i=0;i<8;i++){
		tx_data[0]=0x0F;
		W_OK=I2C1_Escribe(PCFdir, 0x0, tx_data, 1);
		SysTick_Wait1ms(2000);

	}
		
// LEER DATOS MPU

	accum=0;
	I2C1_Lee(MPU6050_ADDR, 0x3D, rx_data, 1);
	accum = rx_data[0]<<8;
	SysTick_Wait1ms(100);	
	I2C1_Lee(MPU6050_ADDR, 0x3E, rx_data, 1);	
	accum += rx_data[0];
	SysTick_Wait1ms(100);	
	raw_acc_x = (accum);	
	SysTick_Wait1ms(100);	


	}
}

void INIT_I2C(){
RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;

GPIOB->MODER |= (2<<16) | (2<<18);
GPIOB->OTYPER|= (1<<8) | (1<<9);	
GPIOB->OSPEEDR |= (3<<16) | (3<<18);
GPIOB->AFR[1] |= (4<<0) | (4<<4);
RCC->DCKCFGR2 |= (2<<16);
	
RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;
I2C1->TIMINGR |= 0x30420F13;
// Enable I2C1
	I2C1->CR1 |= 0x7;
}

void DBG_Pins(){
RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
GPIOA->MODER |= (1<<0) | (1<<2)| (1<<4);
}

int	I2C1_Lee(int direccion, int reg_dir, int *buffer, int nbytes ) {
	uint32_t 	t_espera;
	uint8_t		n;			// Contador para la lectura de los nbytes

	// Dirección del dispositivo
	I2C1->CR2 &= ~I2C_CR2_SADD_Msk;
	I2C1->CR2 |= ((direccion <<1U) <<I2C_CR2_SADD_Pos);

  // i2c Modo Escritura
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;

	I2C1->CR2 &= ~I2C_CR2_NBYTES;
	I2C1->CR2 |= (1 <<16U);
	I2C1->CR2 &= ~I2C_CR2_AUTOEND;

	// Iniciar charla I2C 
	I2C1->CR2 |= I2C_CR2_START;

	// Espera a TXIS o se sale del while y devuelve un 1
	// SI devuelve 1 significa que no se inicio la charla
	t_espera = 200000;
	while (((I2C1->ISR) & I2C_ISR_TXIS) != I2C_ISR_TXIS)
	{
		t_espera--;
		if (t_espera == 0) return 1;
	}

	// Envia la dirección del registro que se va a leer
	I2C1->TXDR = reg_dir;

	// Espera a TC o se sale del while y devuelve un 2
	t_espera = 200000;
	while (((I2C1->ISR) & I2C_ISR_TC) != I2C_ISR_TC)
	{
		t_espera--;
		if (t_espera == 0) return 2;
	}

	// i2c en modo lectura
	I2C1->CR2 |= I2C_CR2_RD_WRN;
  
	I2C1->CR2 &= ~I2C_CR2_NBYTES;
	I2C1->CR2 |= (nbytes <<16U);
	I2C1->CR2 &= ~I2C_CR2_AUTOEND;

	// Se repite la condición de inicio para indicar que se leen
	// nbytes
	I2C1->CR2 |= I2C_CR2_START;

	n = nbytes;

	while (n>0)
	{
		// Espera a RXNE o se sale del while y devuelve un 3
		t_espera = 200000;
		while (((I2C1->ISR) & I2C_ISR_RXNE) != I2C_ISR_RXNE)
		{
			t_espera--;
			if (t_espera == 0) return 3;
		}

		// Se guardan los datos en buffer.
		// Buffer debe tener tantas posiciones como datos a leer
		*buffer = I2C1->RXDR;
		buffer++;
		n--;
	}

	// para i2c
	I2C1->CR2 |= I2C_CR2_STOP;

	// Espera a STOPF o se sale del while y devuelve un 4
	t_espera = 200000;
	while (((I2C1->ISR) & I2C_ISR_STOPF) != I2C_ISR_STOPF)
	{
		t_espera--;
		if (t_espera == 0) return 4;
	}

	// Todo OK, todo correcto y yo retorno 0.
	return 0;
}



int	I2C1_Escribe( int direccion, int reg_dir, int *buffer, int nbytes ){
	
	uint32_t 	t_espera;	// t_espera
	uint8_t		n;		// Contador para la lectura de datos

	// Dirección del esclavo
	I2C1->CR2 &= ~I2C_CR2_SADD_Msk;
	I2C1->CR2 |= ((direccion <<1) <<I2C_CR2_SADD_Pos);

	// i2c modo escritura
	I2C1->CR2 &= ~I2C_CR2_RD_WRN;
	I2C1->CR2 &= ~I2C_CR2_NBYTES;
	I2C1->CR2 |= ((nbytes+1) <<16);
	I2C1->CR2 |= I2C_CR2_AUTOEND;

	// Limpiar bandera de STOP, por si acaso
	I2C1->ICR |= I2C_ICR_STOPCF;

	// iniciar i2c
	I2C1->CR2 |= I2C_CR2_START;

	// Espera lla bandera TXIS o que se acabe el tiempo de espera
	t_espera = 200000;
	while (((I2C1->ISR) & I2C_ISR_TXIS) != I2C_ISR_TXIS)
	{
		t_espera--;
		if (t_espera == 0) return 1;
	}

	// Dirección del registro que se quiere modificar
	I2C1->TXDR = reg_dir;

	n = nbytes;

	while(n>0)
	{
		// Espera la bandera TXIS o que se acabe el tiempo de espera
		t_espera = 100000;
		while (((I2C1->ISR) & I2C_ISR_TXIS) != I2C_ISR_TXIS)
		{
			t_espera--;
			if (t_espera == 0) return 2;
		}

		// Se envian los datos en el array llamado buffer
		I2C1->TXDR = *buffer;
		buffer++;
		n--; // La n que cuenta el numero de datos
	}

	// Hasta que este STOPF o se cumpla el t_espera
	t_espera = 200000;
	while (((I2C1->ISR) & I2C_ISR_STOPF) != I2C_ISR_STOPF)
	{
		t_espera--;
		if (t_espera == 0) return 3;
	}

	// Si todo sale bien seria 0
	return 0;
}

void configMPU(void){

	tx_data[0]=0x08;
	I2C1_Escribe(MPU6050_ADDR, MPU6050_GYRO_CONFIG, tx_data, 1);	
		
	tx_data[0]=0x01;
	I2C1_Escribe(0x68, 0x6B, tx_data, 1);	
	
	}
