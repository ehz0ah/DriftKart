#include "MKL25Z4.h" // Device header
#include "pwm.h"

// Initialize PWM for both motor drivers
void initPWM(void)
{
	SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK);  // removed SIM_SCGC5_PORTA_MASK

	// Enable Clock Gating for Timer 0 and Timer 1
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	//SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;  // No need for timer 1 anymore

	// Front Left Motor Pins
	//PORTD->PCR[Front_Left_Pin_1] &= ~PORT_PCR_MUX_MASK;
	//PORTD->PCR[Front_Left_Pin_1] |= PORT_PCR_MUX(4);

	//PORTD->PCR[Front_Left_Pin_2] &= ~PORT_PCR_MUX_MASK;
	//PORTD->PCR[Front_Left_Pin_2] |= PORT_PCR_MUX(4);

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

	// Back Right Motor Pins
	//PORTA->PCR[Back_Right_Pin_1] &= ~PORT_PCR_MUX_MASK;
	//PORTA->PCR[Back_Right_Pin_1] |= PORT_PCR_MUX(3);

	//PORTA->PCR[Back_Right_Pin_2] &= ~PORT_PCR_MUX_MASK;
	//PORTA->PCR[Back_Right_Pin_2] |= PORT_PCR_MUX(3);

	// Set Modulo Value
	TPM0->MOD = PWM_PERIOD;
	//TPM1->MOD = PWM_PERIOD;

	// Set compare values (initially 0)
	//TPM1_C0V = 0;
	//TPM1_C1V = 0;

	//TPM0_C0V = 0;
	//TPM0_C1V = 0;

	TPM0_C2V = 0;
	TPM0_C3V = 0;

	TPM0_C4V = 0;
	TPM0_C5V = 0;

	// Edge-Aligned PWM configuration
	//TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	//TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	//TPM1->SC &= ~(TPM_SC_CPWMS_MASK);

	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);

	// Enable PWM on each channel
	//TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High True Pulses

	//TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High True Pulses

	//TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High True Pulses

	//TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	//TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High True Pulses

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

// Use for shorter delays
void shortDelay(volatile uint32_t nof)
{
	while (nof != 0)
	{
		__asm("NOP");
		nof--;
	}
}


void setWheels(wheelsDirection dir, int speed)
{
	switch (dir)
	{
	/*
	case FLF:
		TPM0_C0V = speed;
		TPM0_C1V = 0;
		break;
	case FLR:
		TPM0_C0V = 0;
		TPM0_C1V = speed;
		break;
	*/
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
	/*
	case BRF:
		//TPM1_C0V = speed;
		//TPM1_C1V = 0;
		break;
	case BRR:
		TPM1_C0V = 0;
		TPM1_C1V = speed;
		break;
	*/
	}
}

void forward(int speed) {
	setWheels(LF, speed);
	setWheels(RF, speed);
}

void reverse(int speed) {
	setWheels(LR, speed);
	setWheels(RR, speed);
}

void spinLeft(int speed) {
	setWheels(LR, speed);
	setWheels(RF, speed);
}

void spinRight(int speed) {
	setWheels(LF, speed);
	setWheels(RR, speed);
}

void curveLeft(int speed, double diff) {
	setWheels(LF, diff * speed);
	setWheels(RF, speed);
}

void curveRight(int speed, double diff) {
	setWheels(LF, speed);
	setWheels(RF, diff * speed);
}

void stop(void) {
	setWheels(LF, 0);
	setWheels(RF, 0);
}

void move(uint8_t command, uint8_t spin, int speed) {
	switch (command) {  // The 16 commands (0-15) reverse left and right not yet implemented.
		case 0:
			stop();
			break;
		case 1:
			forward(speed);
			break;
		case 2:
			reverse(speed);
			break;
		case 3:
			curveLeft(speed, 0.9);  // curve left 1 (weakest left)
			break;
		case 4:
			curveLeft(speed, 0.8);  // curve left 2
			break;
		case 5:
			curveLeft(speed, 0.7);  // curve left 3
			break;
		case 6:
			if (spin) {
				spinLeft(speed);  // SPIN LEFT ON THE SPOT
			} else {
				curveLeft(speed, 0);  // curve left 4  (Hardest left, NOT SPIN ON THE SPOT)
			}
			break;
		case 7:
			curveRight(speed, 0.9);  // curve right 1
			break;
		case 8:
			curveRight(speed, 0.8);  // curve right 2
			break;
		case 9:
			curveRight(speed, 0.7);  // curve right 3
			break;
		case 10:
			if (spin) {
				spinRight(speed);  // SPIN RIGHT ON THE SPOT
			} else {
				curveRight(speed, 0);  // curve right 4  (Hardest right, NOT SPIN ON THE SPOT)
			}
			break;
	}
}
