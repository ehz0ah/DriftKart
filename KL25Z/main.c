#include "lib.h"
#include "queue.h"
#include "uart.h"
#include "pwm.h"
#include "buzzer.h"
#include "led.h"

volatile uint8_t running;  // Used for LED thread

osEventFlagsId_t runEndEvent;

osMessageQueueId_t motorMessage, robotMessage;

typedef struct {
	uint8_t command;
	uint8_t throttle;
} dataPacket;

void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	if (UART2->S1 & UART_S1_RDRF_MASK) {  // Check if receive data
		dataPacket data;
		uint8_t rxData = UART2->D;
		data.command = rxData & 0x0F;
		data.throttle = rxData & 0x10;
		osMessageQueuePut(robotMessage, &data, NULL, 0); // Send data to main robot control to execute command
	}
	PORTD->ISFR = 0xffffffff;
}

void parser_thread(void* argument) {
	dataPacket rxData;
	for (;;) {
		osMessageQueueGet(robotMessage, &rxData, NULL, osWaitForever);
		if (rxData.command == 0x00) {
			running = 0;
		} else {
			running = 1;
		}
		
		if (rxData.command == 0x0D) {   // Signal end
			osEventFlagsClear(runEndEvent, 0x01);
			osEventFlagsSet(runEndEvent, 0x02);  // bit 1 controls playing of end music
		}
		
		osMessageQueuePut(motorMessage, &rxData, NULL, 0);  
	}
}

void pwm_thread(void* argument) {
	dataPacket rxData;
	for (;;) {
		osMessageQueueGet(motorMessage, &rxData, NULL, osWaitForever);
		int speed = rxData.throttle ? FULL_SPEED : SLOW_SPEED;
		move(rxData.command, speed);   // remove spin
	}
}

void run_music_thread(void* argument) {
	for(;;) {
		playMelodyC();
	}
}

void end_music_thread(void* argument) {
	for(;;) {
		playMelodyB();
	}
}

void green_led_thread(void* argument) {
	for(;;) {
		if (running) {
			BlinkGreenLED();
		} else {
			LightGreenLED();
		}
	}
}

void red_led_thread(void* argument) {
	for(;;) {
		if (running) {
			BlinkRedLED(500);
		} else {
			BlinkRedLED(250);
		}
	}
}

int main(void) {
	
	SystemCoreClockUpdate();
	initUART2(BAUD_RATE);
	initPWM();
	initGPIO();
	initBuzzer();
	
	osKernelInitialize();
	
	runEndEvent = osEventFlagsNew(NULL);
	osEventFlagsSet(runEndEvent,0x01);
	robotMessage = osMessageQueueNew(1, sizeof(dataPacket), NULL);
	motorMessage = osMessageQueueNew(1, sizeof(dataPacket), NULL);
	osThreadNew(parser_thread, NULL, NULL);
	osThreadNew(pwm_thread, NULL, NULL);
	osThreadNew(red_led_thread, NULL, NULL);
	osThreadNew(green_led_thread, NULL, NULL);
	osThreadNew(run_music_thread, NULL, NULL);
	osThreadNew(end_music_thread, NULL, NULL);
	
	osKernelStart();
	
	for (;;) {}
	
}
