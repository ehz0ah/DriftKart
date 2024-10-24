#include "buzzer.h" // Include the header file
#include "pwm.h"

// Melody A (e.g., Twinkle Twinkle Little Star)
uint32_t melodyA[] = {440, 440, 523, 523, 587, 587, 523};  // Frequencies
uint32_t durationsA[] = {500, 500, 500, 500, 500, 500, 1000}; // Durations in ms

void initBuzzer(void) {
    // Enable clock for Port C and TPM modules
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;   // Enable clock for Port C
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;    // Enable clock for TPM0

    // Set PTC2 (TPM0_CH1) to alternate function (MUX = 4 for TPM)
    PORTC->PCR[2] = PORT_PCR_MUX(4);

    // Set TPM clock source to MCGFLLCLK (or another clock source)
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // Set clock source for TPM

    // Set TPM0 to up-counting mode and configure prescaler
    TPM0->SC = TPM_SC_PS(7); // Prescaler of 128

    // Enable TPM0 and set it to PWM mode with edge-aligned PWM
    TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Edge-aligned PWM
}

void setPWMTone(uint32_t frequency) {
    // Calculate MOD value for the desired frequency
    uint32_t modValue = (CLOCK / (128 * frequency)) - 1;
    TPM0->MOD = modValue;

    // Set duty cycle to 100% (continuous signal for maximum volume)
    TPM0->CONTROLS[1].CnV = modValue;

    // Start TPM0 if not already started
    TPM0->SC |= TPM_SC_CMOD(1); // Start TPM with external clock
}

void stopBuzzer(void) {
    // Disable the TPM0 by setting CMOD to 0
    TPM0->SC &= ~TPM_SC_CMOD_MASK;
}

// Delay function using osDelay
void delay(uint32_t delayTime) {
    osDelay(delayTime);  // Use osDelay for RTOS context
}

void playMelody(void) {
    uint32_t numNotes = sizeof(melodyA) / sizeof(melodyA[0]); // Number of notes in the melody
		for (uint32_t i = 0; i < numNotes; i++) {
			osEventFlagsWait(runEndEvent,0x01,osFlagsNoClear, osWaitForever);
			setPWMTone(melodyA[i]);             // Set frequency
			osDelay(durationsA[i]);             // Delay for the note's duration
			stopBuzzer();                          // Stop the tone
			osDelay(100);                       // Short gap between notes
		}
}

void playEndMusic(void) {
		osEventFlagsWait(runEndEvent, 0x02, osFlagsNoClear, osWaitForever);
		setPWMTone(523); // Set frequency for continuous beep
}
