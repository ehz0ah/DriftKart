#include "MKL25Z4.h"
#include "cmsis_os2.h"

// Define the pins for the LEDs
#define RED_LED      1    // PortE Pin 1
#define GREEN_LED1   30   // PortE Pin 30
#define GREEN_LED2   29   // PortE Pin 29
#define GREEN_LED3   23   // PortE Pin 23
#define GREEN_LED4   22   // PortE Pin 22
#define GREEN_LED5   21   // PortE Pin 21
#define GREEN_LED6   20   // PortE Pin 20
#define GREEN_LED7   5    // PortE Pin 5
#define GREEN_LED8   4    // PortE Pin 4
#define GREEN_LED9   3    // PortE Pin 3
#define GREEN_LED10  2    // PortE Pin 2
#define MASK(x)      (1 << (x))
#define GREEN_LED_COUNT 10  // Total number of green LEDs

extern osSemaphoreId_t redledSem;
extern osSemaphoreId_t greenledSem;extern volatile int blinkingActive;
extern volatile int redFrequency;extern int green_led_array[10];

// Function declarations
void InitGPIO(void);void delay(uint32_t delayTime);
void checkFlag(void *argument);void BlinkRedLED(void *argument);
void BlinkGreenLED(void *argument);void LightGreenLED(void *argument);
void onGreen(void);void offGreen(void);