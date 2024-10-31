#include "MKL25Z4.h" // Device header
#include "pwm.h"

// Initialize PWM for both motor drivers
void initPWM(void)
{
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK);

	// Enable Clock Gating for Timer 0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;


	// Front Right Motor Pins
	PORTD->PCR[Front_Right_Pin_1] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[Front_Right_Pin_1] |= PORT_PCR_MUX(4);

	PORTD->PCR[Front_Right_Pin_2] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[Front_Right_Pin_2] |= PORT_PCR_MUX(4);

	// Back Left Motor Pins
	PORTD->PCR[Back_Left_Pin_1] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[Back_Left_Pin_1] |= PORT_PCR_MUX(4);

	PORTD->PCR[Back_Left_Pin_2] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[Back_Left_Pin_2] |= PORT_PCR_MUX(4);

	// Set Modulo Value
	TPM0->MOD = PWM_PERIOD;

	TPM0_C2V = 0;
	TPM0_C3V = 0;

	TPM0_C4V = 0;
	TPM0_C5V = 0;

	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);


	TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High True Pulses

	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High True Pulses

	TPM0_C4SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C4SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High True Pulses

	TPM0_C5SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High True Pulses
}

/*
// Delay function used for longer delays
void longDelay(volatile uint32_t delay) {
	uint32_t i;
	for (i = 0; i < delay * 1000; i++) {
		__NOP(); // No operation (just a placeholder for a small delay)
	}
}
*/

/*
// Use for shorter delays
void shortDelay(volatile uint32_t nof)
{
	while (nof != 0)
	{
		__asm("NOP");
		nof--;
	}
}
*/

void setWheels(wheelsDirection dir, int speed)
{
	switch (dir) {
	case RF:
		TPM0_C4V = speed;
		TPM0_C5V = 0;
		break;
	case RR:
		TPM0_C4V = 0;
		TPM0_C5V = speed;
		break;
	case LF:
		TPM0_C2V = speed;
		TPM0_C3V = 0;
		break;
	case LR:
		TPM0_C2V = 0;
		TPM0_C3V = speed;
		break;
	}
}

void forward(int speed)
{
	setWheels(LF, speed);
	setWheels(RF, speed);
}

void reverse(int speed)
{
	setWheels(LR, speed);
	setWheels(RR, speed);
}

void spinLeft(int speed)
{
	setWheels(LR, speed);
	setWheels(RF, speed);
}

void spinRight(int speed)
{
	setWheels(LF, speed);
	setWheels(RR, speed);
}

void curveLeft(int speed, double diff)
{
	setWheels(LF, diff * speed);
	setWheels(RF, speed);
}

void curveRight(int speed, double diff)
{
	setWheels(LF, speed);
	setWheels(RF, diff * speed);
}

void stop(void)
{
	setWheels(LF, 0);
	setWheels(RF, 0);
}

void move(uint8_t command, int speed) {
  switch (command) {
    case 0x00:
      stop();
      break;
    case 0x01:
      forward(speed);
      break;
    case 0x02:
      reverse(speed);
      break;
    case 0x03:
      curveLeft(speed,0.6);
      break;
    case 0x04:
      curveRight(speed,0.6);
      break;
    case 0x05:
      curveLeft(speed,0.4);
      break;
    case 0x06:
      curveRight(speed,0.4);
      break;
    case 0x07:
      curveLeft(speed,0.2);
      break;
    case 0x08:
      curveRight(speed,0.2);
      break;
    case 0x09:
      curveLeft(speed,0.2);
      break;
    case 0x0A:
      curveRight(speed,0.2);
      break;
    case 0x0B:
      spinLeft(speed);
      break;
    case 0x0C:
      spinRight(speed);
      break;
    default:
      stop();
      break;
  }
}
