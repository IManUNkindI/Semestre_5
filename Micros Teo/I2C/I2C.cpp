#include <stm32f767xx.h>
#include "SysTick.h"
#include "I2C_Config.h"

#define deviceAddress  	0x3F// Dirección del dispositivo I2C al que deseas enviar datos (0x3F en este caso)
#define Address  				0xAB
#define Address1  			0xC8

// Datos que deseas enviar
int dataToSend[1]; // Ejemplo de dato a enviar
int dataToSend1[1]; // Ejemplo de dato a enviar

int main(void)
{
  // Inicializar el sistema y configurar el I2C2
  I2C2_Init();
	
   while (1)
  {
		// Enviar los datos
		dataToSend[0] = 0x1;
		dataToSend1[0] = 0x8;
		I2C2_Write(deviceAddress, Address, dataToSend, 1);
		I2C2_Write(deviceAddress, Address1, dataToSend1, 1);
  }
}