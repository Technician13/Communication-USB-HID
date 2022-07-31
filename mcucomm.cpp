#include "mcucomm.hpp"

/* constructor */
MCU_Comm::MCU_Comm(void)
{
    commusb = new commUSB;
    commusb->commUSBInit();
}

/* destructor */
MCU_Comm::~MCU_Comm(void)
{
    delete commusb;
}

/* load tx data */
void MCU_Comm::WriteUSBData(char *data)
{   
    /* ******************************** option start ******************************** */
    union trans_t
    {
        float dataf[16];
        char datac[16 * 4];
    }trans;
    
    trans.dataf[0] = 1.2;
    trans.dataf[1] = 1.2;
    trans.dataf[2] = 1.2;
    trans.dataf[3] = 1.2;
    trans.dataf[4] = 1.2;
    trans.dataf[5] = 1.2;
    trans.dataf[6] = 1.2;
    trans.dataf[7] = 1.2;
    trans.dataf[8] = 1.2;
    trans.dataf[9] = 1.2;
    trans.dataf[10] = 1.2;
    trans.dataf[11] = 1.2;
    trans.dataf[12] = 1.2;
    trans.dataf[13] = 1.2;
    trans.dataf[14] = 1.2;
    trans.dataf[15] = 1.2;

    #ifdef USB_TEST_TX_FRAME_DATA_FLOAT
        std::cout << "----------------------------------------- TX_FRAME_DATA_FLOAT -----------------------------------------" << std::endl;
        for(int i = 0; i < 16; i++)
        {
            std::cout << trans.dataf[i] << " ";
        }
        std::cout << std::endl;
    #endif
    
    for (int j = 0; j < 16 * 4; j++)
    {
        data[j] = trans.datac[j];
    }
    /* ******************************** option end ******************************** */
}

/* load rx data */
void MCU_Comm::ReadUSBData(char* data)
{
    /* ******************************** option start ******************************** */
    union trans_t       
    {
        int32_t dataf[38];
        char datac[38 * 4];
    }trans;

    for(int i = 0; i < 152; i++)
    {
        trans.datac[i] = data[i];
    }

    #ifdef USB_TEST_RX_FRAME_DATA_FLOAT
        std::cout << "----------------------------------------- RX_FRAME_DATA_FLOAT -----------------------------------------" << std::endl;
        for(int i = 0; i < 38; i++)
        {
            std::cout << (float)(trans.dataf[i] / 10.0) << " ";
        }
        std::cout<<std::endl;
    #endif
    /* ******************************** option end ******************************** */
}

/* communication function */
void MCU_Comm::Communicate2Mcu()
{
    /* for time counting */
    #ifdef PRINT_COMMU_TIME
        clock_t time_start, time_end;
    #endif

    /* load tx data to usb_tx_transdata */
    WriteUSBData(usb_tx_transdata);

    /* extend with head & tail */
    commusb->txbuf[0] = 0x0;
    commusb->txbuf[1] = 'H';
    commusb->txbuf[2] = 'M';
    commusb->txbuf[MCU_COMMU_TX_LENGTH - 3] = '\r';
    commusb->txbuf[MCU_COMMU_TX_LENGTH - 2] = '\n';
    commusb->txbuf[MCU_COMMU_TX_LENGTH - 1] = '0';
    for(int i = 0; i < (MCU_COMMU_TX_LENGTH - 6); i++)
    {
        commusb->txbuf[i+3] = usb_tx_transdata[i];
    }

    #ifdef USB_TEST_TX_FRAME_DATA_CHAR
        std::cout << "----------------------------------------- TX_FRAME_DATA_CHAR -----------------------------------------" << std::endl;
        for(int i = 0; i < (MCU_COMMU_TX_LENGTH - 6); i++)
        {
            std::cout << usb_tx_transdata[i] << " ";
        }
        std::cout << std::endl;
    #endif

    #ifdef USB_TEST_TX_FRAME_CHAR
        std::cout << "----------------------------------------- TX_FRAME_CHAR -----------------------------------------" << std::endl;
        for(int i = 0; i < (MCU_COMMU_TX_LENGTH); i++)
        {
            std::cout << commusb->txbuf[i] << " ";
        }
        std::cout << std::endl;
    #endif
    
    #ifdef PRINT_COMMU_TIME
        time_start = clock();
    #endif
    
    /* communication */
    commusb->commUSBWrite();
	commusb->commUSBRead();

    #ifdef PRINT_COMMU_TIME
        time_end = clock();
        std::cout << "----------------------- COMMU_TIME --------------------" << std::endl;
        std::cout << (double)((time_end - time_start) / 1000.0) << std::endl;
    #endif

    /* shift checking */
    if(commusb->rxbuf[0] == 'S' && commusb->rxbuf[1] == 'M' && commusb->rxbuf[MCU_COMMU_RX_LENGTH-3] == '\r' && commusb->rxbuf[MCU_COMMU_RX_LENGTH-2] == '\n')
    {
        for(int i = 0; i < (MCU_COMMU_RX_LENGTH - 5); i ++)
        {
            usb_rx_transdata[i] = commusb->rxbuf[i + 2];
        }
        ReadUSBData(usb_rx_transdata);
    }
    else if(commusb->rxbuf[1] == 'S' && commusb->rxbuf[2] == 'M' && commusb->rxbuf[MCU_COMMU_RX_LENGTH-2] == '\r' && commusb->rxbuf[MCU_COMMU_RX_LENGTH-1] == '\n')
    {
        for(int i = 0; i < (MCU_COMMU_RX_LENGTH - 5); i ++)
        {
            usb_rx_transdata[i] = commusb->rxbuf[i + 3];
        }
        ReadUSBData(usb_rx_transdata);
    }

    #ifdef USB_TEST_RX_FRAME_CHAR
        std::cout << "----------------------------------------- RX_FRAME_CHAR -----------------------------------------" << std::endl;
        for(int i = 0; i < (MCU_COMMU_RX_LENGTH); i++)
        {
            std::cout << commusb->rxbuf[i] << " ";
        }
        std::cout << std::endl;
    #endif

    #ifdef USB_TEST_RX_FRAME_DATA_CHAR
        std::cout << "----------------------------------------- RX_FRAME_DATA_CHAR -----------------------------------------" << std::endl;
        for(int i = 0; i < (MCU_COMMU_RX_LENGTH - 5); i++)
        {
            std::cout << usb_rx_transdata[i] ;
        }
        std::cout << std::endl;
    #endif

    /* clear receive container */
    for(uint8_t i = 0; i < MCU_COMMU_RX_LENGTH; i++)
    {
        commusb->rxbuf[i] = '0';
    }
    for(uint8_t i = 0; i < (MCU_COMMU_RX_LENGTH - 5) ; i++)
    {
        usb_rx_transdata[i] = '0';
    }
}