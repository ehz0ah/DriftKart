#include "lib.h"

// Definitions
#define TONE_PIN (1 << 2) // Port C Pin 2 (TPM0_CH1)
#define CLOCK 48000000    // KL25Z core clock frequency in Hz

// Event flag for melody control
extern osEventFlagsId_t runEndEvent; // External reference to the event flag

// Melody A (e.g., Twinkle Twinkle Little Star)
extern uint32_t melodyA[];    // Frequencies for Melody A
extern uint32_t durationsA[]; // Durations for Melody A

// Jingle
extern uint32_t melodyB[];    // Frequencies for Melody B
extern uint32_t durationsB[]; // Durations for Melody B

extern uint32_t melodyC[];    // Frequencies for Melody C
extern uint32_t durationsC[]; // Durations for Melody C


void initBuzzer(void);

void setPWMTone(uint32_t frequency);

void stopBuzzer(void);

void playMelodyA(void);

void playMelodyB(void);

void playMelodyC(void);

