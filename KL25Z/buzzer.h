#include "lib.h"

// Definitions
#define TONE_PIN (1 << 2) // Port C Pin 2 (TPM0_CH1)
#define CLOCK 48000000    // KL25Z core clock frequency in Hz

#define NOTE_C4    261
#define NOTE_D4    294
#define NOTE_E4    329
#define NOTE_F4    349
#define NOTE_G4    392
#define NOTE_A4    440
#define NOTE_B4    466
#define NOTE_C5    523
#define NOTE_D5    587
#define NOTE_E5    659
#define NOTE_F5    698
#define NOTE_G5    784
#define NOTE_C6    1046
#define NOTE_D6    1175
#define NOTE_E6    1319
#define NOTE_F6    1397
#define NOTE_G6    1568
#define NOTE_A6    1760
#define NOTE_B6    1975

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

extern uint32_t melodyD[];    // Frequencies for Melody D
extern uint32_t durationsD[]; // Durations for Melody D


void initBuzzer(void);

void setPWMTone(uint32_t frequency);

void stopBuzzer(void);

void playMelodyA(void);

void playMelodyB(void);

void playMelodyC(void);

void playMelodyD(void);
