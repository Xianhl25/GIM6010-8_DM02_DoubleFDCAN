/* 通过STM32CubeMX初始化FDCAN控制后，还要进一步设置后，FDCAN才能正常运行。
 * 初始化滤波器 -> 初始化中断 -> 开启FDCAN控制
 */
#include "bsp_fdcan.h"

/* 外部变量 */
extern FDCAN_HandleTypeDef hfdcan1;       			   /* FDCAN1的对象句柄 */
extern FDCAN_HandleTypeDef hfdcan2;                /* FDCAN2的对象句柄 */


/* 内部变量 */
static FDCAN_FilterTypeDef hfdcan1_RX_Filter0;   /* FDCAN1滤波器0对象句柄 */
static FDCAN_FilterTypeDef hfdcan2_RX_Filter0;   /* FDCAN2滤波器0对象句柄 */

/* 全局变量 */
FDCAN_RxHeaderTypeDef hfdcan1_RX;         /* FDCAN1接收处理单元的对象句柄 */
FDCAN_RxHeaderTypeDef hfdcan2_RX;         /* FDCAN2接收处理单元的对象句柄 */
FDCAN_TxHeaderTypeDef hfdcan1_TX;         /* FDCAN1发送处理单元的对象句柄 */
FDCAN_TxHeaderTypeDef hfdcan2_TX;         /* FDCAN2发送处理单元的对象句柄 */

_FDCAN_HANDLER fdcan1 = {  .rx_MSG = {0},
                           .start = FDCAN1_Start,
                           .rx_Filter_Init = FDCAN1_RX_Filter0_Init,
                           .rx_Interrupt_Init = FDCAN1_Interrupt_Init,
                           .send_MSG = FDCAN1_Send_Msg,
						               .update_RXFIFO_Status = update_RXFIFO_Status,
};
_FDCAN_HANDLER fdcan2 = {  .rx_MSG = {0},
                           .start = FDCAN2_Start,
                           .rx_Filter_Init = FDCAN2_RX_Filter0_Init,
                           .rx_Interrupt_Init = FDCAN2_Interrupt_Init,
                           .send_MSG = FDCAN2_Send_Msg,
						               .update_RXFIFO_Status = update_RXFIFO_Status,
};


/**
  * 函数功能: 设置并初始化滤波器0,供FDCAN1使用
  * 输入参数: void
  *           
  * 返回值：  void
  *           
  *
  * 说明:
  *     1. 通过将成员FilterType设为FDCAN_FILTER_MASK,再加上成员FilterID1与FilterID2设为0x00,实现接收所有CAN ID.
  *     2. FDCAN1只设置了使用一个滤波器，所以成员FilterIndex设0，因为是从0开始的。
  *     3. FDCAN1设置了使用FIFO0存放CAN报文，所以滤波器需要关联到FIFO0.
  */
void FDCAN1_RX_Filter0_Init(void)
{
   hfdcan1_RX_Filter0.IdType = FDCAN_STANDARD_ID;              /* 只接收标准帧ID */
   hfdcan1_RX_Filter0.FilterIndex = 0;                         /* 滤波器索引0 */
   hfdcan1_RX_Filter0.FilterType = FDCAN_FILTER_MASK;          /* 滤波器类型 */
   //hfdcan1_RX_Filter0.FilterType = FDCAN_FILTER_RANGE;       /* 滤波器类型(允许接收报文的ID范围是FilterID1至FilterID2 */
   
   /* 注意：
    *  1.FDCAN_HandleTypeDef对象句柄的成员RxFifo0ElmtsNbr设置大于0时，表示启用RXFIFO0.
    *  2.
    *  3.如果启用RXFIFO0,那么滤波器必须关联到RXFIFO0,即FilterConfig必须赋值FDCAN_FILTER_TO_RXFIFO0.
    *  4.同理,如果启动RXFIFO1的话,滤波器必须关联到RXFIFO1,即FilterConfig必须赋值FDCAN_FILTER_TO_RXFIFO1.
    *
    */
   hfdcan1_RX_Filter0.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;  /* 滤波器关联到RXFIFO0 */
   hfdcan1_RX_Filter0.FilterID1 = 0x00;                        /* 滤波ID1: 0x00 */
   hfdcan1_RX_Filter0.FilterID2 = 0x00;                        /* 滤波ID2: 0x00 */
	
   /* 看看滤波器有没有创建成功 */
   if(HAL_FDCAN_ConfigFilter(&hfdcan1,&hfdcan1_RX_Filter0) != HAL_OK)
   {
        user_Assert(__FILE__,__LINE__);
   }

   /* HAL_FDCAN_ConfigGlobalFilter()
    * 参数2：设置标准帧ID，接收的报文ID没有匹配上滤波器时，选择拒绝接收(没有匹配上时,可以选择放入FIFO0或者FIFO1)。
    * 参数3：设置拓展帧ID，接收的报文ID没有匹配上滤波器时，选择拒绝接收。
    * 参数4：设置是否拒绝远程标准帧，ENABLE代表拒绝接收。
    * 参数5：设置是否拒绝远程拓展帧，ENABLE代表拒绝接收。
    */
   if(HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,FDCAN_REJECT,FDCAN_REJECT,DISABLE,DISABLE) != HAL_OK) /* 设置FDCAN1滤波器0全局配置  */
   {
	    user_Assert(__FILE__,__LINE__);
   }
}

/**
  * 函数功能: 设置并初始化滤波器1,供FDCAN2使用
  * 输入参数: void
  *           
  * 返回值：  void
  *           
  *
  * 说明:
  *     1. 通过将成员FilterType设为FDCAN_FILTER_MASK,再加上成员FilterID1与FilterID2设为0x00,实现接收所有CAN ID.
  *     2. FDCAN2只设置了使用一个滤波器，所以成员FilterIndex设0，因为是从0开始的。
  *     3. FDCAN2设置了使用FIFO1存放CAN报文，所以滤波器需要关联到FIFO1.
  */
void FDCAN2_RX_Filter0_Init(void)
{
   hfdcan2_RX_Filter0.IdType = FDCAN_STANDARD_ID;              /* 只接收标准帧ID */
   hfdcan2_RX_Filter0.FilterIndex = 0;                         /* 滤波器索引0 */
   hfdcan2_RX_Filter0.FilterType = FDCAN_FILTER_MASK;          /* 滤波器类型(允许接收报文的ID范围是FilterID1至FilterID2 */
   
   /* 注意：
    *  1.FDCAN_HandleTypeDef对象句柄的成员RxFifo1ElmtsNbr设置大于0时，表示启用RXFIFO1.
    *  2.
    *  3.如果启用RXFIFO0,那么滤波器必须关联到RXFIFO0,即FilterConfig必须赋值FDCAN_FILTER_TO_RXFIFO0.
    *  4.同理,如果启动RXFIFO1的话,滤波器必须关联到RXFIFO1,即FilterConfig必须赋值FDCAN_FILTER_TO_RXFIFO1.
    *
    */
   hfdcan2_RX_Filter0.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;  /* 滤波器关联到RXFIFO1 */
   hfdcan2_RX_Filter0.FilterID1 = 0x00;                        /* 滤波ID1: 0x00 */
   hfdcan2_RX_Filter0.FilterID2 = 0x00;                        /* 滤波ID2:   0x00 */
   
   /* 看看滤波器有没有创建成功 */
   if(HAL_FDCAN_ConfigFilter(&hfdcan2,&hfdcan2_RX_Filter0) != HAL_OK)
   {
	   /* 设置滤波器不成的话，进入断言 */
	   user_Assert(__FILE__,__LINE__);
   }
   /* HAL_FDCAN_ConfigGlobalFilter()
    * 参数2：设置标准帧ID，接收的报文ID没有匹配上滤波器时，选择拒绝接收(没有匹配上时,可以选择放入FIFO0或者FIFO1)。
    * 参数3：设置拓展帧ID，接收的报文ID没有匹配上滤波器时，选择拒绝接收。
    * 参数4：设置是否拒绝远程标准帧，ENABLE代表拒绝接收。
    * 参数5：设置是否拒绝远程拓展帧，ENABLE代表拒绝接收。
    */
   if(HAL_FDCAN_ConfigGlobalFilter(&hfdcan2,FDCAN_REJECT,FDCAN_REJECT,DISABLE,DISABLE) != HAL_OK) /* 设置FDCAN1滤波器1全局配置  */
   {
	   user_Assert(__FILE__,__LINE__);
   }
}

/**
  * 函数功能: 设置FDCAN1的中断
  * 输入参数: void
  *           
  * 返回值：  void
  *           
  *
  * 说明:
  *     1. 设置收到新的数据就产生中断。
  *     2. 配置水印中断防止FIFO溢出。
  *     3. 配置FIFO溢出中断，防止FIFO溢出导致丢失报文。
  */
void FDCAN1_Interrupt_Init(void)
{
   /*  在函数MX_FDCAN1_Init,将FDCAN1设置使用RXFIFO0(没有使用RXFIFO1),所以中断也需要判断RXFIFO0是不是有新数据 */
   if(HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0) != HAL_OK) /* 使能FDCAN中断(接收到新数据) */
   {
	   user_Assert(__FILE__,__LINE__);
   }

   if(HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_RX_FIFO0_FULL,0) != HAL_OK) /* 使能FDCAN中断(FIFO0挤满)  */
   {
	   user_Assert(__FILE__,__LINE__);
   }

   if(HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_RX_FIFO0_WATERMARK,0) != HAL_OK) /* 使能FDCAN中断(水印中断)  */
   {
	   user_Assert(__FILE__,__LINE__);
   }
}

/**
  * 函数功能: 设置FDCAN2的中断(FIFO1接收到新的报文立刻中断)
  * 输入参数: void
  *           
  * 返回值：  void
  *           
  *
  * 说明:
  *     1. 必须配置水印才能产生中断
  */
void FDCAN2_Interrupt_Init(void)
{
    /*  在函数MX_FDCAN1_Init,将FDCAN1设置使用RXFIFO0(没有使用RXFIFO1),所以中断也需要判断RXFIFO0是不是有新数据 */
    if(HAL_FDCAN_ActivateNotification(&hfdcan2,FDCAN_IT_RX_FIFO1_NEW_MESSAGE,0) != HAL_OK) /* 使能FDCAN中断(接收到新数据) */
    {
    	user_Assert(__FILE__,__LINE__);
    }
    if(HAL_FDCAN_ActivateNotification(&hfdcan2,FDCAN_IT_RX_FIFO1_WATERMARK,0) != HAL_OK) /* 使能FDCAN中断(接收到新数据) */
    {
    	user_Assert(__FILE__,__LINE__);
    }
    if(HAL_FDCAN_ActivateNotification(&hfdcan2,FDCAN_IT_RX_FIFO1_FULL,0) != HAL_OK) /* 使能FDCAN中断(接收到新数据) */
    {
    	user_Assert(__FILE__,__LINE__);
    }
}

/**
  * 函数功能: 更新FDCAN接收FIFO的状态寄存器
  * 输入参数:  FDCAN_HandleTypeDef *hfdcan
  *         struct fdcan_handler *fdcan
  * 返回值：  void
  *
  *
  * 说明:
  *     1. FDCAN控制器必须配置RX FIFO后才能使用
  */
void update_RXFIFO_Status(FDCAN_HandleTypeDef *hfdcan,struct fdcan_handler *fdcan)
{
	if(fdcan == &fdcan1)
    {
		fdcan->RXFxS = hfdcan->Instance->RXF0S;  /* 获取RXF0S */
    }

	if(fdcan == &fdcan2)
	{
		fdcan->RXFxS = hfdcan->Instance->RXF1S;  /* 获取RXF1S */
	}
    /* 获取F0GI,F0PI,F0FL */
    fdcan->FxGI = (fdcan->RXFxS >> 8) & 0x1F;
    fdcan->FxPI = (fdcan->RXFxS >> 16) & 0x1F;
    fdcan->FxFL = fdcan->RXFxS & 0x3F;
}


/**
  * 函数功能:      使用FDCAN1将数据发送出去
  * 输入参数:      CAN_ID :     发送报文的ID
  *            msg_length : 发送数据的长度(0 - 8)
  *            *msg :       发送数据的指针(uint8_t *)
  *            
  * 返回值: uint8_t
  *           
  *
  * 说明:
  *     1.这个函数只允许报文的长度是0 ~ 8,不支持大于8的报文(FDCAN模式)
  *      
  */
uint8_t FDCAN1_Send_Msg(_CANMSG *msg)
{
    /* 选择数据的长度 */
    switch (msg->len)
    {
        case 0:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_0;    /* 数据长度:0 */
            break;
        case 1:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_1;    /* 数据长度:1 */
            break;
        case 2:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_2;    /* 数据长度:2 */
            break;
        case 3:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_3;    /* 数据长度:3 */
            break;
        case 4:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_4;    /* 数据长度:4 */
            break;
        case 5:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_5;    /* 数据长度:5 */
            break;
        case 6:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_6;    /* 数据长度:6 */
            break;
        case 7:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_7;    /* 数据长度:7 */
            break;
        case 8:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_8;    /* 数据长度:8 */
            break;
        default:
            /* 其他长度就退出函数 */
            return 0;
    }   

    hfdcan1_TX.Identifier = msg->id;                      /* 32位ID */
    hfdcan1_TX.IdType = FDCAN_STANDARD_ID;                /* 标准ID */
    hfdcan1_TX.TxFrameType = FDCAN_DATA_FRAME;            /* 数据帧 */
    hfdcan1_TX.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    hfdcan1_TX.BitRateSwitch = FDCAN_BRS_OFF;             /* 关闭速率转换 */
    hfdcan1_TX.FDFormat = FDCAN_CLASSIC_CAN;              /* 标准CAN模式(非FDCAN模式) */
    hfdcan1_TX.TxEventFifoControl = FDCAN_NO_TX_EVENTS;   /* 无发送事件 */
    hfdcan1_TX.MessageMarker = 0;  

    /* 远程帧 or  数据帧     */
    if(REMOTE_FRAME == msg->rtr )
        hfdcan1_TX.TxFrameType = FDCAN_REMOTE_FRAME;   /* 远程帧 */
    else
        hfdcan1_TX.TxFrameType = FDCAN_DATA_FRAME;     /* 数据帧 */
    
    /* 将需要发送的数据压入到TX FIFO */
    if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1,&hfdcan1_TX,(uint8_t *)msg->buffer) == HAL_OK)
    {
        //printf("FDCAN1 sent data successfully! \n");
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
  * 函数功能:      使用FDCAN2将数据发送出去
  * 输入参数:      CAN_ID :     发送报文的ID
  *            msg_length : 发送数据的长度(0 - 8)
  *            *msg :       发送数据的指针(uint8_t *)
  *            
  * 返回值: uint8_t
  *           
  *
  * 说明:
  *     1.仅支持标准CAN模式，不支持CANFD模式。
  *      
  */
uint8_t FDCAN2_Send_Msg(_CANMSG *msg)
{
    /* 选择数据的长度 */
    switch (msg->len)
    {
        case 0:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_0;    /* 数据长度:0 */
            break;
        case 1:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_1;    /* 数据长度:1 */
            break;
        case 2:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_2;    /* 数据长度:2 */
            break;
        case 3:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_3;    /* 数据长度:3 */
            break;
        case 4:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_4;    /* 数据长度:4 */
            break;
        case 5:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_5;    /* 数据长度:5 */
            break;
        case 6:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_6;    /* 数据长度:6 */
            break;
        case 7:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_7;    /* 数据长度:7 */
            break;
        case 8:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_8;    /* 数据长度:8 */
            break;
        default:
            /* 其他长度就退出函数 */
            return 0;
    }   

    hfdcan2_TX.Identifier = msg->id;                      /* 32位ID */
    hfdcan2_TX.IdType = FDCAN_STANDARD_ID;                /* 标准ID */
    hfdcan2_TX.TxFrameType = FDCAN_DATA_FRAME;            /* 数据帧 */
    hfdcan2_TX.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    hfdcan2_TX.BitRateSwitch = FDCAN_BRS_OFF;             /* 关闭速率转换 */
    hfdcan2_TX.FDFormat = FDCAN_CLASSIC_CAN;              /* 标准CAN模式(非FDCAN模式) */
    hfdcan2_TX.TxEventFifoControl = FDCAN_NO_TX_EVENTS;   /* 无发送事件 */
    hfdcan2_TX.MessageMarker = 0;  

    /* 远程帧 or  数据帧     */
    if(REMOTE_FRAME == msg->rtr )
        hfdcan2_TX.TxFrameType = FDCAN_REMOTE_FRAME;   /* 远程帧 */
    else
        hfdcan2_TX.TxFrameType = FDCAN_DATA_FRAME;     /* 数据帧 */

    /* 将需要发送的数据压入到TX FIFO */
    if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2,&hfdcan2_TX,(uint8_t *)msg->buffer) == HAL_OK)
    {
        //printf("FDCAN2 sent data successfully! \n");
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
  * 函数功能: FDCAN1启动
  * 输入参数: void
  *           
  * 返回值：  void
  *           
  *
  * 说明:
  *    
  *      
  */
void FDCAN1_Start(void)
{
    HAL_FDCAN_Start(&hfdcan1); 
}

/**
  * 函数功能: FDCAN2启动
  * 输入参数: void
  *           
  * 返回值：  void
  *           
  *
  * 说明:
  *    
  *      
  */
void FDCAN2_Start(void)
{
    HAL_FDCAN_Start(&hfdcan2);
}

/**
  * 函数功能: FIFO0的接收中断回调函数
  * 输入参数: RxFifo0ITs：返回标志位
  *
  * 返回值：  void
  *
  *
  * 说明:
  *      1.FDCAN1使用RXFIFO0
  *
  */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{	
    uint8_t rxdata[8] = {0};
    fdcan1.RXFIFO_Inter_Type = RxFifo0ITs;        /* 看看中断的类型 */

    /* 看看rx FIFO0 是什么中断（新数据中断与水印中断与FIFO溢出中断） */
    if(FDCAN_IT_RX_FIFO0_WATERMARK == RxFifo0ITs   ||
       FDCAN_IT_RX_FIFO0_NEW_MESSAGE == RxFifo0ITs ||
	   FDCAN_IT_RX_FIFO0_FULL == RxFifo0ITs)
    {
    	do
    	{
    		HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0,&hfdcan1_RX,rxdata);  /* 提取FIFO0中接收到的数据 */
    		memcpy((uint8_t *)fdcan1.rx_MSG.buffer,rxdata,8);  /* 保存CAN报文的内容 */
    		fdcan1.rx_MSG.id = hfdcan1_RX.Identifier;          /* 保存CAN报文的ID */
    		fdcan1.rx_MSG.len = hfdcan1_RX.DataLength >> 16;   /* 保存CAN报文的内容长度 */

    		/* 判断远程帧或者数据帧 */
    		if(hfdcan1_RX.RxFrameType == FDCAN_REMOTE_FRAME)
    			fdcan1.rx_MSG.rtr = REMOTE_FRAME;
    		else
    			fdcan1.rx_MSG.rtr = DATA_FRAME;

    		fdcan1.update_RXFIFO_Status(hfdcan,&fdcan1);  /* 更新RXFIFO的状态（了解FIFO是否有剩余报文） */

    	}while(fdcan1.FxFL > 0);  /* 循环直到FxFL等于0 */

    }
}


/**
  * 函数功能: FIFO1的接收中断回调函数
  * 输入参数: RxFifo0ITs：返回标志位
  *
  * 返回值：  void
  *
  *
  * 说明:
  *     1、FDCAN2使用RXFIFO1
  *
  */
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
    uint8_t rxdata[8] = {0};
    uint16_t msg_buffer[13] = {0};                /* 加入消息队列的报文长度 */
    fdcan2.RXFIFO_Inter_Type = RxFifo1ITs;        /* 看看中断的类型 */

    /* FIFO1新数据中断 */
    if(FDCAN_IT_RX_FIFO1_WATERMARK == RxFifo1ITs ||
       FDCAN_IT_RX_FIFO1_NEW_MESSAGE == RxFifo1ITs ||
	   FDCAN_IT_RX_FIFO1_FULL == RxFifo1ITs)
    {
    	do
    	{
    		HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO1,&hfdcan2_RX,rxdata);  /* 提取FIFO0中接收到的数据 */
    		memcpy((uint8_t *)fdcan2.rx_MSG.buffer,rxdata,8);  /* 保存CAN报文的内容 */
    		fdcan2.rx_MSG.id = hfdcan2_RX.Identifier;          /* 保存CAN报文的ID */
    		fdcan2.rx_MSG.len = hfdcan2_RX.DataLength >> 16;   /* 保存CAN报文的内容长度 */

    		/* 判断远程帧或者数据帧 */
    		if(hfdcan2_RX.RxFrameType == FDCAN_REMOTE_FRAME)
    			fdcan2.rx_MSG.rtr = REMOTE_FRAME;
    		else
    			fdcan2.rx_MSG.rtr = DATA_FRAME;

    		fdcan2.update_RXFIFO_Status(hfdcan,&fdcan2);  /* 更新RXFIFO的状态(了解FIFO是否有剩余报文） */

    	}while(fdcan2.FxFL > 0);  /* 循环直到FxFL等于0 */

    }
}

HAL_StatusTypeDef CAN_Send_StdDataFrame(FDCAN_HandleTypeDef *hfdcan, uint32_t Identifier, uint8_t *msg) {
    FDCAN_TxHeaderTypeDef CAN_Tx = {
            .Identifier = Identifier,                 //标准标识符
            .IdType = FDCAN_STANDARD_ID,              //使用标准帧
            .TxFrameType = FDCAN_DATA_FRAME,          //数据帧
            .DataLength = FDCAN_DLC_BYTES_8,
            .ErrorStateIndicator = FDCAN_ESI_PASSIVE,
            .FDFormat = FDCAN_CLASSIC_CAN,
            .BitRateSwitch = FDCAN_BRS_OFF,
            .TxEventFifoControl=FDCAN_NO_TX_EVENTS,
            .MessageMarker=0
            };
    HAL_FDCAN_ConfigTxDelayCompensation(&hfdcan1, hfdcan1.Init.DataPrescaler * hfdcan1.Init.DataTimeSeg1, 0);
    HAL_FDCAN_EnableTxDelayCompensation(&hfdcan1);

    HAL_StatusTypeDef err = HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &CAN_Tx, msg);
    return err;
}


