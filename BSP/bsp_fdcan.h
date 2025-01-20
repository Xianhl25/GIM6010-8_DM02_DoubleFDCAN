#ifndef __BSP_FDCAN_H__
#define __BSP_FDCAN_H__

#include "bsp.h"
#include "fdcan.h"

/* 帧类型 */
typedef enum frame_type
{
    DATA_FRAME   = 0,  /* 数据帧 */
    REMOTE_FRAME = 1,  /* 远程帧 */
    
}_FRAME_TYPE;


/* 接收CAN报文结构体 */
typedef struct canmsg
{
	__IO uint16_t     id;         /* CANID */
    _FRAME_TYPE       rtr;        /* 远程帧，数据帧 */
    __IO uint8_t      len;        /* CAN报文长度 */
    __IO uint8_t      buffer[8];  /* CAN报文内容 */

}_CANMSG;

/* FACAN对象结构体 */
typedef struct fdcan_handler
{
	/* 成员 */
	_CANMSG rx_MSG;                   /* CAN报文结构体 */
	__IO uint32_t RXFIFO_Inter_Type;  /* RXFIFO中断的类型（新数据，水印，FIFO溢出) */
	__IO uint32_t RXFxS;              /* FDCAN_RXF0C 或 FDCAN_RXF1C */
	__IO uint8_t  FxGI;               /* FDCAN_F0GI 或 FDCAN_F1GI */
	__IO uint8_t  FxFL;               /* FDCAN_F0FL 或 FDCAN_F1FL */
	__IO uint8_t  FxPI;               /* FDCAN_F0PI 或 FDCAN_F1PI */

	/* 方法 */
    void    (*start)(void);
    void    (*rx_Filter_Init)(void);
    void    (*rx_Interrupt_Init)(void);
    uint8_t (*send_MSG)(_CANMSG*);
    void    (*update_RXFIFO_Status)(FDCAN_HandleTypeDef *,struct fdcan_handler *);
    
}_FDCAN_HANDLER;




void FDCAN1_RX_Filter0_Init(void);
void FDCAN2_RX_Filter0_Init(void);
void FDCAN1_Interrupt_Init(void);
void FDCAN2_Interrupt_Init(void);
uint8_t FDCAN1_Send_Msg(_CANMSG *msg);
uint8_t FDCAN2_Send_Msg(_CANMSG *msg);
void FDCAN1_Start(void);
void FDCAN2_Start(void);
uint8_t look_FIFOSR_TXEFS_Bit_EFF(FDCAN_HandleTypeDef *hdfcan);
void update_RXFIFO_Status(FDCAN_HandleTypeDef *hfdcan,struct fdcan_handler *fdcan);
HAL_StatusTypeDef CAN_Send_StdDataFrame(FDCAN_HandleTypeDef *hfdcan, uint32_t Identifier, uint8_t *msg);

/* 外部变量 */
extern _FDCAN_HANDLER fdcan1;
extern _FDCAN_HANDLER fdcan2;
extern FDCAN_RxHeaderTypeDef hfdcan1_RX;        
extern FDCAN_RxHeaderTypeDef hfdcan2_RX; 
extern FDCAN_TxHeaderTypeDef hfdcan1_TX;        
extern FDCAN_TxHeaderTypeDef hfdcan2_TX;   

#endif /*__BSP_FDCAN_H */






