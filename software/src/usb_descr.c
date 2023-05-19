// ===================================================================================
// USB Descriptors
// ===================================================================================

#include "usb_descr.h"

// ===================================================================================
// Device Descriptor
// ===================================================================================
__code USB_DEV_DESCR DevDescr = {
  .bLength            = sizeof(DevDescr),       // size of the descriptor in bytes: 18
  .bDescriptorType    = USB_DESCR_TYP_DEVICE,   // device descriptor: 0x01
  .bcdUSB             = 0x0200,                 // USB specification: USB 2.0
  .bDeviceClass       = USB_DEV_CLASS_MISC,     // miscellaneous class
  .bDeviceSubClass    = 2,                      // unused
  .bDeviceProtocol    = 1,                      // unused
  .bMaxPacketSize0    = EP0_SIZE,               // maximum packet size for Endpoint 0
  .idVendor           = USB_VENDOR_ID,          // VID
  .idProduct          = USB_PRODUCT_ID,         // PID
  .bcdDevice          = USB_DEVICE_VERSION,     // device version
  .iManufacturer      = 1,                      // index of Manufacturer String Descr
  .iProduct           = 2,                      // index of Product String Descriptor
  .iSerialNumber      = 3,                      // index of Serial Number String Descr
  .bNumConfigurations = 1                       // number of possible configurations
};

// ===================================================================================
// Configuration Descriptor
// ===================================================================================
__code USB_CFG_DESCR_HID CfgDescr = {

  // Configuration Descriptor
  .config = {
    .bLength            = sizeof(USB_CFG_DESCR),  // size of the descriptor in bytes
    .bDescriptorType    = USB_DESCR_TYP_CONFIG,   // configuration descriptor: 0x02
    .wTotalLength       = sizeof(CfgDescr),       // total length in bytes
    .bNumInterfaces     = 3,                      // number of interfaces: 3
    .bConfigurationValue= 1,                      // value to select this configuration
    .iConfiguration     = 0,                      // no configuration string descriptor
    .bmAttributes       = 0x80,                   // attributes = bus powered, no wakeup
    .MaxPower           = USB_MAX_POWER_mA / 2    // in 2mA units
  },


  // Interface Association Descriptor
  .association = {
    .bLength            = sizeof(USB_IAD_DESCR),  // size of the descriptor in bytes
    .bDescriptorType    = USB_DESCR_TYP_IAD,      // interf association descr: 0x0B
    .bFirstInterface    = 0,                      // first interface
    .bInterfaceCount    = 2,                      // total number of interfaces
    .bFunctionClass     = USB_DEV_CLASS_COMM,     // function class: CDC (0x02)
    .bFunctionSubClass  = 2,                      // 2: Abstract Control Model (ACM)
    .bFunctionProtocol  = 1,                      // 1: AT command protocol
    .iFunction          = 4                       // index of String Descriptor
  },

  // Interface Descriptor: Interface 0 (CDC)
  .interface0 = {
    .bLength            = sizeof(USB_ITF_DESCR),  // size of the descriptor in bytes: 9
    .bDescriptorType    = USB_DESCR_TYP_INTERF,   // interface descriptor: 0x04
    .bInterfaceNumber   = 0,                      // number of this interface: 0
    .bAlternateSetting  = 0,                      // value used to select alternative setting
    .bNumEndpoints      = 1,                      // number of endpoints used: 1
    .bInterfaceClass    = USB_DEV_CLASS_COMM,     // interface class: CDC (0x02)
    .bInterfaceSubClass = 2,                      // 2: Abstract Control Model (ACM)
    .bInterfaceProtocol = 1,                      // 1: AT command protocol
    .iInterface         = 4                       // index of String Descriptor
  },

  // Functional Descriptors for Interface 0
  .functional = {
    0x05,0x24,0x00,0x10,0x01,                     // header functional descriptor
    0x05,0x24,0x01,0x00,0x01,                     // call management functional descriptor
    0x04,0x24,0x02,0x02,                          // direct line management functional descriptor
    0x05,0x24,0x06,0x00,0x01                      // union functional descriptor: CDC IF0, Data IF1
  },

  // Endpoint Descriptor: Endpoint 3 (CDC Upload, Interrupt)
  .ep3IN = {
    .bLength            = sizeof(USB_ENDP_DESCR), // size of the descriptor in bytes: 7
    .bDescriptorType    = USB_DESCR_TYP_ENDP,     // endpoint descriptor: 0x05
    .bEndpointAddress   = USB_ENDP_ADDR_EP3_IN,   // endpoint: 3, direction: IN (0x83)
    .bmAttributes       = USB_ENDP_TYPE_INTER,    // transfer type: interrupt (0x03)
    .wMaxPacketSize     = EP3_SIZE,               // max packet size
    .bInterval          = 1                       // polling intervall in ms
  },

  // Interface Descriptor: Interface 1 (Data)
  .interface1 = {
    .bLength            = sizeof(USB_ITF_DESCR),  // size of the descriptor in bytes: 9
    .bDescriptorType    = USB_DESCR_TYP_INTERF,   // interface descriptor: 0x04
    .bInterfaceNumber   = 1,                      // number of this interface: 1
    .bAlternateSetting  = 0,                      // value used to select alternative setting
    .bNumEndpoints      = 2,                      // number of endpoints used: 2
    .bInterfaceClass    = USB_DEV_CLASS_DATA,     // interface class: data (0x0a)
    .bInterfaceSubClass = 0,                      // interface sub class
    .bInterfaceProtocol = 0,                      // interface protocol
    .iInterface         = 4                       // index of String Descriptor
  },

  // Endpoint Descriptor: Endpoint 2 (OUT)
  .ep2OUT = {
    .bLength            = sizeof(USB_ENDP_DESCR), // size of the descriptor in bytes: 7
    .bDescriptorType    = USB_DESCR_TYP_ENDP,     // endpoint descriptor: 0x05
    .bEndpointAddress   = USB_ENDP_ADDR_EP2_OUT,  // endpoint: 2, direction: OUT (0x02)
    .bmAttributes       = USB_ENDP_TYPE_BULK,     // transfer type: bulk (0x02)
    .wMaxPacketSize     = EP2_SIZE,               // max packet size
    .bInterval          = 0                       // polling intervall (ignored for bulk)
  },

  // Endpoint Descriptor: Endpoint 2 (IN)
  .ep2IN = {
    .bLength            = sizeof(USB_ENDP_DESCR), // size of the descriptor in bytes: 7
    .bDescriptorType    = USB_DESCR_TYP_ENDP,     // endpoint descriptor: 0x05
    .bEndpointAddress   = USB_ENDP_ADDR_EP2_IN,   // endpoint: 2, direction: IN (0x82)
    .bmAttributes       = USB_ENDP_TYPE_BULK,     // transfer type: bulk (0x02)
    .wMaxPacketSize     = EP2_SIZE,               // max packet size
    .bInterval          = 0                       // polling intervall (ignored for bulk)
  },

  // Interface Descriptor: Interface 2 (HID)
  .interface2 = {
    .bLength            = sizeof(USB_ITF_DESCR),  // size of the descriptor in bytes: 9
    .bDescriptorType    = USB_DESCR_TYP_INTERF,   // interface descriptor: 0x04
    .bInterfaceNumber   = 2,                      // number of this interface: 2
    .bAlternateSetting  = 0,                      // value used to select alternative setting
    .bNumEndpoints      = 2,                      // number of endpoints used: 2
    .bInterfaceClass    = USB_DEV_CLASS_HID,      // interface class: HID (0x03)
    .bInterfaceSubClass = 0,                      // no subclass
    .bInterfaceProtocol = 0,                      // no protocoll
    .iInterface         = 5                       // interface string descriptor
  },

  // HID Descriptor
  .hid0 = {
    .bLength            = sizeof(USB_HID_DESCR),  // size of the descriptor in bytes: 9
    .bDescriptorType    = USB_DESCR_TYP_HID,      // HID descriptor: 0x21
    .bcdHID             = 0x0110,                 // HID class spec version (BCD: 1.1)
    .bCountryCode       = 0,                      // no country code
    .bNumDescriptors    = 1,                      // number of report descriptors: 1
    .bDescriptorTypeX   = USB_DESCR_TYP_REPORT,   // descriptor type: report
    .wDescriptorLength  = sizeof(ReportDescr)     // report descriptor length
  },

  // Endpoint Descriptor: Endpoint 1 (IN, Interrupt)
  .ep1IN = {
    .bLength            = sizeof(USB_ENDP_DESCR), // size of the descriptor in bytes: 7
    .bDescriptorType    = USB_DESCR_TYP_ENDP,     // endpoint descriptor: 0x05
    .bEndpointAddress   = USB_ENDP_ADDR_EP1_IN,   // endpoint: 1, direction: IN (0x81)
    .bmAttributes       = USB_ENDP_TYPE_INTER,    // transfer type: interrupt (0x03)
    .wMaxPacketSize     = EP1_SIZE,               // max packet size
    .bInterval          = 1                       // polling intervall in ms
  },

  // Endpoint Descriptor: Endpoint 1 (OUT, Interrupt)
  .ep1OUT = {
    .bLength            = sizeof(USB_ENDP_DESCR), // size of the descriptor in bytes: 7
    .bDescriptorType    = USB_DESCR_TYP_ENDP,     // endpoint descriptor: 0x05
    .bEndpointAddress   = USB_ENDP_ADDR_EP1_OUT,  // endpoint: 2, direction: OUT (0x01)
    .bmAttributes       = USB_ENDP_TYPE_INTER,    // transfer type: interrupt (0x03)
    .wMaxPacketSize     = EP1_SIZE,               // max packet size
    .bInterval          = 1                       // polling intervall in ms
  }
};

// ===================================================================================
// HID Report Descriptor
// ===================================================================================
__code uint8_t ReportDescr[] ={
  0x06, 0x00, 0xFF,   // Usage Page = 0xFF00 (Vendor Defined Page 1)
  0x09, 0x01,         // Usage (Vendor Usage 1)
  0xA1, 0x01,         // Collection (Application)
  0x15, 0x00,         //   Logical minimum value 0
  0x25, 0xFF,         //   Logical maximum value 255
  0x75, 0x08,         //   Report Size: 8-bit field size
  0x95, 0x40,         //   Report Count: Make 64 fields
  0x09, 0x01,         //   Usage (Vendor Usage 1)
  0x81, 0x02,         //   Input (Data,Var,Abs,No Wrap,Linear)
  0x09, 0x01,         //   Usage (Vendor Usage 1)
  0x91, 0x02,         //   Output (Data,Var,Abs,No Wrap,Linear)
  0xC0                // End Collection
};

__code uint8_t ReportDescrLen = sizeof(ReportDescr);

// ===================================================================================
// String Descriptors
// ===================================================================================

// Language Descriptor (Index 0)
__code uint16_t LangDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(LangDescr), 0x0409 };  // US English

// Manufacturer String Descriptor (Index 1)
__code uint16_t ManufDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(ManufDescr), MANUFACTURER_STR };

// Product String Descriptor (Index 2)
__code uint16_t ProdDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(ProdDescr), PRODUCT_STR };

// Serial String Descriptor (Index 3)
__code uint16_t SerDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(SerDescr), SERIAL_STR };

// Interface String Descriptor (Index 4)
__code uint16_t InterfDescr1[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(InterfDescr1), INTERFACE_STR_1 };

// Interface String Descriptor (Index 5)
__code uint16_t InterfDescr2[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(InterfDescr2), INTERFACE_STR_2 };
