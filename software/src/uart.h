// ===================================================================================
// UART Functions with Receive Buffer and Interrupt for CH551, CH552 and CH554
// ===================================================================================

#pragma once
#include <stdint.h>
#include "ch554.h"

// UART Macros
#define UART_ready()      (UART_readyFlag)    // ready to send data?
#define UART_available()  (UART_readPointer != UART_writePointer) // something in buffer?

// Define initial BAUD rate
#define UART_BAUD         115200
#define UART_BAUD_SET     (uint8_t)(256 - (((F_CPU / 8 / UART_BAUD) + 1) / 2))

// Variables
extern __xdata uint8_t  UART_buffer[];
extern volatile uint8_t UART_readPointer;
extern volatile uint8_t UART_writePointer;
extern volatile __bit   UART_readyFlag;

// Setup UART
inline void UART_init(void) {
//SM0    = 0;                     // UART0 8 data bits
  SM1    = 1;                     // UART0 BAUD rate by timer
//SM2    = 0;                     // UART0 no multi-device comm
//RCLK   = 0;                     // UART0 receive clock:  TIMER1
//TCLK   = 0;                     // UART0 transmit clock: TIMER1
  PCON  |= SMOD;                  // UART0 fast BAUD rate
  TMOD  |= bT1_M1;                // TIMER1 8-bit auto-reload
  T2MOD |= bTMR_CLK | bT1_CLK;    // TIMER1 fast clock selection
  TH1    = UART_BAUD_SET;         // TIMER1 configure for BAUD rate
  TR1    = 1;                     // TIMER1 start
  REN    = 1;                     // enable RX
  ES     = 1;                     // enable UART0 interrupt
}

// Transmit a data byte, return echo
inline void UART_write(uint8_t data) {
  UART_readyFlag = 0;             // clear ready flag
  SBUF = data;                    // start transmitting data byte
}

// Receive a data byte
inline uint8_t UART_read(void) {
  uint8_t result = UART_buffer[UART_readPointer++];
  UART_readPointer &= 63;
  return result;
}

// Set BAUD rate
inline void UART_setBAUD(uint32_t baud) {
  TH1 = (uint8_t)(256 - ((((F_CPU / 8) / baud) + 1) / 2));
}

void UART_interrupt(void);
