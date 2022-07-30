#include "commUSB.hpp"

enum USB_STATUS usb_status = NORMAL;

commUSB::commUSB()
{

}

commUSB::~commUSB()
{
    /* close the device */
    hid_close(handle);
    /* finalize the hidapi lib */
    hid_exit();
}

void commUSB::commUSBInit()
{
    int res;

    /* initialize the hidapi lib */
    res = hid_init();
    if(res == -1)
    {
        std::cout << "hid_init failed" << std::endl;
        usb_status = ERROR;
    }
        
    // Open the device using the VID, PID, and optionally the Serial number.
    handle = hid_open(USB_VENDOR_ID , USB_PRODUCT_ID , NULL);
    if(handle == NULL)
    {
        std::cout << "hid_open failed" << std::endl;
        usb_status = ERROR;
    }
        

    // Read the Manufacturer String
    res = hid_get_manufacturer_string(handle , wstr , MAX_STR);
    if(res == -1)
    {
        std::cout << "hid_get_manufacturer_string failed" << std::endl;
        usb_status = ERROR;
    }
    #ifdef PRINT_USB_DEVICE_INFO
        std::cout << res << std::endl;
        wprintf(L"Manufacturer String: %s\n" , wstr);
    #endif
    
    // Read the Product String
    res = hid_get_product_string(handle , wstr , MAX_STR);
    if(res == -1)
    {
        std::cout << "hid_get_product_string failed" << std::endl;
        usb_status = ERROR;
    }
    #ifdef PRINT_USB_DEVICE_INFO
        wprintf(L"Product String: %s\n" , wstr);
    #endif

    // Read the Serial Number String
    res = hid_get_serial_number_string(handle , wstr , MAX_STR);
    if(res == -1)
    {
        std::cout << "hid_get_serial_number_string failed" << std::endl;
        usb_status = ERROR;
    }
    #ifdef PRINT_USB_DEVICE_INFO
        wprintf(L"Serial Number String: (%d) %s\n" , wstr[0] , wstr);
    #endif

    // Read Indexed String 1
    res = hid_get_indexed_string(handle , 1 , wstr , MAX_STR);
    if(res == -1)
    {
        std::cout << "hid_get_indexed_string failed" << std::endl;
        usb_status = ERROR;
    }
    #ifdef PRINT_USB_DEVICE_INFO
        wprintf(L"Indexed String 1: %s\n" , wstr);
    #endif
}

void commUSB::commUSBWrite()
{
    hid_write(handle , txbuf , TX_DATA_LEN);
}

void commUSB::commUSBRead()
{
    hid_read(handle , rxbuf , RX_DATA_LEN);
}

void commUSB::commUSBPrintResult()
{
    // Print out the returned buffer.
    for (int i = 0 ; i < TX_DATA_LEN ; i++)
    {
        std::cout << "txbuf[ " << i << " ]: " << txbuf[i] << std::endl;
    }
        
    // Print out the returned buffer.
    for (int i = 0 ; i < RX_DATA_LEN ; i++)
    {
        std::cout << "rxbuf[ " << i << " ]: " << rxbuf[i] << std::endl;
    }
}

