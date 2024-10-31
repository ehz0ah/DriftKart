#include "LED.h"


// Array to store the pin numbers of green LEDs
int green_led_array[10] = {GREEN_LED1, GREEN_LED2, GREEN_LED3, GREEN_LED4, 
                           GREEN_LED5, GREEN_LED6, GREEN_LED7, GREEN_LED8, 
                           GREEN_LED9, GREEN_LED10};

void initGPIO(void) {
    // Enable Clock to PortE and PortB
    SIM->SCGC5 |= (SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTB_MASK);

    // Configure MUX settings for RED LED on PortE and GREEN LEDs on PortE/PortB as GPIO
    PORTE->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
    PORTE->PCR[RED_LED] |= PORT_PCR_MUX(1);  // PortE Pin 1 as GPIO

    // Configure PortE LEDs (GREEN_LED1 to GREEN_LED6)
    for (int i = 0; i < 6; i++) {
        PORTE->PCR[green_led_array[i]] &= ~PORT_PCR_MUX_MASK;
        PORTE->PCR[green_led_array[i]] |= PORT_PCR_MUX(1);  // Set as GPIO
    }

    // Configure PortB LEDs (GREEN_LED7 to GREEN_LED10)
    for (int i = 6; i < GREEN_LED_COUNT; i++) {
        PORTB->PCR[green_led_array[i]] &= ~PORT_PCR_MUX_MASK;
        PORTB->PCR[green_led_array[i]] |= PORT_PCR_MUX(1);  // Set as GPIO
    }

    // Set RED LED as output
    PTE->PDDR |= MASK(RED_LED);

    // Set GREEN LEDs as output (PortE and PortB)
    for (int i = 0; i < 6; i++) {
        PTE->PDDR |= MASK(green_led_array[i]);  // Set PortE LEDs as output
    }
    for (int i = 6; i < GREEN_LED_COUNT; i++) {
        PTB->PDDR |= MASK(green_led_array[i]);  // Set PortB LEDs as output
    }

    // Turn off all LEDs initially
    PTE->PCOR |= MASK(RED_LED);
    for (int i = 0; i < 6; i++) {
        PTE->PCOR |= MASK(green_led_array[i]);  // Turn off PortE LEDs
    }
    for (int i = 6; i < GREEN_LED_COUNT; i++) {
        PTB->PCOR |= MASK(green_led_array[i]);  // Turn off PortB LEDs
    }
}

/*
void delay(uint32_t delayTime) {
    osDelay(delayTime);  // Use CMSIS-RTOS osDelay
}
*/

void BlinkRedLED(int redFrequency) {
    PTE->PSOR = MASK(RED_LED);  // Turn on RED LED
    osDelay(redFrequency);  // Delay based on frequency

    PTE->PCOR = MASK(RED_LED);  // Turn off RED LED
    osDelay(redFrequency);  // Delay based on frequency
}

void onGreen() {
    for (int i = 0; i < 6; i++) {
        PTE->PSOR |= MASK(green_led_array[i]);  // Turn on PortE GREEN LEDs
    }
    for (int i = 6; i < GREEN_LED_COUNT; i++) {
        PTB->PSOR |= MASK(green_led_array[i]);  // Turn on PortB GREEN LEDs
    }
}

void offGreen() {
    for (int i = 0; i < 6; i++) {
        PTE->PCOR |= MASK(green_led_array[i]);  // Turn off PortE GREEN LEDs
    }
    for (int i = 6; i < GREEN_LED_COUNT; i++) {
        PTB->PCOR |= MASK(green_led_array[i]);  // Turn off PortB GREEN LEDs
    }
}

void BlinkGreenLED() {
    offGreen();  // Ensure all LEDs are off initially

    for (int i = 0; i < 6; i++) {
        PTE->PSOR = MASK(green_led_array[i]);  // Turn on current PortE LED
        osDelay(100);  // 100ms delay
        PTE->PCOR = MASK(green_led_array[i]);  // Turn off current PortE LED
    }
    for (int i = 6; i < GREEN_LED_COUNT; i++) {
        PTB->PSOR = MASK(green_led_array[i]);  // Turn on current PortB LED
        osDelay(100);  // 100ms delay
        PTB->PCOR = MASK(green_led_array[i]);  // Turn off current PortB LED
    }
}

void LightGreenLED() {
    onGreen();  // Turn on all GREEN LEDs
}
