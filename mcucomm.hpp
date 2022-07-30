#include "parameters.hpp"


#ifndef _MCUCOMM_H
#define _MCUCOMM_H

#include <iostream>
#include "cppStruct.h"

extern "C" 
{
	#include <stdlib.h>
	#include <stdint.h>
	#include <unistd.h>
	#include <stdarg.h>
}

#include "commUSB.hpp"

#define MCU_COMMU_TX_LENGTH TX_DATA_LEN
#define MCU_COMMU_RX_LENGTH RX_DATA_LEN

/******************С���Ժ�����֮��*************************/
typedef struct
{
	legMsg_t lf;
	legMsg_t lr;
	legMsg_t rf;
	legMsg_t rr;
	imu_t imu;
	float m_w_speed;
}slaveMsg_t;

typedef struct
{	
	legAngle_t lf_angle;
	legAngle_t lr_angle;
	legAngle_t rf_angle;
	legAngle_t rr_angle;
	uint8_t blank[52];

	float m_w_torque;
}hostMsg_t;

extern int dev_fd;

class MCU_Comm
{
public:
	slaveMsg_t slaveMsg = { 0 };
	hostMsg_t hostMsg = { 0 };
	commUSB *commusb;

	explicit MCU_Comm(void);
	virtual ~MCU_Comm(void);
	// void WaitMcuPrepare(void);
	void Communicate2Mcu();

	void WriteUSBData(char* data);
	void ReadUSBData(char* data);

protected:

private:
	char usb_tx_transdata[MCU_COMMU_TX_LENGTH - 6] = { 0 };
	char usb_rx_transdata[MCU_COMMU_RX_LENGTH - 5] = { 0 };
};


#endif // _MCUCOMM_H

