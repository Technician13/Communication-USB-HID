#include "commUSB.hpp"

enum USB_STATUS usb_status = NORMAL;

/* constructor */
commUSB::commUSB()
{

}

/* destructor */
commUSB::~commUSB()
{
    /* close the device */
    hid_close(handle);
    /* finalize the hidapi lib */
    hid_exit();
}

/* initialization function */
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
        
    /* open the device using the VID, PID, and optionally the Serial number */
    handle = hid_open(USB_VENDOR_ID , USB_PRODUCT_ID , NULL);
    if(handle == NULL)
    {
        std::cout << "hid_open failed" << std::endl;
        usb_status = ERROR;
    }
        
    /* read the manufacturer string */
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
    
    /* read the product string */
    res = hid_get_product_string(handle , wstr , MAX_STR);
    if(res == -1)
    {
        std::cout << "hid_get_product_string failed" << std::endl;
        usb_status = ERROR;
    }
    #ifdef PRINT_USB_DEVICE_INFO
        wprintf(L"Product String: %s\n" , wstr);
    #endif

    /* read the serial number string */
    res = hid_get_serial_number_string(handle , wstr , MAX_STR);
    if(res == -1)
    {
        std::cout << "hid_get_serial_number_string failed" << std::endl;
        usb_status = ERROR;
    }
    #ifdef PRINT_USB_DEVICE_INFO
        wprintf(L"Serial Number String: (%d) %s\n" , wstr[0] , wstr);
    #endif

    /* read indexed string 1 */
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

/* tranform function */
void commUSB::commUSBWrite()
{
    hid_write(handle , txbuf , TX_DATA_LEN);
}

/* receive function */
void commUSB::commUSBRead()
{
    hid_read(handle , rxbuf , RX_DATA_LEN);
}

/* print tx data & rx data info */
void commUSB::commUSBPrintResult()
{
    /* print the tx data */
    for (int i = 0 ; i < TX_DATA_LEN ; i++)
    {
        std::cout << "txbuf[ " << i << " ]: " << txbuf[i] << std::endl;
    }
        
    /* print the rx data */
    for (int i = 0 ; i < RX_DATA_LEN ; i++)
    {
        std::cout << "rxbuf[ " << i << " ]: " << rxbuf[i] << std::endl;
    }
}