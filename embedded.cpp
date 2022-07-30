 #include "embedded.hpp"

MCU_Comm* comm = new MCU_Comm();

void Embedded_Init(void)
{
    /*初始化*/
    std::cout << "embedded init .." << std::endl;

    ThreadBindCPU();

    // std::cout << "waiting for spi ..." << std::endl;
    // //等待MCU初始化
    // // comm->WaitMcuPrepare();

    std::cout << "init done!!!" << std::endl;
}

void Embedded()
{
    //与mcu通信
    comm->Communicate2Mcu();
}

imu_t GetRobotEuler(void)
{
    imu_t return_imu={0};
    return_imu = comm->slaveMsg.imu;
    return return_imu;
}

void Set_M_W_Torque(float p_torque)
{
    comm->hostMsg.m_w_torque = p_torque;
}

float Get_M_W_Speed(void)
{
    return comm->slaveMsg.m_w_speed;
}

robotLegMsg_t GetRobotJointMsg(void)
{
    robotLegMsg_t return_legMsg={0};
    return_legMsg.lf=comm->slaveMsg.lf;
    return_legMsg.lr=comm->slaveMsg.lr;
    return_legMsg.rf=comm->slaveMsg.rf;
    return_legMsg.rr=comm->slaveMsg.rr;

    return return_legMsg;
}

void SetRobotJointAngle(robotLegAngle_t legAngle)
{
    comm->hostMsg.lf_angle=legAngle.lf_angle;
    comm->hostMsg.lr_angle=legAngle.lr_angle;
    comm->hostMsg.rf_angle=legAngle.rf_angle;
    comm->hostMsg.rr_angle=legAngle.rr_angle;
}

int8_t ThreadBindCPU(void)
{
    int cpus = 0;
    cpu_set_t mask;

    cpus = sysconf(_SC_NPROCESSORS_CONF);
    std::cout << "cpus: " << cpus << std::endl;

    CPU_ZERO(&mask);    /* 初始化set集，将set置为空*/
    CPU_SET(0, &mask);  /* 依次将0、1、2、3号cpu加入到集合，前提是你的机器是多核处理器*/

    /*设置cpu 亲和性（affinity）*/
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        std::cout << "Set CPU affinity failue, ERROR: "<< strerror(errno) << std::endl;
        return -1;
    }
    usleep(1000); /* 让当前的设置有足够时间生效*/

    return 0;
}

