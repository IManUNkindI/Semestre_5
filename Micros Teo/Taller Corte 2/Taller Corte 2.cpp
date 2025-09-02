#include <stm32f767xx.h>
#include "SysTick.h"
#include "LCD-Comm-Write.h"
#include "I2C.h"

void configs();
float grados;
int n;
volatile float voltage = 0.0; // Variable para almacenar el valor del voltaje
float previousTemperature ;
float VSENSE ; // Suponiendo referencia de 3.3V
float V25 ; // Valor típico de VSENSE a 25°C (ajusta según las características de tu chip)
float Avg_Slope ; // Pendiente promedio (ajusta según las características de tu chip)
float temperature ;

int main(void) {
	SysTick_Init();
	configs();
	
  // Habilita el reloj para el módulo ADC1
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Habilita el reloj para el puerto A
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // Habilita el reloj para el ADC1

  // Configura el pin A0 como entrada analógica
  GPIOA->MODER |= GPIO_MODER_MODER0;

  // Habilita el sensor de temperatura
  ADC->CCR |= ADC_CCR_TSVREFE;

  // Selecciona el canal de entrada ADC1_IN18 (Sensor de temperatura)
	//ADC1->SQR3 &= ~ADC_SQR3_SQ1;
	//ADC1->SQR3 |= ADC_CHANNEL_TEMPSENSOR;
	ADC1->SQR1 &= ~ADC_SQR1_L;
	ADC1->SQR3 &= ~ADC_SQR3_SQ1;
	ADC1->SQR3 |= (18 << ADC_SQR3_SQ1_Pos);

  // Selecciona un tiempo de muestreo mayor que el mínimo (ajusta según tus necesidades)
  ADC1->SMPR1 |= ADC_SMPR1_SMP18;

  // Habilita el ADC
  ADC1->CR2 |= ADC_CR2_ADON;

  // Variable para almacenar la temperatura anterior
  previousTemperature = 0.0;


	// Inicializar el sistema y configurar el I2C2
  I2C2_Init();

  // Dirección del dispositivo I2C al que deseas enviar datos (0x3F en este caso)
  uint8_t deviceAddress = 0x3F;

  // Datos que deseas enviar
  uint8_t dataToSend = 0xAB; // Ejemplo de dato a enviar

	while (1) {
		// Inicia una conversión en ADC1 para medir la temperatura
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // Inicia una conversión en ADC2 para medir el voltaje
    ADC2->CR2 |= ADC_CR2_SWSTART;

    // Espera a que ambas conversiones finalicen
    while (!(ADC1->SR & ADC_SR_EOC));
    while (!(ADC2->SR & ADC_SR_EOC));

    // Lee los valores del registro de datos
    uint16_t adcValue1 = ADC1->DR; // Valor de temperatura
    uint16_t adcValue2 = ADC2->DR; // Valor de voltaje
		//3.2 v equivale a 35 grados
			
    // Calcula la temperatura (ajusta las fórmulas según las especificaciones de tu chip)
    VSENSE = (float)adcValue1 / 4095.0 * 3.3;
    V25 = 0.76;
    Avg_Slope = 2.5;
    temperature = ((VSENSE - V25) / Avg_Slope) * 1000;
    temperature = 10 + temperature;
		
    // Calcula el voltaje (ajusta las fórmulas según las especificaciones de tu chip)
    voltage = (float)adcValue2 * 3.3 / 4095;
				
				
		// CONDICIONES 
		// GRADOS ELEGIDOS
		grados=(float) voltage*35/3.3;
		/*	if(temperature>=grados)
			{
				GPIOB->ODR|=1<<0;
				n=5;
			}
			else if (grados>=temperature)
			{
				GPIOB->ODR|=0<<0;
				n=1;
			}*/
					
		if (temperature >= grados) {
			GPIOB->ODR |= 1 << 0; // Enciende el motor
			n = 5;
    } else {
			GPIOB->ODR &= ~(1 << 0); // Apaga el motor
      n = 1;
    }
		// Enviar los datos

		I2C2_SendData(dataToSend, deviceAddress);
    }
}
void configs(void){
	// ventilador 
	RCC->AHB1ENR|=(1<<1); // GPIO B habilitado
	GPIOB->MODER |=0x01; // conectar ventilador a b0
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// Configura el pin GPIOB_PIN0 como salida
  GPIOB->MODER |= GPIO_MODER_MODER0_0; // Configura el pin como salida

  // Configuración GPIO
  RCC->AHB1ENR|=(1<<0); // GPIO A habilitado
  GPIOA->MODER|=(1<<14)|(1<<15); // PA7 configurado como analógico<
	

  // Configuración del ADC
  RCC->APB2ENR|=(1<<9);// ADC2 habilitado
  ADC->CCR |= (0<<16); // PCLK2 dividido por 2
  ADC2->CR1 |= (0<<24); // 12 bits de resolución
  ADC2->CR2 |= (1<<0)|(1<<1)|(1<<5)|(0<<11); // ADC encendido, conversión continua, EOCS habilitado, alineación a la derecha.
  ADC2->SMPR1 |= (0<<0); // Sin tiempo de muestreo configurado (por defecto)
  ADC2->SMPR2 |= (7<<12); // ADC2 IN4 con 480 ciclos (ajusta según tus necesidades)
  ADC2->SQR1 |= (0<<0); // L=0, número de conversiones = 1
  ADC2->SQR2 |= (0<<0); // Nada
  ADC2->SQR3 |= (7<<0); // ADC2 IN4 como primera conversión
}