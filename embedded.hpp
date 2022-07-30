#include "parameters.hpp"

#ifndef __EMBEDDED_HPP
#define __EMBEDDED_HPP

#include "mcucomm.hpp"
#include "cppStruct.h"
#include <thread>
#include <sched.h>
#include <pthread.h> 
#include <sys/time.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <iostream>

extern "C" {

#include <errno.h>
#include <unistd.h>
}

void Embedded_Init(void);
void Embedded();
void ConfigTask(void);
int8_t ThreadBindCPU(void);
imu_t GetRobotEuler(void);
void Set_M_W_Torque(float p_torque);
float Get_M_W_Speed(void);
robotLegMsg_t GetRobotJointMsg(void);
void SetRobotJointAngle(robotLegAngle_t legAngle);
#endif
