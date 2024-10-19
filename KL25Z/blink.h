#include "MKL25Z4.h"                    // Device header

#define RED_LED    18 // PortB Pin 18
#define GREEN_LED  19 // PortB Pin 19
#define BLUE_LED   1  // PortD Pin 1
#define MASK(x)    (1 << (x))

typedef enum 
{
	RED,
	GREEN,
	BLUE,
	OFF
} color_t;


void initGPIO(void);

void led_control(color_t color);
