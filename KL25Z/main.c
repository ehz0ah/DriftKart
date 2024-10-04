#include "MKL25Z4.h"                    // Device header
#include "blink.h"
#include "queue.h"

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO  128

CircularBuffer rx_queue;  // Global buffer instance

void init_UART2(uint32_t baud_rate) {
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
 
 
 // Set baud rate to 4800 baud
 bus_clock = DEFAULT_SYSTEM_CLOCK/2;
 divisor = bus_clock/(baud_rate*16);
 UART2->BDH = UART_BDH_SBR(divisor>>8);
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
  UART2->C2 |= UART_C2_RIE_MASK;  // Enable interrupt on receive data
}

void UART2_IRQHandler(void) 
{
 if (UART2->S1 & UART_S1_RDRF_MASK) // Check if receive data register is full
 {
  uint8_t data = UART2->D;           // Read the data
  buffer_put(&rx_queue, data);       // Insert into circular buffer
 }
}


void UART2_Transmit_Poll(uint8_t data) {
 // wait until transmit data register is empty
 while (!(UART2->S1 & UART_S1_TDRE_MASK));
 UART2->D = data;
}

uint8_t UART2_Receive_Poll(void) {
 // wait until receive data register is full
 while (!(UART2->S1 & UART_S1_RDRF_MASK));
 return UART2->D;
}

static void delay(volatile uint32_t nof) 
{
 while(nof!=0) 
 {
  __asm("NOP");
  nof--;
 }
}

int main(void)
{
 SystemCoreClockUpdate();
 initGPIO();         // Initialize GPIO Ports for LED control
 init_UART2(BAUD_RATE); // Initialize UART2 for communication
 initQueue(&rx_queue);   // Initialize circular buffer
 
 while (1) 
 {
  if (!isEmpty(&rx_queue))       // Check if buffer contains data
  { 
   uint8_t data = buffer_get(&rx_queue);  // Get data from buffer
   switch (data) {
    case 'r':
     led_control(RED);
     break;
    case 'g':
     led_control(GREEN);
     break;
    case 'b':
     led_control(BLUE);
     break;
    default:
     // Ignore unknown data
     break;
   }
  }
 }
}
