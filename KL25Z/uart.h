#include "MKL25Z4.h"                    // Device header

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO  128

void initUART2(uint32_t baud_rate);

void UART2_Transmit_Poll(uint8_t data);

uint8_t UART2_Receive_Poll(void);
