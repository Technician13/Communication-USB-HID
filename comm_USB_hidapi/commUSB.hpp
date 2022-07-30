#ifndef COMMUSB_HPP
#define COMMUSB_HPP

#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <sys/time.h>
#include <iostream>
#include "hidapi.h"

#include "commonFunction.hpp"

/* after testing, the transmitted frame should not be greater than 256 bytes */
#define MAX_STR        255
/* VID of the slave computer, two bytes long, obtained from the documents of the chip */
#define USB_VENDOR_ID  0x0483
/* PID of the slave computer, two bytes long, obtained from the documents of the chip */
#define USB_PRODUCT_ID 0x5750

#define TX_DATA_LEN (48 + 4 + 4 + 4 + 4 + 6)    // 12个关节角度+错误警报标志位+紧急关电+动量轮扭矩+站立标志+6个帧头帧尾     14*4+4+4+6
#define RX_DATA_LEN (112 + 36 + 4 + 5)      // 每个腿的驱动器信息+imu信息+动量轮转速+帧头帧尾  4*7*4 + 9*4 + 4 + 5

/* global variable, USB communication status flag */
enum USB_STATUS{ NORMAL = 0 , ERROR = 1 };
extern enum USB_STATUS usb_status;

class commUSB
{
    private:

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