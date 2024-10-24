#include <MKL25Z4.h>   // Include the KL25Z header file
#include "cmsis_os2.h" // Include CMSIS RTOS header

// Definitions
#define TONE_PIN (1 << 2) // Port C Pin 2 (TPM0_CH1)
#define CLOCK 48000000    // KL25Z core clock frequency in Hz

// Event flag for melody control
extern osEventFlagsId_t runEndEvent; // External reference to the event flag

// Melody A (e.g., Twinkle Twinkle Little Star)
extern uint32_t melodyA[];    // Frequencies for Melody A
extern uint32_t durationsA[]; // Durations for Melody A

// Function prototypes
void initBuzzer(void);
void setPWMTone(uint32_t frequency);
void stopBuzzer(void);
void delay(uint32_t delayTime);

// Music 
void playMelody(void);
void playEndMusic(void);
