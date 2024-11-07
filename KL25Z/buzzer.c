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



// Melody A (Twinkle Twinkle Little Star)
uint32_t melodyA[] = {
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, // Twinkle, twinkle, little star
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4, // How I wonder what you are
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, // Up above the world so high
    NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, // Like a diamond in the sky
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, // Twinkle, twinkle, little star
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4  // How I wonder what you are
};

// Durations for each note (in milliseconds)
uint32_t durationsA[] = {
    500, 500, 500, 500, 500, 500, 1000, // Twinkle, twinkle, little star
    500, 500, 500, 500, 500, 500, 1000, // How I wonder what you are
    500, 500, 500, 500, 500, 500, 1000, // Up above the world so high
    500, 500, 500, 500, 500, 500, 1000, // Like a diamond in the sky
    500, 500, 500, 500, 500, 500, 1000, // Twinkle, twinkle, little star
    500, 500, 500, 500, 500, 500, 1000  // How I wonder what you are
};



// Melody B (Jingle Bells - main theme)
uint32_t melodyB[] = {
    NOTE_E4, NOTE_E4, NOTE_E4, // Jingle bells
    NOTE_E4, NOTE_E4, NOTE_E4, // Jingle bells
    NOTE_E4, NOTE_G4, NOTE_D4, NOTE_E4, NOTE_E4, // Jingle all the way
    NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
    NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_G4
};

// Durations for each note (in milliseconds)
uint32_t durationsB[] = {
    500, 500, 1000,
		500, 500, 1000,
		500, 500, 500, 500, 2000, // First segment
    500, 500, 500, 500, 500, 500, 500,
		1000, 500, 500, 500, 500, 1000, 1000
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

// Melody D (Never Gonna Give You Up) simplified and increased by two octaves
uint32_t melodyD[] = {
    NOTE_C6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_E6, NOTE_C6, NOTE_D6, // "We're no strangers to love"
    NOTE_E6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_E6, NOTE_C6, NOTE_D6, NOTE_C6, // "You know the rules and so do I"
    NOTE_C6, NOTE_E6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_E6, NOTE_C6, NOTE_D6, // "A full commitment's what I'm thinking of"
    NOTE_E6, NOTE_E6, NOTE_D6, NOTE_C6, NOTE_E6, NOTE_C6, NOTE_D6, NOTE_C6, // "You wouldn't get this from any other guy"
};

// Durations for each note (in milliseconds)
uint32_t durationsD[] = {
    500, 500, 500, 500, 500, 500, 500, 500, // "We're no strangers to love"
    500, 500, 500, 500, 500, 500, 500, 500, // "You know the rules and so do I"
    500, 500, 500, 500, 500, 500, 500, 500, // "A full commitment's what I'm thinking of"
    500, 500, 500, 500, 500, 500, 500, 500  // "You wouldn't get this from any other guy"
};

void initBuzzer(void) {
    // Enable clock for Port A and TPM2 modules
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;   // Enable clock for Port A
    SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;    // Enable clock for TPM2

    // Set PTA2 to TPM2_CH1 by selecting the appropriate MUX setting (MUX = 3)
    PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK;      // Clear MUX field for PTA2
    PORTA->PCR[2] |= PORT_PCR_MUX(3);         // Set MUX to 3 for TPM2_CH1 function

    // Set TPM clock source to MCGFLLCLK or an alternative clock source
		SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);        // Set TPM clock source to 48 MHz MCGFLLCLK

    // Set TPM2 to up-counting mode and configure prescaler
		TPM2->SC &= ~TPM_SC_PS_MASK;
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
    TPM2_C1V = modValue*0.05;

    // Start TPM2 if not already started
    TPM2->SC |= TPM_SC_CMOD(1); // Start TPM2 with the selected clock
}

void stopBuzzer(void) {
    TPM2->SC &= ~TPM_SC_CMOD_MASK; // Disable TPM2
}


void playMelodyA(void) {
    uint32_t numNotes = sizeof(melodyA) / sizeof(melodyA[0]); // Number of notes in the melody
		for (uint32_t i = 0; i < numNotes; i++) {
			osEventFlagsWait(runEndEvent,0x01,osFlagsNoClear,osWaitForever);
			setPWMTone(melodyA[i]*4);             // Set frequency
			osDelay(durationsA[i]);             // Delay for the note's duration
			stopBuzzer();                          // Stop the tone
			osDelay(100);                       // Short gap between notes
		}
}

void playMelodyB(void) {
		uint32_t numNotes = sizeof(melodyB) / sizeof(melodyB[0]); // Number of notes in the melody
		for (uint32_t i = 0; i < numNotes; i++) {
			osEventFlagsWait(runEndEvent,0x02,osFlagsNoClear,osWaitForever);
			setPWMTone(melodyB[i]*4);             // Set frequency
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

void playMelodyD(void) {
		uint32_t numNotes = sizeof(melodyD) / sizeof(melodyD[0]); // Number of notes in the melody
		for (uint32_t i = 0; i < numNotes; i++) {
			osEventFlagsWait(runEndEvent,0x01,osFlagsNoClear,osWaitForever);
			setPWMTone(melodyD[i]);             // Set frequency
			osDelay(durationsD[i]);             // Delay for the note's duration
			stopBuzzer();                          // Stop the tone
			osDelay(150);                       // Short gap between notes
		}
}

