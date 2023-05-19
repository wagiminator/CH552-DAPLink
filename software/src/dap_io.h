// ===================================================================================
// CMSIS-DAP CONFIG I/O for CH552 DAPLink
// ===================================================================================

#pragma once
#include "ch554.h"
#include "gpio.h"
#include "config.h"


// ===================================================================================
// SWD PORT CONFIG I/O
// ===================================================================================

// SWD I/O pin manipulations
#define SWK_SET(val)          PIN_write(PIN_SWK, val)
#define SWD_SET(val)          PIN_write(PIN_SWD, val)
#define RST_SET(val)          PIN_write(PIN_RST, val)
#define SWK_GET()             PIN_read(PIN_SWK)
#define SWD_GET()             PIN_read(PIN_SWD)
#define RST_GET()             PIN_read(PIN_RST)

#define PIN_DELAY()

// Connect SWD port (setup port for data transmission)
#define PORT_SWD_CONNECT() {  \
  LED_PRT_SET(1);             \
}

// SWCLK clockout cycle
#define SWD_CLOCK_CYCLE() {   \
  SWK_SET(0);                 \
  PIN_DELAY();                \
  SWK_SET(1);                 \
}

// Write one bit via SWD
#define SWD_WRITE_BIT(bits) { \
  SWD_SET((bits)&1);          \
  SWD_CLOCK_CYCLE();          \
}

// Read one bit via SWD
#define SWD_READ_BIT(bit) {   \
  SWK_SET(0);                 \
  PIN_DELAY();                \
  bit = SWD_GET();            \
  SWK_SET(1);                 \
}

// Set SWDIO pin to output
#define SWD_OUT_ENABLE()      // nothing to be done (open-drain pullup)

// Set SWDIO pin to input
#define SWD_OUT_DISABLE()     SWD_SET(1)


// ===================================================================================
// JTAG PORT CONFIG I/O
// ===================================================================================

// JTAG I/O pin manipulations
#define TCK_SET(val)          PIN_write(PIN_SWK, val)
#define TMS_SET(val)          PIN_write(PIN_SWD, val)
#define TDO_SET(val)          PIN_write(PIN_TDO, val)
#define TDI_SET(val)          PIN_write(PIN_TDI, val)
#define TRST_SET(val)         PIN_write(PIN_TRST, val)
#define TCK_GET()             PIN_read(PIN_SWK)
#define TMS_GET()             PIN_read(PIN_SWD)
#define TDO_GET()             PIN_read(PIN_TDO)
#define TDI_GET()             PIN_read(PIN_TDI)
#define TRST_GET()            PIN_read(PIN_TRST)

// Connect JTAG port (setup port for data transmission)
#define PORT_JTAG_CONNECT() { \
  LED_PRT_SET(1);             \
}

// JTAG TCK cycle
#define JTAG_CYCLE_TCK() {    \
  TCK_SET(0);                 \
  PIN_DELAY();                \
  TCK_SET(1);                 \
}

// JTAG TDI cycle
#define JTAG_CYCLE_TDI(tdi) { \
  TDI_SET(tdi);               \
  JTAG_CYCLE_TCK();           \
}

// JTAG TDO cycle
#define JTAG_CYCLE_TDO(tdo) { \
  TCK_SET(0);                 \
  PIN_DELAY();                \
  tdo = TDO_GET();            \
  TCK_SET(1);                 \
}

// JTAG TDIO cycle
#define JTAG_CYCLE_TDIO(tdi,tdo) { \
  TDI_SET(tdi);               \
  TCK_SET(0);                 \
  PIN_DELAY();                \
  tdo = TDO_GET();            \
  TCK_SET(1);                 \
}


// ===================================================================================
// GENERAL PORT CONFIG I/O
// ===================================================================================

// Initial port setup (at firmware start)
#define PORT_SETUP()          // nothing to be done (reset values already fit)

// Disconnect complete port (idle state, input/pullup)
#define PORT_OFF() {          \
  SWK_SET(1);                 \
  SWD_SET(1);                 \
  TDI_SET(1);                 \
  TDO_SET(1);                 \
  RST_SET(1);                 \
  TRST_SET(1);                \
  LED_PRT_SET(0);             \
}

// LED I/O pin manipulations
#define LED_PRT_SET(val)      PIN_write(PIN_LED, !(val))  // LED is active low
#define LED_CON_SET(val)
#define LED_RUN_SET(val)


// ===================================================================================
// GENERAL CONFIG
// ===================================================================================

// HID transfer buffers
#define DAP_READ_BUF_PTR      EP1_buffer
#define DAP_WRITE_BUF_PTR     EP1_buffer + 64
extern __xdata uint8_t EP1_buffer[];

// DAP init function
#define DAP_init()            HID_init(); PORT_SETUP();
extern void HID_init(void);
