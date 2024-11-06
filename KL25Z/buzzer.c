#include "buzzer.h" // Include the header file

#define  a3f    208     // 208 Hz
#define  b3f    233     // 233 Hz
#define  b3     247     // 247 Hz
#define  c4     261     // 261 Hz MIDDLE C
#define  c4s    277     // 277 Hz
#define  e4f    311     // 311 Hz    
#define  f4     349     // 349 Hz 
#define  a4f    415     // 415 Hz  
#define  b4f    466     // 466 Hz 
#define  b4     493     //  493 Hz 
#define  c5     523     // 523 Hz 
#define  c5s    554     // 554  Hz
#define  e5f    622     // 622 Hz  
#define  f5     698     // 698 Hz 
#define  f5s    740     // 740 Hz
#define  a5f    831     // 831 Hz
#define  rest    0



// Melody B (Twinkle, Twinkle, Little Star)
uint32_t melodyA[] = {
    523, 523, 784, 784, 880, 880, 784, // Twinkle, twinkle, little star
    739, 739, 698, 698, 659, 659, 523, // How I wonder what you are
    523, 523, 784, 784, 880, 880, 784, // Up above the world so high
    739, 739, 698, 698, 659, 659, 523  // Like a diamond in the sky
};

// Durations for each note (in milliseconds)
uint32_t durationsA[] = {
    250, 250, 500, 500, 500, 500, 1000, // Twinkle, twinkle, little star
    250, 250, 500, 500, 500, 500, 1000, // How I wonder what you are
    250, 250, 500, 500, 500, 500, 1000, // Up above the world so high
    250, 250, 500, 500, 500, 500, 1000  // Like a diamond in the sky
};



// Melody B (Jingle Bells - main theme)
uint32_t melodyB[] = {
    659, 659, 659, // Jingle bells
    659, 659, 659, // Jingle bells
    659, 784, 523, 587, 659, // Jingle all the way
};

// Durations for each note (in milliseconds)
uint32_t durationsB[] = {
    300, 300, 600, // Jingle bells
    300, 300, 600, // Jingle bells
    300, 300, 300, 300, 800, // Jingle all the way
};

uint32_t melodyC[]  = { 
	b4f, b4f, a4f, a4f,
  f5, f5, e5f, b4f, b4f, a4f, a4f, e5f, e5f, c5s, c5, b4f,
  c5s, c5s, c5s, c5s,
  c5s, e5f, c5, b4f, a4f, a4f, a4f, e5f, c5s,
  b4f, b4f, a4f, a4f,
  f5,  f5, e5f, b4f, b4f, a4f, a4f, a5f, c5, c5s, c5, b4f,
  c5s, c5s, c5s, c5s,
  c5s, e5f, c5, b4f, a4f, rest, a4f, e5f, c5s, rest
};

uint32_t durationsC[]  = { 
	1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
 1, 1, 1, 1,
 3, 3, 3, 1, 2, 2, 2, 4, 8,
  1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3,  1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8, 4
};


void initBuzzer(void) {
    // Enable clock for Port A and TPM2 modules
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;   // Enable clock for Port A
    SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;    // Enable clock for TPM2

    // Set PTA2 to TPM2_CH1 by selecting the appropriate MUX setting (MUX = 3)
    PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK;      // Clear MUX field for PTA2
    PORTA->PCR[2] |= PORT_PCR_MUX(3);         // Set MUX to 3 for TPM2_CH1 function

    // Set TPM clock source to MCGFLLCLK or an alternative clock source
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);        // Set TPM clock source to 48 MHz MCGFLLCLK

    // Set TPM2 to up-counting mode and configure prescaler
    TPM2->SC = TPM_SC_PS(7);                  // Set prescaler to 128

    // Set TPM2 to edge-aligned PWM mode with high-true pulses
    TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK));
    TPM2_C1SC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK);  // Edge-aligned PWM with high-true pulses
}


void setPWMTone(uint32_t frequency) {
    if (frequency == 0) {
        TPM2->SC &= ~TPM_SC_CMOD_MASK; // Stop TPM2 if frequency is zero
        return;
    }

    // Calculate MOD value for the desired frequency
    uint32_t modValue = 375000/frequency;

    // Set MOD value to control the frequency
    TPM2->MOD = modValue;

    // Set duty cycle to 50% (adjustable as needed)
    TPM2_C1V = modValue * 0.5;

    // Start TPM2 if not already started
    TPM2->SC |= TPM_SC_CMOD(1); // Start TPM2 with the selected clock
}

void stopBuzzer(void) {
    TPM2->SC &= ~TPM_SC_CMOD_MASK; // Disable TPM2
}


void playMelodyA(void) {
    uint32_t numNotes = sizeof(melodyA) / sizeof(melodyA[0]); // Number of notes in the melody
		for (uint32_t i = 0; i < numNotes; i++) {
			setPWMTone(melodyA[i]);             // Set frequency
			osDelay(durationsA[i]);             // Delay for the note's duration
			stopBuzzer();                          // Stop the tone
			osDelay(100);                       // Short gap between notes
		}
}

void playMelodyB(void) {
		uint32_t numNotes = sizeof(melodyB) / sizeof(melodyB[0]); // Number of notes in the melody
		for (uint32_t i = 0; i < numNotes; i++) {
			osEventFlagsWait(runEndEvent,0x02,osFlagsNoClear,osWaitForever);
			setPWMTone(melodyB[i]);             // Set frequency
			osDelay(durationsB[i]);             // Delay for the note's duration
			stopBuzzer();                          // Stop the tone
			osDelay(100);                       // Short gap between notes
		}
}

void playMelodyC(void) {
		uint32_t numNotes = sizeof(melodyC) / sizeof(melodyC[0]); // Number of notes in the melody
		for (uint32_t i = 0; i < numNotes; i++) {
			osEventFlagsWait(runEndEvent,0x01,osFlagsNoClear,osWaitForever);
			setPWMTone(melodyC[i]);             // Set frequency
			osDelay(durationsC[i] * 100);             // Delay for the note's duration
			stopBuzzer();                          // Stop the tone
			osDelay(150);                       // Short gap between notes
		}
}
