// ===================================================================================
// Project:   DAPLink - CMSIS-DAP compliant debugging probe with VCP based on CH552
// Version:   v1.0
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// The CH552-based DAPLink is a CMSIS-DAP compliant debugging probe with SWD and JTAG
// protocol support. It can be used to program Microchip SAM and other ARM-based
// microcontrollers. The additional Virtual COM Port (VCP) provides an additional
// debugging feature. The SWD-part of the firmware is based on Ralph Doncaster's 
// DAPLink-implementation for CH55x microcontrollers and Deqing Sun's CH55xduino port.
//
// References:
// -----------
// - Blinkinlabs: https://github.com/Blinkinlabs/ch554_sdcc
// - Deqing Sun: https://github.com/DeqingSun/ch55xduino
// - Ralph Doncaster: https://github.com/nerdralph/ch554_sdcc
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
// - ARMmbed DAPLink: https://github.com/ARMmbed/DAPLink
// - picoDAP: https://github.com/wagiminator/CH552-picoDAP
//
// Compilation Instructions:
// -------------------------
// - Chip:  CH552
// - Clock: 16 MHz internal
// - Adjust the firmware parameters in src/config.h if necessary.
// - Make sure SDCC toolchain and Python3 with PyUSB is installed.
// - Press BOOT button on the board and keep it pressed while connecting it via USB
//   with your PC.
// - Run 'make flash' immediatly afterwards.
//
// Operating Instructions:
// -----------------------
// Connect the DAPLink to the target board via the pin header. You can supply power
// via the 3V3 pin or the 5V pin (max 400 mA). Plug the DAPLink into a USB port on 
// your PC. Since it is recognized as a Human Interface Device (HID), no driver 
// installation is required. However, Windows users may need to install a CDC driver
// for the Virtual COM Port (VCP). The DAPLink should work with any debugging software
// that supports CMSIS-DAP (e.g. OpenOCD or PyOCD). The virtual COM port (8N1 only)
// can be used with any serial monitor.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================

// Libraries
#include "src/system.h"                     // system functions
#include "src/delay.h"                      // delay functions
#include "src/dap.h"                        // CMSIS-DAP functions
#include "src/usb_cdc.h"                    // USB CDC functions
#include "src/uart.h"                       // UART functions

// Prototypes for used interrupts
void USB_interrupt(void);
void USB_ISR(void) __interrupt(INT_NO_USB) {
  USB_interrupt();
}

void UART_interrupt(void);
void UART_ISR(void) __interrupt(INT_NO_UART0) {
  UART_interrupt();
}

// Number of received bytes in endpoint
extern volatile __xdata uint8_t HID_byteCount;

// ===================================================================================
// Main Function
// ===================================================================================
void main(void) {
  // Setup
  CLK_config();                             // configure system clock
  DLY_ms(10);                               // wait for clock to settle
  UART_init();                              // init UART
  DAP_init();                               // init CMSIS-DAP
  CDC_init();                               // init virtual COM

  // Loop
  while(1) {
    // Handle DAP
    if(HID_byteCount && !UEP1_T_LEN) {      // DAP packet received and out buffer empty?                      
      DAP_Thread();                         // handle DAP packet
      HID_byteCount = 0;                    // clear byte counter
      UEP1_T_LEN = 64;                      // Windows hangs if smaller
      UEP1_CTRL = UEP1_CTRL & ~(MASK_UEP_R_RES | MASK_UEP_T_RES); // send/receive package
    }

    // Handle virtual COM
    if(CDC_available() && UART_ready()) UART_write(CDC_read());
    if(UART_available() && CDC_getDTR()) {
      while(UART_available()) CDC_write(UART_read());
      CDC_flush();
    }
  }
}
