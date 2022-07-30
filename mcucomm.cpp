#include "mcucomm.hpp"

MCU_Comm::MCU_Comm(void)
{
    commusb = new commUSB;
    commusb->commUSBInit();
}

MCU_Comm::~MCU_Comm(void)
{
    delete commusb;
}

static uint8_t mcuTalkOk = 0;

void MCU_Comm::Communicate2Mcu()
{
    clock_t time_start,time_end;
    commusb->txbuf[0] = 0x0;
    commusb->txbuf[1] = 'H';
    commusb->txbuf[2] = 'M';
    commusb->txbuf[MCU_COMMU_TX_LENGTH - 3] = '\r';
    commusb->txbuf[MCU_COMMU_TX_LENGTH - 2] = '\n';
    commusb->txbuf[MCU_COMMU_TX_LENGTH - 1] = '0';

    //write joint angle to usb_tx_transdata

    WriteUSBData(usb_tx_transdata);

    for(int i = 0; i < (MCU_COMMU_TX_LENGTH - 6); i ++)
    {
        commusb->txbuf[i+3] = usb_tx_transdata[i];
    }

    #ifdef USB_TEST
        std::cout << " =========================== usb_tx_transdata - char ===========================" << std::endl;
        for(int i = 0; i < (MCU_COMMU_TX_LENGTH - 6); i ++)
        {
            std::cout << usb_tx_transdata[i] ;
        }
        std::cout << std::endl;

        std::cout << " =========================== txbuf - char ===========================" << std::endl;
        for(int i = 0; i < (MCU_COMMU_TX_LENGTH); i ++)
        {
            std::cout << commusb->txbuf[i] ;
        }
        std::cout << std::endl;
    #endif
    
    #ifdef PRINT_COMMU_TIME
    time_start=clock();
    #endif
    
    commusb->commUSBWrite();
	commusb->commUSBRead();

    #ifdef PRINT_COMMU_TIME
    time_end=clock();
    std::cout<< "-----------------------TIME--------------------"<<std::endl;
    std::cout<<(double)((time_end-time_start)/1000.0)<<std::endl;
    #endif

    if(commusb->rxbuf[0] == 'S' && commusb->rxbuf[1] == 'M' && commusb->rxbuf[MCU_COMMU_RX_LENGTH-3] == '\r' && commusb->rxbuf[MCU_COMMU_RX_LENGTH-2] == '\n')
    {
        for(int i = 0; i < (MCU_COMMU_RX_LENGTH - 5); i ++)
        {
            usb_rx_transdata[i] = commusb->rxbuf[i+2];
        }
        ReadUSBData(usb_rx_transdata);
    }
    else if(commusb->rxbuf[1] == 'S' && commusb->rxbuf[2] == 'M' && commusb->rxbuf[MCU_COMMU_RX_LENGTH-2] == '\r' && commusb->rxbuf[MCU_COMMU_RX_LENGTH-1] == '\n')
    {
        for(int i = 0; i < (MCU_COMMU_RX_LENGTH - 5); i ++)
        {
            usb_rx_transdata[i] = commusb->rxbuf[i+3];
        }
        ReadUSBData(usb_rx_transdata);
    }

    #ifdef USB_TEST
        std::cout << " =========================== rxbuf - char ===========================" << std::endl;
        for(int i = 0; i < (MCU_COMMU_RX_LENGTH); i ++)
        {
            std::cout << commusb->rxbuf[i] ;
        }
        std::cout << std::endl;

        std::cout << " =========================== usb_tx_transdata - char ===========================" << std::endl;
        for(int i = 0; i < (MCU_COMMU_RX_LENGTH - 5); i ++)
        {
            std::cout << usb_rx_transdata[i] ;
        }
        std::cout << std::endl;
    #endif

    for(uint8_t i = 0; i < MCU_COMMU_RX_LENGTH; i++)
    {
        commusb->rxbuf[i] = '0';
    }
    for(uint8_t i = 0; i < (MCU_COMMU_RX_LENGTH - 5) ; i++)
    {
        usb_rx_transdata[i] = '0';
    }
}


void MCU_Comm::WriteUSBData(char *data)
{   
    union trans_t
    {
        float dataf[16];
        char datac[16*4];
    }trans;
    
    trans.dataf[0] = hostMsg.lf_angle.shoulder_angle;
    trans.dataf[1] = hostMsg.lf_angle.hip_angle;
    trans.dataf[2] = hostMsg.lf_angle.knee_angle;

    trans.dataf[3] = hostMsg.lr_angle.shoulder_angle;
    trans.dataf[4] = hostMsg.lr_angle.hip_angle;
    trans.dataf[5] = hostMsg.lr_angle.knee_angle;

    trans.dataf[6] = hostMsg.rf_angle.shoulder_angle;
    trans.dataf[7] = hostMsg.rf_angle.hip_angle;
    trans.dataf[8] = hostMsg.rf_angle.knee_angle;

    trans.dataf[9] = hostMsg.rr_angle.shoulder_angle;
    trans.dataf[10] = hostMsg.rr_angle.hip_angle;
    trans.dataf[11] = hostMsg.rr_angle.knee_angle;

    trans.dataf[12] = error_flag;
    trans.dataf[13] = power_off_flag;

    // trans.dataf[14] = 1.23456;
    trans.dataf[14] = hostMsg.m_w_torque;
    
    // trans.dataf[15] = 2;
    trans.dataf[15] = stand_begin_flag;

    #ifdef USB_TEST
        for(int i = 0; i < 15; i ++)
        {
            trans.dataf[i] = 1.2;
        }
        std::cout << " =========================== usb_tx_transdata - float ===========================" << std::endl;
        for(int i = 0; i < 14; i ++)
        {
            std::cout << trans.dataf[i] ;
        }
        std::cout << std::endl;
    #endif
    
    for (int j = 0; j < 16*4; j++)
    {
        data[j] = trans.datac[j];
    }
}

void MCU_Comm::ReadUSBData(char* data)
{
    union trans_t       // 驱动器信息
    {
        int32_t dataf[7];
        char datac[28];
    }trans;

    union trans_t_2     // IMU信息
    {
        int32_t dataf[9];
        char datac[36];
    }trans_2;

    union trans_t_3     // 动量轮转速
    {
        int32_t dataf[1];
        char datac[4];
    }trans_3;

    legMsg_t leg[4] = {0};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            trans.datac[j] = data[j+i*28];
        }
        leg[i].shoulder_pos = trans.dataf[0]/1000.0;
        leg[i].shoulder_vel = trans.dataf[3]/1000.0;

        leg[i].hip_pos = trans.dataf[1]/100.0;
        leg[i].hip_vel = trans.dataf[4]/1000.0;

        leg[i].knee_pos = trans.dataf[2]/100.0;
        leg[i].knee_vel = trans.dataf[5]/1000.0;

        leg[i].contact = trans.dataf[6];

        #ifdef USB_TEST
            std::cout << " =========================== usb_rx_transdata - 1 - float ===========================" << std::endl;
            for(int i = 0; i < 7; i ++)
            {
                std::cout << trans.dataf[i]<<"     ";
            }
            std::cout<<std::endl;
        #endif
    }

    slaveMsg.lf = leg[0];
    slaveMsg.lr = leg[1];
    slaveMsg.rf = leg[2];
    slaveMsg.rr = leg[3];

    for(int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            trans_2.datac[i * 4 + j] = data[i * 4 + j + 112];
        }
    }

    slaveMsg.imu.omega_roll = trans_2.dataf[0]/1000.0;
    slaveMsg.imu.omega_pitch = trans_2.dataf[1]/1000.0;
    slaveMsg.imu.omega_yaw = trans_2.dataf[2]/1000.0;
    slaveMsg.imu.roll = trans_2.dataf[3]/1000.0;
    slaveMsg.imu.pitch = trans_2.dataf[4]/1000.0;
    slaveMsg.imu.yaw = trans_2.dataf[5]/1000.0;
    slaveMsg.imu.acc_x = trans_2.dataf[6]/1000.0;
    slaveMsg.imu.acc_y = trans_2.dataf[7]/1000.0;
    slaveMsg.imu.acc_z = trans_2.dataf[8]/1000.0;

    for(int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            trans_3.datac[i * 4 + j] = data[i * 4 + j + 112 + 36];
        }
    }
    slaveMsg.m_w_speed = trans_3.dataf[0]/1000.0;

    #ifdef USB_TEST
            std::cout << " =========================== usb_rx_transdata - 2 - float ===========================" << std::endl;
            for(int i = 0; i < 9; i ++)
            {
                std::cout <<(float)(trans_2.dataf[i]/1000.0) << "  ";
            }
            std::cout<<std::endl;
    #endif
}
