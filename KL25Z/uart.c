#include "MKL25Z4.h" // Device header
#include "uart.h"
#include "queue.h"

void initUART2(uint32_t baud_rate)
{
  uint32_t divisor, bus_clock;

  // enable clock to UART and Port E
  SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

  // connect UART to pins for PTE22, PTE23
  PORTE->PCR[UART_TX_PORTE22] &= ~PORT_PCR_MUX_MASK;
  PORTE->PCR[UART_TX_PORTE22] |= PORT_PCR_MUX(4);

  PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
  PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);

  // ensure tx and rx are disabled before configuration
  // UART2->C2 &= ~(UARTLP_C2_TE_MASK | UARTLP_C2_RE_MASK);  // LP means UART low power
  UART2->C2 &= ~((UART_C2_TE_MASK | UART_C2_RE_MASK));

  // Set baud rate to 9600 baud
  bus_clock = DEFAULT_SYSTEM_CLOCK / 2;
  divisor = bus_clock / (baud_rate * 16);
  UART2->BDH = UART_BDH_SBR(divisor >> 8);
  UART2->BDL = UART_BDL_SBR(divisor);

  // No parity, 8 bits, one stop bit
  UART2->C1 = UART2->S2 = UART2->C3 = 0;

  // Enable transmitter and receiver
  UART2->C2 |= ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));

  // Set priority for UART2 interrupt
  NVIC_SetPriority(UART2_IRQn, UART2_INT_PRIO);
  NVIC_ClearPendingIRQ(UART2_IRQn);
  NVIC_EnableIRQ(UART2_IRQn);

  // Enable Receive Interrupt
  UART2->C2 |= UART_C2_RIE_MASK; // Enable interrupt on receive data
}

void UART2_Transmit_Poll(uint8_t data)
{
  // wait until transmit data register is empty
  while (!(UART2->S1 & UART_S1_TDRE_MASK))
    ;
  UART2->D = data;
}

uint8_t UART2_Receive_Poll(void)
{
  // wait until receive data register is full
  while (!(UART2->S1 & UART_S1_RDRF_MASK))
    ;
  return UART2->D;
}
