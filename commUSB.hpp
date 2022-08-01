#ifndef COMMUSB_HPP
#define COMMUSB_HPP

#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <sys/time.h>
#include <iostream>
#include "hidapi.h"

/* ***************************************** option start ***************************************** */
/* print manufacturer info */
// #define PRINT_USB_DEVICE_INFO
/* the transmitted frame should not be greater than 256 bytes after testing */
#define MAX_STR        255
/* VID of the slave computer, two bytes long, obtained from the documents of the chip */
#define USB_VENDOR_ID  0x0483
/* PID of the slave computer, two bytes long, obtained from the documents of the chip */
#define USB_PRODUCT_ID 0x5750
/* example:  tx data = 12 * 4 + 1 * 4 + 1 * 4 + 1 * 4 + 1 * 4 + 6 * 1 */
#define TX_DATA_LEN (48 + 4 + 4 + 4 + 4 + 6)
/* example:  rx data = 4 * ( 7 * 4 ) + 9 * 4 + 5 * 1 */
#define RX_DATA_LEN (112 + 36 + 4 + 5)
/* global variable, USB communication status flag */
enum USB_STATUS{ NORMAL = 0 , ERROR = 1 };
extern enum USB_STATUS usb_status;
/* ***************************************** option end ***************************************** */

class commUSB
{
    private:

    protected:

    public:
        unsigned char txbuf[TX_DATA_LEN];
        unsigned char rxbuf[RX_DATA_LEN]; 
        wchar_t wstr[MAX_STR];
        hid_device *handle;

        commUSB();
        ~commUSB();
        void commUSBInit();
        void commUSBWrite();
        void commUSBRead();
        void commUSBPrintResult();
};

#endif