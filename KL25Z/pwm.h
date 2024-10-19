#include "MKL25Z4.h"                    // Device header

#define PWM_PERIOD 7500 // Set PWM period for TPM

#define Front_Left_Pin_1 0  // Port D, Pin 0 (PWM)
#define Front_Left_Pin_2 1   // Port D, Pin 1 (PWM)

#define Back_Left_Pin_1 2    // Port D, Pin 2 (PWM) (main signal)
#define Back_Left_Pin_2 3    // Port D, Pin 3 (PWM)

#define Front_Right_Pin_1 4   // Port D, Pin 4 (PWM) (main signal)
#define Front_Right_Pin_2 5   // Port D, Pin 5 (PWM)

#define Back_Right_Pin_1 12   // Port A, Pin 12 (PWM)
#define Back_Right_Pin_2 13   // Port A, Pin 13 (PWM)


typedef enum {
	LF,  // Forward Left Forward
	LR,
	RF,
	RR,
	//BLF,
	//BLR,  // No need
	//BRF,
	//BRR
} wheelsDirection;

void initPWM(void);

/*
void longDelay(volatile uint32_t delay);
*/


void shortDelay(volatile uint32_t nof);


void setWheels(wheelsDirection dir, int speed);

void forward(int speed);

void reverse(int speed);

void spinLeft(int speed);

void spinRight(int speed);

void curveLeft(int speed, double diff);

void curveRight(int speed, double diff);

void stop(void);

void move(uint8_t command, uint8_t spin, int speed);