#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os2.h"
#include "queue.h"
#include "uart.h"
#include "pwm.h"

int leftSpeed = 0;
int rightSpeed = 0;
uint8_t command = 0;

CircularBuffer rx_queue; // Global buffer instance

void UART2_IRQHandler(void)
{
	if (UART2->S1 & UART_S1_RDRF_MASK)
	{									 // Check if receive data register is full
		buffer_put(&rx_queue, UART2->D); // Insert into circular buffer
	}
}

int test(void)
{
	SystemCoreClockUpdate();
	initPWM();
	while (1)
	{
		spinLeft(5000);
		shortDelay(0x80000);
		spinRight(5000);
		shortDelay(0x80000);
	}
}
