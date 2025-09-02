void I2C2_Init(void)
{
    // Habilitar el reloj para el módulo I2C2
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // Configurar los pines PF0 (SCL) y PF1 (SDA) como alternancia de funciones en modo AF4
    GPIOF->AFR[0] |= (4 << (4 * 0)); // PF0 (SCL)
    GPIOF->AFR[0] |= (4 << (4 * 1)); // PF1 (SDA)
    GPIOF->MODER |= (2 << (2 * 0)); // PF0 (SCL) en modo alternancia
    GPIOF->MODER |= (2 << (2 * 1)); // PF1 (SDA) en modo alternancia

    // Configurar la velocidad del reloj del I2C (16MHz)
    I2C2->TIMINGR = 0x00701D2B;

    // Habilitar el I2C2
    I2C2->CR1 |= I2C_CR1_PE;
}

void I2C2_Start(void)
{
    // Generar una señal de inicio
    I2C2->CR2 |= I2C_CR2_START;
    while (!(I2C2->ISR & I2C_ISR_TXE)); // Esperar hasta que el buffer de transmisión esté vacío
}

void I2C2_Stop(void)
{
    // Generar una señal de parada
    I2C2->CR2 |= I2C_CR2_STOP;
    while (I2C2->CR2 & I2C_CR2_STOP); // Esperar hasta que la señal de parada se complete
}

void I2C2_SendData(uint8_t data, uint8_t address)
{
    // Enviar un byte de datos a la dirección especificada
    I2C2_Start();
    I2C2->TXDR = (address << 1) & 0xFE; // Dirección del dispositivo en modo de escritura
    while (!(I2C2->ISR & I2C_ISR_TXE)); // Esperar hasta que el buffer de transmisión esté vacío
    I2C2->TXDR = data; // Datos a enviar
    while (!(I2C2->ISR & I2C_ISR_TXE)); // Esperar hasta que el buffer de transmisión esté vacío
    I2C2_Stop();
}