#include "lib.h"

//#include "MKL25Z4.h"
//#include "cmsis_os2.h"  // CMSIS-RTOS API

// Define the pins for the LEDs
#define RED_LED      1    // PortE Pin 1
#define GREEN_LED1   21   // PortE Pin 21
#define GREEN_LED2   20   // PortE Pin 20
#define GREEN_LED3   5   // PortE Pin 5
#define GREEN_LED4   4   // PortE Pin 4
#define GREEN_LED5   3   // PortE Pin 3
#define GREEN_LED6   2   // PortE Pin 2
#define GREEN_LED7   11    // PortB Pin 11
#define GREEN_LED8   10    // PortB Pin 10
#define GREEN_LED9   9    // PortB Pin 9
#define GREEN_LED10  8    // PortB Pin 8

#define MASK(x)      (1 << (x))
#define GREEN_LED_COUNT 10  // Total number of green LEDs

// Function declarations
void initGPIO(void);
//void ledDelay(uint32_t delayTime);
void BlinkRedLED(int redFrequency);
void BlinkGreenLED(void);
void LightGreenLED(void);
void onGreen(void);
void offGreen(void);
