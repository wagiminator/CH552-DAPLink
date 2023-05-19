// ===================================================================================
// USB HID Functions for CH551, CH552 and CH554
// ===================================================================================

#include "ch554.h"
#include "usb.h"
#include "usb_hid.h"
#include "usb_descr.h"
#include "usb_handler.h"

// ===================================================================================
// Variables and Defines
// ===================================================================================

volatile __xdata uint8_t HID_byteCount = 0; // received bytes in endpoint

// ===================================================================================
// Front End Functions
// ===================================================================================

// Setup USB HID
void HID_init(void) {
  USB_init();
  UEP1_T_LEN  = 0;
}

// ===================================================================================
// HID-Specific USB Handler Functions
// ===================================================================================

// Setup HID endpoints
void HID_setup(void) {
  UEP1_DMA    = (uint16_t)EP1_buffer;       // EP1 data transfer address
  UEP1_CTRL   = bUEP_AUTO_TOG               // EP1 Auto flip sync flag
              | UEP_T_RES_NAK               // EP1 IN transaction returns NAK
              | UEP_R_RES_ACK;              // EP1 OUT transaction returns ACK
  UEP4_1_MOD  = bUEP1_TX_EN                 // EP1 TX enable
              | bUEP1_RX_EN;                // EP1 RX_enable
}

// Reset HID parameters
void HID_reset(void) {
  UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
  HID_byteCount = 0;
}

// Endpoint 1 IN handler (HID report transfer to host)
void HID_EP1_IN(void) {
  UEP1_T_LEN = 0;                                           // no data to send anymore
  UEP1_CTRL = UEP1_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_NAK;  // default NAK
}

// Endpoint 1 OUT handler (HID report transfer from host)
void HID_EP1_OUT(void) {
  if(U_TOG_OK) {                            // discard unsynchronized packets
    HID_byteCount = USB_RX_LEN;
    if(HID_byteCount)
      // Respond NAK after a packet. Let main code change response after handling.
      UEP1_CTRL = UEP1_CTRL & ~MASK_UEP_R_RES | UEP_R_RES_NAK;
  }
}
