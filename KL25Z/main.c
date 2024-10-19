#include "lib.h"
#include "queue.h"
#include "uart.h"
#include "pwm.h"

int speed = 0;
uint8_t command = 0;
uint8_t spin = 0;

osEventFlagsId_t runEndEvent;

CircularBuffer rx_queue; // Global buffer instance

void UART2_IRQHandler(void) {
	if (UART2->S1 & UART_S1_RDRF_MASK) {  // Check if receive data							 
		buffer_put(&rx_queue, UART2->D); // Insert into circular buffer
	}
}

void parser_thread(void* argument) {
	for (;;) {
		parseData(&rx_queue, &speed, &command, &spin, runEndEvent);
	}
}

void pwm_thread(void* argument) {
	for (;;) {
		move(command, spin, speed);
	}
}

const osThreadAttr_t parser_thread_attr = {
  .priority = osPriorityHigh                      
};

const osThreadAttr_t pwm_thread_attr = {
  .priority = osPriorityHigh                      
};


int main(void) {
	
	SystemCoreClockUpdate();
	//SystemInit();
	initQueue(&rx_queue);
	initUART2(BAUD_RATE);
	initPWM();
	
	osKernelInitialize();
	runEndEvent = osEventFlagsNew(NULL);
	osEventFlagsSet(runEndEvent,0x01);  // set bit 0 to run main music, bit 1 to run end music
	osThreadNew(parser_thread, NULL, &parser_thread_attr);
	osThreadNew(pwm_thread, NULL, &pwm_thread_attr);
	osKernelStart();
	
	for (;;) {}
		
}
