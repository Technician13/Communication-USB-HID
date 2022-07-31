#ifndef MCUCOMM_HPP
#define MCUCOMM_HPP

#include <iostream>
#include "commUSB.hpp"
extern "C" 
{
	#include <stdlib.h>
	#include <stdint.h>
	#include <unistd.h>
	#include <stdarg.h>
}

/* a little redundant operation... */
#define MCU_COMMU_TX_LENGTH TX_DATA_LEN
#define MCU_COMMU_RX_LENGTH RX_DATA_LEN

/* ******************************** option start ******************************** */
/* print tx frame data info in float */
// #define USB_TEST_TX_FRAME_DATA_FLOAT
/* print tx frame data info in char */
// #define USB_TEST_TX_FRAME_DATA_CHAR
/* print tx frame info in char */
// #define USB_TEST_TX_FRAME_CHAR

/* print rx frame info */
// #define USB_TEST_RX_FRAME_DATA_FLOAT
/* print rx frame data info in char */
// #define USB_TEST_RX_FRAME_DATA_CHAR
/* print rx frame info in char */
// #define USB_TEST_RX_FRAME_CHAR

/* time counting */
// #define PRINT_COMMU_TIME
/* ******************************** option end ******************************** */

class MCU_Comm
{
	private:

	protected:
	
	public:
		commUSB *commusb;

		char usb_tx_transdata[MCU_COMMU_TX_LENGTH - 6] = { 0 };
		char usb_rx_transdata[MCU_COMMU_RX_LENGTH - 5] = { 0 };

		explicit MCU_Comm(void);
		virtual ~MCU_Comm(void);
		void Communicate2Mcu();
		void WriteUSBData(char* data);
		void ReadUSBData(char* data);	
};

#endif