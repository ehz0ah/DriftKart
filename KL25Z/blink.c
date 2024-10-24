#include "blink.h"

// Semaphore variablesosSemaphoreId_t greenledSem;
volatile int blinkingActive = 1;
volatile int redFrequency = 500;
// Array to store the pin numbers of green LEDs
int green_led_array[10] = {GREEN_LED1, GREEN_LED2, GREEN_LED3, GREEN_LED4, 
                           GREEN_LED5, GREEN_LED6, GREEN_LED7, GREEN_LED8,
														GREEN_LED9, GREEN_LED10};
void InitGPIO(void) {
  // Enable Clock to PortE    
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
  // Configure MUX settings for RED and GREEN LEDs as GPIO
  PORTE->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK; 
	PORTE->PCR[RED_LED] |= PORT_PCR_MUX(1);  // PortE Pin 1 as GPIO
	
	for (int i = 0; i < GREEN_LED_COUNT; i++) {
		PORTE->PCR[green_led_array[i]] &= ~PORT_PCR_MUX_MASK;
		PORTE->PCR[green_led_array[i]] |= PORT_PCR_MUX(1);  // Set as GPIO
	}
	
	// Set RED and GREEN LEDs as output    PTE->PDDR |= MASK(RED_LED);
	for (int i = 0; i < GREEN_LED_COUNT; i++) {   
		PTE->PDDR |= MASK(green_led_array[i]);
	}
	
	// Turn off all LEDs initially    PTE->PCOR |= MASK(RED_LED);
	for (int i = 0; i < GREEN_LED_COUNT; i++) { 
		PTE->PCOR |= MASK(green_led_array[i]);
	}
}
void delay(uint32_t delayTime) {  
  osDelay(delayTime);  // Use CMSIS-RTOS osDelay
}

void checkFlag(void *argument) {  
  int count = 1;
  while (1) {  
		osSemaphoreRelease(greenledSem);  // Release the green LED semaphore
    if (count) {  
			redFrequency = 500;
    } else {    
			redFrequency = 250;
    }   
		count = (count + 1) % 2;
    delay(5500);  // 5.5-second delay    }
}
	
void BlinkRedLED(void *argument) {  
  while (1) {
		PTE->PSOR = MASK(RED_LED);  // Turn on RED LED        delay(redFrequency);  // Delay based on frequency
		PTE->PCOR = MASK(RED_LED);  // Turn off RED LED
		delay(redFrequency);  // Delay based on frequency    }
}
void onGreen() {  
		for (int i = 0; i < GREEN_LED_COUNT; i++) {
        PTE->PSOR |= MASK(green_led_array[i]);  // Turn on all GREEN LEDs    }
}
void offGreen() {    for (int i = 0; i < GREEN_LED_COUNT; i++) {
        PTE->PCOR |= MASK(green_led_array[i]);  // Turn off all GREEN LEDs    }
}
void BlinkGreenLED(void *argument) {    while (1) {
        osSemaphoreAcquire(greenledSem, osWaitForever);  // Wait for semaphore        int currentLEDIndex = 0;
        offGreen();        blinkingActive = 1;
        while (blinkingActive) {
            PTE->PSOR = MASK(green_led_array[currentLEDIndex]);  // Turn on current LED            delay(100);  // 100ms delay
            PTE->PCOR = MASK(green_led_array[currentLEDIndex]);  // Turn off current LED
            currentLEDIndex = (currentLEDIndex + 1) % GREEN_LED_COUNT;  // Move to next LED        }
        onGreen();  // Turn on all LEDs at the end    }
}
void LightGreenLED(void *argument) {    while (1) {
        osSemaphoreAcquire(greenledSem, osWaitForever);  // Wait for semaphore        blinkingActive = 0;
        onGreen();  // Turn on all GREEN LEDs    }
}