#ifndef __ODRIVEMOTOR_H
#define __ODRIVEMOTOR_H

#include <stdint.h>
#include <string.h>

#define MAX_BUS_NUM 8
#define MAX_MOTOR_NUM_PER_BUS 16

typedef enum {
    MW_ERROR_SUCCESS = 0,
    MW_ERROR_OUT_OF_RANGE = 1,
} MW_ERROR;

typedef enum {
    MW_ENDPOINT_DATA_TYPE_INT = 0,
    MW_ENDPOINT_DATA_TYPE_FLOAT = 1,
} MW_ENDPOINT_DATA_TYPE;

typedef struct {
    MW_ENDPOINT_DATA_TYPE type;
    union {
        int32_t intData;
        float floatData;
    } data;
} MW_ENDPOINT_DATA;

typedef enum {
    MW_ENDPOINT_OPCODE_READ = 0,
    MW_ENDPOINT_OPCODE_WRITE = 1,
} MW_ENDPOINT_OPCODE;

typedef struct {
    MW_ENDPOINT_OPCODE opcode;
    uint16_t id;
    MW_ENDPOINT_DATA data;
} MW_ENDPOINT;

/**
 * @brief MW_CMD_ID指令
 */
typedef enum {
    MW_HEARTBEAT_CMD                  = 0x001,
    MW_ESTOP_CMD                      = 0x002,
    MW_GET_ERROR_CMD                  = 0x003,
    MW_RXSDO_CMD                      = 0x004,
    MW_TXSDO_CMD                      = 0x005,
    MW_SET_AXIS_NODE_ID_CMD           = 0x006,
    MW_SET_AXIS_STATE_CMD             = 0x007,
    MW_MIT_CONTROL_CMD                = 0x008,
    MW_GET_ENCODER_ESTIMATES_CMD      = 0x009,
    MW_GET_ENCODER_COUNT_CMD          = 0x00A,
    MW_SET_CONTROLLER_MODE_CMD        = 0x00B,
    MW_SET_INPUT_POS_CMD              = 0x00C,
    MW_SET_INPUT_VEL_CMD              = 0x00D,
    MW_SET_INPUT_TORQUE_CMD           = 0x00E,
    MW_SET_LIMITS_CMD                 = 0x00F,
    MW_START_ANTICOGGING_CMD          = 0x010,
    MW_SET_TRAJ_VEL_LIMIT_CMD         = 0x011,
    MW_SET_TRAJ_ACCEL_LIMIT_CMD       = 0x012,
    MW_SET_TRAJ_INERTIA_CMD           = 0x013,
    MW_GET_IQ_CMD                     = 0x014,
    MW_GET_SENSORLESS_ESTIMATES_CMD   = 0x015,
    MW_REBOOT_CMD                     = 0x016,
    MW_GET_BUS_VOLTAGE_CURRENT_CMD    = 0x017,
    MW_CLEAR_ERRORS_CMD               = 0x018,
    MW_SET_LINEAR_COUNT_CMD           = 0x019,
    MW_SET_POS_GAIN_CMD               = 0x01A,
    MW_SET_VEL_GAIN_CMD               = 0x01B,
    MW_GET_TORQUES_CMD                = 0x01C,
    MW_GET_POWERS_CMD                 = 0x01D,
    MW_DISABLE_CAN_CMD                = 0x01E,
    MW_SAVE_CONFIGURATION_CMD         = 0x01F
} MW_CMD_ID;

/**
 * @brief 电机状态
 */
typedef enum { 
    MW_AXIS_STATE_UNDEFINED                            = 0x0,
    MW_AXIS_STATE_IDLE                                 = 0x1,
    MW_AXIS_STATE_STARTUP_SEQUENCE                     = 0x2,
    MW_AXIS_STATE_FULL_CALIBRATION_SEQUENCE            = 0x3,
    MW_AXIS_STATE_MOTOR_CALIBRATION                    = 0x4,
    MW_AXIS_STATE_ENCODER_INDEX_SEARCH                 = 0x6,
    MW_AXIS_STATE_ENCODER_OFFSET_CALIBRATION           = 0x7,
    MW_AXIS_STATE_CLOSED_LOOP_CONTROL                  = 0x8,
    MW_AXIS_STATE_LOCKIN_SPIN                          = 0x9,
    MW_AXIS_STATE_ENCODER_DIR_FIND                     = 0xA,
    MW_AXIS_STATE_HOMING                               = 0xB,
    MW_AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION    = 0xC,
    MW_AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION       = 0XD,
    MW_AXIS_STATE_ANTICOGGING_CALIBRATION              = 0XE
} MW_MOTER_STATE;

/**
 * @brief 电机控制模式
 */
typedef enum { 
    MW_VOLTAGE_CONTROL          = 0,
    MW_TORQUE_CONTROL           = 1,
    MW_VELOCITY_CONTROL         = 2,
    MW_POSITION_CONTROL         = 3
} MW_CONTROL_MODE;

/**
 * @brief 电机输入模式
 */
typedef enum { 
    MW_IDLE_INPUT               = 0,
    MW_DIRECT_CONTROL_INPUT     = 1,
    MW_RAMP_RATE_INPUT          = 2,
    MW_POSITION_FILTERING_INPUT = 3,
    MW_TRAPEZOIDAL_CURVE_INPUT  = 5,
    MW_TORQUE_RAMP_INPUT        = 6,
    MW_MIT_INPUT                = 9
} MW_INPUT_MODE;

/**
 * @brief 电机错误类型
 */
typedef enum {
    MW_MOTOR_ERROR = 0,
    MW_ENCODER_ERROR = 1,
    MW_SENSORLESS_ERROR = 2,
    MW_CONTROLLER_ERROR = 3,
} MW_ERROR_TYPE;

/**
 * @brief MW电机MIT模式数据结构体
 */
typedef struct {
    double targetPos;
    double ffVel;
    double kp;
    double kd;
    double ffTorque;
} MW_MIT_CTRL_INPUT;

/**
 * @brief MW电机数据结构体
 */
typedef struct {
    uint8_t busId;                       //!<@brief 驱动器总线
    uint8_t nodeId;                      //!<@brief 驱动器节点
    
    struct {                                  
        uint32_t axisError;              //!<@brief 无感异常位
        uint8_t motorErrorFlag;          //!<@brief 电机异常位
        uint8_t encoderErrorFlag;        //!<@brief 编码器异常位
        uint8_t controllerErrorFlag;     //!<@brief 控制器异常位
    } ErrorStatus;                       //!<@brief 异常标志位
    uint32_t error;                      //!<@brief 异常获取
    uint8_t currentState;                //!<@brief 当前电机状态
                                           
    float encoderPosEstimate;            //!<@brief 编码器位置预估值
    float encoderVelEstimate;            //!<@brief 编码器速度预估值
    float sensorlessPosEstimate;         //!<@brief 锁相环输出角度
    float sensorlessVelEstimate;         //!<@brief 锁相环速度
    float torque;                        //!<@brief 实际力矩
    float torqueSetpoint;                //!<@brief 转矩控制器实际使用的转矩参考
    float torqueConstant;                //!<@brief 转矩常数
    int32_t shadowCount;                 //!<@brief 每转脉冲数
    int32_t countInCPR;                  //!<@brief 每转计数(CPR)
    float iqSetpoint;                    //!<@brief 电机电流设置参数
    float iqMeasured;                    //!<@brief 电机相电流
                                           
    float busVoltage;                    //!<@brief 电源电压
    float busCurrent;                    //!<@brief 电源电流
    float electricalPower;               //!<@brief 电机功率
    float mechanicalPower;               //!<@brief 电机机械功率
    float motorTemperature;              //!<@brief 电机温度
    float fetTemperature;                //!<@brief 驱动器温度
                                           
    MW_MIT_CTRL_INPUT motorMIT;          //!<@brief MIT模式下控制返回参数
                                           
    uint8_t controlMode;                 //!<@brief 电机控制模式
    uint8_t inputMode;                   //!<@brief 电机输入模式
    float velGain;                       //!<@brief 速度环PID控制的P值
    float velIntegratorGain;             //!<@brief 速度环PID控制的I值
    float posGain;                       //!<@brief 位置环PID控制的P值
                                      
    MW_ENDPOINT EndpointData;
} MW_MOTOR_DATA;

typedef void (*MotorSender)(uint8_t busId, uint8_t canId, uint8_t *data, uint8_t dataSize);
typedef void (*MotorNotifier)(uint8_t busId, uint8_t nodeId, MW_CMD_ID cmdId);

typedef struct {
    uint8_t busId;
    uint8_t nodeId;
    MW_MOTOR_DATA *motorData;
    MotorSender sender;
    MotorNotifier notifier;
} MWMotorAccessInfo;


/* 所有总线挂的所有电机访问信息 */
extern MWMotorAccessInfo motors[MAX_BUS_NUM][MAX_MOTOR_NUM_PER_BUS];

/**
 * 注册电机
 * 
 * 本函数用于在系统中注册一个电机，将其接入到指定的总线和节点上。
 * 注册成功返回MW_ERROR_SUCCESS，否则返回MW_ERROR_OUT_OF_RANGE。
 * 
 * @param motor 一个结构体，包含电机的总线ID和节点ID，以及电机的访问信息。
 * @return 如果电机的总线ID或节点ID超出范围，则返回MW_ERROR_OUT_OF_RANGE；
 *         否则返回MW_ERROR_SUCCESS，表示注册成功。
 */
uint8_t MWRegisterMotor(MWMotorAccessInfo motor);

/**
 * @brief 用户的程序中需要调用这具函数，处理接收到的针对MW电机的CAN消息。
 * 
 * 此函数根据CAN消息的ID和数据，执行相应的处理逻辑。它解析CAN消息中的节点ID和命令ID，
 * 并根据这些信息调用相应的处理函数。这个函数是处理MW电机控制系统中CAN通信的核心函数。
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param canId CAN消息的ID，用于识别节点ID和命令ID。
 * @param data CAN消息的数据部分，包含命令的具体数据。
 */
void MWReceiver(uint8_t busId, uint32_t canId, uint8_t *data);

/**
 * 紧急停止指定总线和节点上的电机
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @note cmd_id: 0x002
 */
void MWEstop(uint8_t busId, uint8_t nodeId);

/**
 * @brief 获取电机错误信息
 * 
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param errorType 错误类型，指定要查询的错误类型
 * @note cmd_id: 0x003
 */
void MWGetMotorError(uint8_t busId, uint8_t nodeId, MW_ERROR_TYPE errorType);

/**
 * 访问电机控制器中任意端点数据，所谓端点数据是指电机的状态、参数、错误等数据。
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param endpointData 端点数据结构体，包含操作码、端点ID和数据
 * @note cmd_id: 0x004
 */
void MWRxSdo(uint8_t busId, uint8_t nodeId, MW_ENDPOINT endpointData);

/**
 * 设置电机的节点ID
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param newNodeId 新的节点ID，即将被设置为电机的新的节点ID。
 * @param motorInfo 电机访问信息。
 * @note cmd_id: 0x006
 */
void MWSetAxisNodeID(uint8_t busId, uint8_t nodeId, uint8_t newNodeId, MWMotorAccessInfo *motorInfo);

/**
 * 设置电机状态
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param state 电机的新状态，具体状态含义由上层应用定义。
 * @note cmd_id: 0x007
 */
void MWSetAxisState(uint8_t busId, uint8_t nodeId, MW_MOTER_STATE state);

/**
 * MIT运动控制
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param mit 控制输入参数，包含目标位置、前馈速度、前馈力矩、增益等信息
 * @note cmd_id: 0x008
 */
void MWMitControl(uint8_t busId, uint8_t nodeId, MW_MIT_CTRL_INPUT *mit);

/**
 * 获取编码器的速度和位置信息
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。\
 * @note cmd_id: 0x009
 */
void MWGetEncoderEstimates(uint8_t busId, uint8_t nodeId);

/**
 * 设置电机控制器的控制模式和输入模式。
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param ctrlMode 控制器模式，指定电机控制器的工作模式。
 * @param inputMode 输入模式，指定电机控制器的输入模式。
 * @note cmd_id: 0x00B
 */
void MWSetControllerMode(uint8_t busId, uint8_t nodeId, MW_CONTROL_MODE ctrlMode, MW_INPUT_MODE inputMode);

/**
 * 位置控制，输入电机位置、速度前馈和力矩前馈。
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param inputPos 电机的输入位置，通常是由外部传感器提供的期望位置，单位turns。
 * @param velFF 速度前馈值，用于补偿电机系统的惯性或阻力，单位0.001turns/s。
 * @param torqueFF 力矩前馈值，用于补偿电机系统的摩擦或重力影响，单位0.001Nm。
 * @note cmd_id: 0x00C
 */
void MWPosControl(uint8_t busId, uint8_t nodeId, float inputPos, int16_t velFF, int16_t torqueFF);

/**
 * @brief 速度控制，输入电机速度和力矩前馈。
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param inputVel 期望输入速度，单位turns/s。
 * @param torqueFF 扭矩前馈值，用于补偿或增强电机的扭矩，单位Nm。
 * @note cmd_id: 0x00D
 */
void MWVelControl(uint8_t busId, uint8_t nodeId, float inputVel, float torqueFF);

/**
 * @brief 力矩控制
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param inputTorque 输入扭矩值，单位Nm。
 * @note cmd_id: 0x00E
 */
void MWTorqueControl(uint8_t busId, uint8_t nodeId, float inputTorque);

/**
 * 设置电机限速和限流
 * 
 * 本函数用于通过指定的总线ID和电机节点ID，设置该电机的最高速度限制和最大电流限制。
 * 这些限制用于保护电机和相关电路，避免过高的速度或电流导致损坏。
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param velLim 电机的速度限制，单位turns/s。
 * @param currLim 电机的电流限制，单位为安培（A）。
 * @note cmd_id: 0x00F
 */
void MWSetLimits(uint8_t busId, uint8_t nodeId, float velLim, float currLim);

/**
 * @brief 进行力矩纹波校准
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @note cmd_id: 0x010
 */
void MWStartAnticogging(uint8_t busId, uint8_t nodeId);

/**
 * @brief 设置梯形曲线位置控制滑行速度限制
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param velLim 滑行速度最大值
 * @note cmd_id: 0x011
 */
void MWSetTrajVelLimit(uint8_t busId, uint8_t nodeId, float velLim);

/**
 * @brief 设置梯形曲线位置控制加速度限制
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param accLim 加速度最大值
 * @param decLim 减速度最大值
 * @note cmd_id: 0x012
 */
void MWSetTrajAccelLimits(uint8_t busId, uint8_t nodeId, float accLim, float decLim);

/**
 * @brief 设置梯形曲线位置控制惯量
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param inertia 惯量
 * @note cmd_id: 0x013
 */
void MWSetTrajInertia(uint8_t busId, uint8_t nodeId, float inertia);

/**
 * @brief 电机重启
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @note cmd_id: 0x016
 */
void MWReboot(uint8_t busId, uint8_t nodeId);

/**
 * @brief 清除所有错误
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @note cmd_id: 0x018
 */
void MWClearErrors(uint8_t busId, uint8_t nodeId);

/**
* @brief 设置电机位置环PID控制的P值

 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param posgain 位置环PID控制的P值
 * @note cmd_id: 0x01A
 */
void MWSetPosGain(uint8_t busId, uint8_t nodeId, float posGain);

/**
* @brief 设置速度环PID控制的P值，I值

 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @param velGain 位置环PID控制的P值
 * @param velIntGain 位置环PID控制的I值
 * @note cmd_id: 0x01B
 */
void MWSetVelGain(uint8_t busId, uint8_t nodeId, float velGain, float velIntGain);

/**
 * @brief 禁用CAN
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @note cmd_id: 0x01E
 */
void MWDisableCAN(uint8_t busId, uint8_t nodeId);

/**
 * @brief 保存电机设置并重启电机
 * 
 * @param busId 电机所在总线的ID。总线ID用于识别电机控制系统中的不同总线。
 * @param nodeId 电机在总线上的节点ID。节点ID用于在总线上唯一标识每个电机。
 * @note cmd_id: 0x01F
 */
void MWSaveConfigeration(uint8_t busId, uint8_t nodeId);

#endif
