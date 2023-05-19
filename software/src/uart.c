// ===================================================================================
// UART Functions for CH551, CH552 and CH554
// ===================================================================================

#include "uart.h"

__xdata uint8_t  UART_buffer[64];             // UART RX ring buffer
volatile uint8_t UART_readPointer  = 0;       // UART RX buffer read pointer
volatile uint8_t UART_writePointer = 0;       // UART RX buffer write pointer
volatile __bit   UART_readyFlag    = 1;       // UART ready to write flag

// ===================================================================================
// UART Interrupt Service Routine
// ===================================================================================
#pragma save
#pragma nooverlay
void UART_interrupt(void) {
  if(RI) {                                    // RX complete?
    UART_buffer[UART_writePointer++] = SBUF;  // push received byte to buffer...
    UART_writePointer &= 63;                  // increase ring buffer pointer
    RI = 0;                                   // clear RX interrupt flag
  }
  if(TI) {                                    // TX complete?
    UART_readyFlag = 1;                       // set ready to write flag
    TI = 0;                                   // clear TX interrupt flag
  }
}
#pragma restore
