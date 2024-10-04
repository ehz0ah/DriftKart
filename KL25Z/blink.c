#include "MKL25Z4.h"                    // Device header
#include "blink.h"

void initGPIO(void)
{
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	// Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;  // We are accessing the pin control register for the RED_LED and clear MUX bits
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);    // sets MUX (multiplexer) to be 1 for GPIO
	
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED)); // Data Direction Registers for Port B
	PTD->PDDR |= MASK(BLUE_LED);                    // Sets 1 to configure it to output
}

void led_control(color_t color)
{
	// Turn off all LEDs first
	PTB->PCOR = MASK(RED_LED) | MASK(GREEN_LED); // Port clear Output Register for Port B, so we set high to these bits
	PTD->PCOR = MASK(BLUE_LED);   
	
	// Turn on the selected LED
	switch (color)
	{
		case RED:
				PTB->PSOR = MASK(GREEN_LED);  // Port Set Output Register, also write high since its set register
				PTD->PSOR = MASK(BLUE_LED);
				break;
		case GREEN:
				PTB->PSOR = MASK(RED_LED);
				PTD->PSOR = MASK(BLUE_LED);
				break;
		case BLUE:
				PTB->PSOR = MASK(RED_LED);
				PTB->PSOR = MASK(GREEN_LED);
				break;
	}
}
