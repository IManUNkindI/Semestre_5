void I2C2_Init(){
	
		// Configurar los pines PF0 (SCL) y PF1 (SDA) como alternancia de funciones en modo AF4
	RCC->AHB1ENR |= 0x20;
  GPIOF->MODER |= 0xA; // PF0 (SCL)  PF1 (SDA) en modo alternancia
	GPIOF->OTYPER |= 0x3; // Open-Drain
	GPIOF->OSPEEDR |= 0xFFFFFFFF; // Very high speed
	GPIOF->PUPDR |= 0x5; // Pull-Up
	GPIOF->AFR[0] |= 0x44; // PF0 (SCL)  PF1 (SDA)
	
	  // Habilitar el reloj para el módulo I2C2
	RCC->DCKCFGR2 |= (2<<18);

  RCC->APB1ENR |= (0x1<<22);
  I2C2->TIMINGR = 0x30420F13;

  // Habilitar el I2C2
	I2C2->CR1 |= 0x7;
}
int	I2C2_Read(int direccion, int reg_dir, int *buffer, int nbytes ) {
	uint32_t 	t_espera;
	uint8_t		n;			// Contador para la lectura de los nbytes

	// Dirección del dispositivo
	I2C2->CR2 &= ~I2C_CR2_SADD_Msk;
	I2C2->CR2 |= ((direccion <<1U) <<I2C_CR2_SADD_Pos);

  // i2c Modo Escritura
	I2C2->CR2 &= ~I2C_CR2_RD_WRN;

	I2C2->CR2 &= ~I2C_CR2_NBYTES;
	I2C2->CR2 |= (1 <<16U);
	I2C2->CR2 &= ~I2C_CR2_AUTOEND;

	// Iniciar charla I2C 
	I2C2->CR2 |= I2C_CR2_START;

	// Espera a TXIS o se sale del while y devuelve un 1
	// SI devuelve 1 significa que no se inicio la charla
	t_espera = 200000;
	while (((I2C2->ISR) & I2C_ISR_TXIS) != I2C_ISR_TXIS){
		t_espera--;
		if (t_espera == 0) return 1;
	}

	// Envia la dirección del registro que se va a leer
	I2C2->TXDR = reg_dir;

	// Espera a TC o se sale del while y devuelve un 2
	t_espera = 200000;
	while (((I2C2->ISR) & I2C_ISR_TC) != I2C_ISR_TC){
		t_espera--;
		if (t_espera == 0) return 2;
	}

	// i2c en modo lectura
	I2C2->CR2 |= I2C_CR2_RD_WRN;
  
	I2C2->CR2 &= ~I2C_CR2_NBYTES;
	I2C2->CR2 |= (nbytes <<16U);
	I2C2->CR2 &= ~I2C_CR2_AUTOEND;

	// Se repite la condición de inicio para indicar que se leen
	// nbytes
	I2C2->CR2 |= I2C_CR2_START;

	n = nbytes;

	while (n>0){
		// Espera a RXNE o se sale del while y devuelve un 3
		t_espera = 200000;
		while (((I2C2->ISR) & I2C_ISR_RXNE) != I2C_ISR_RXNE){
			t_espera--;
			if (t_espera == 0) return 3;
		}

		// Se guardan los datos en buffer.
		// Buffer debe tener tantas posiciones como datos a leer
		*buffer = I2C2->RXDR;
		buffer++;
		n--;
	}

	// para i2c
	I2C2->CR2 |= I2C_CR2_STOP;

	// Espera a STOPF o se sale del while y devuelve un 4
	t_espera = 200000;
	while (((I2C2->ISR) & I2C_ISR_STOPF) != I2C_ISR_STOPF){
		t_espera--;
		if (t_espera == 0) return 4;
	}

	// Todo OK, todo correcto y yo retorno 0.
	return 0;
}
int	I2C2_Write( int direccion, int reg_dir, int *buffer, int nbytes ){
	
	uint32_t 	t_espera;	// t_espera
	uint8_t		n;		// Contador para la lectura de datos

	// Dirección del esclavo
	I2C2->CR2 &= ~I2C_CR2_SADD_Msk;
	I2C2->CR2 |= ((direccion <<1) <<I2C_CR2_SADD_Pos);

	// i2c modo escritura
	I2C2->CR2 &= ~I2C_CR2_RD_WRN;
	I2C2->CR2 &= ~I2C_CR2_NBYTES;
	I2C2->CR2 |= ((nbytes+1) <<16);
	I2C2->CR2 |= I2C_CR2_AUTOEND;

	// Limpiar bandera de STOP, por si acaso
	I2C2->ICR |= I2C_ICR_STOPCF;

	// iniciar i2c
	I2C2->CR2 |= I2C_CR2_START;

	// Espera lla bandera TXIS o que se acabe el tiempo de espera
	t_espera = 200000;
	while (((I2C2->ISR) & I2C_ISR_TXIS) != I2C_ISR_TXIS){
		t_espera--;
		if (t_espera == 0) return 1;
	}

	// Dirección del registro que se quiere modificar
	I2C2->TXDR = reg_dir;

	n = nbytes;

	while(n>0){
		// Espera la bandera TXIS o que se acabe el tiempo de espera
		t_espera = 100000;
		while (((I2C2->ISR) & I2C_ISR_TXIS) != I2C_ISR_TXIS){
			t_espera--;
			if (t_espera == 0) return 2;
		}

		// Se envian los datos en el array llamado buffer
		I2C2->TXDR = *buffer;
		buffer++;
		n--; // La n que cuenta el numero de datos
	}

	// Hasta que este STOPF o se cumpla el t_espera
	t_espera = 200000;
	while (((I2C2->ISR) & I2C_ISR_STOPF) != I2C_ISR_STOPF){
		t_espera--;
		if (t_espera == 0) return 3;
	}

	// Si todo sale bien seria 0
	return 0;
}