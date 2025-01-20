/* ͨ��STM32CubeMX��ʼ��FDCAN���ƺ󣬻�Ҫ��һ�����ú�FDCAN�����������С�
 * ��ʼ���˲��� -> ��ʼ���ж� -> ����FDCAN����
 */
#include "bsp_fdcan.h"

/* �ⲿ���� */
extern FDCAN_HandleTypeDef hfdcan1;       			   /* FDCAN1�Ķ����� */
extern FDCAN_HandleTypeDef hfdcan2;                /* FDCAN2�Ķ����� */


/* �ڲ����� */
static FDCAN_FilterTypeDef hfdcan1_RX_Filter0;   /* FDCAN1�˲���0������ */
static FDCAN_FilterTypeDef hfdcan2_RX_Filter0;   /* FDCAN2�˲���0������ */

/* ȫ�ֱ��� */
FDCAN_RxHeaderTypeDef hfdcan1_RX;         /* FDCAN1���մ���Ԫ�Ķ����� */
FDCAN_RxHeaderTypeDef hfdcan2_RX;         /* FDCAN2���մ���Ԫ�Ķ����� */
FDCAN_TxHeaderTypeDef hfdcan1_TX;         /* FDCAN1���ʹ���Ԫ�Ķ����� */
FDCAN_TxHeaderTypeDef hfdcan2_TX;         /* FDCAN2���ʹ���Ԫ�Ķ����� */

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
  * ��������: ���ò���ʼ���˲���0,��FDCAN1ʹ��
  * �������: void
  *           
  * ����ֵ��  void
  *           
  *
  * ˵��:
  *     1. ͨ������ԱFilterType��ΪFDCAN_FILTER_MASK,�ټ��ϳ�ԱFilterID1��FilterID2��Ϊ0x00,ʵ�ֽ�������CAN ID.
  *     2. FDCAN1ֻ������ʹ��һ���˲��������Գ�ԱFilterIndex��0����Ϊ�Ǵ�0��ʼ�ġ�
  *     3. FDCAN1������ʹ��FIFO0���CAN���ģ������˲�����Ҫ������FIFO0.
  */
void FDCAN1_RX_Filter0_Init(void)
{
   hfdcan1_RX_Filter0.IdType = FDCAN_STANDARD_ID;              /* ֻ���ձ�׼֡ID */
   hfdcan1_RX_Filter0.FilterIndex = 0;                         /* �˲�������0 */
   hfdcan1_RX_Filter0.FilterType = FDCAN_FILTER_MASK;          /* �˲������� */
   //hfdcan1_RX_Filter0.FilterType = FDCAN_FILTER_RANGE;       /* �˲�������(������ձ��ĵ�ID��Χ��FilterID1��FilterID2 */
   
   /* ע�⣺
    *  1.FDCAN_HandleTypeDef�������ĳ�ԱRxFifo0ElmtsNbr���ô���0ʱ����ʾ����RXFIFO0.
    *  2.
    *  3.�������RXFIFO0,��ô�˲������������RXFIFO0,��FilterConfig���븳ֵFDCAN_FILTER_TO_RXFIFO0.
    *  4.ͬ��,�������RXFIFO1�Ļ�,�˲������������RXFIFO1,��FilterConfig���븳ֵFDCAN_FILTER_TO_RXFIFO1.
    *
    */
   hfdcan1_RX_Filter0.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;  /* �˲���������RXFIFO0 */
   hfdcan1_RX_Filter0.FilterID1 = 0x00;                        /* �˲�ID1: 0x00 */
   hfdcan1_RX_Filter0.FilterID2 = 0x00;                        /* �˲�ID2: 0x00 */
	
   /* �����˲�����û�д����ɹ� */
   if(HAL_FDCAN_ConfigFilter(&hfdcan1,&hfdcan1_RX_Filter0) != HAL_OK)
   {
        user_Assert(__FILE__,__LINE__);
   }

   /* HAL_FDCAN_ConfigGlobalFilter()
    * ����2�����ñ�׼֡ID�����յı���IDû��ƥ�����˲���ʱ��ѡ��ܾ�����(û��ƥ����ʱ,����ѡ�����FIFO0����FIFO1)��
    * ����3��������չ֡ID�����յı���IDû��ƥ�����˲���ʱ��ѡ��ܾ����ա�
    * ����4�������Ƿ�ܾ�Զ�̱�׼֡��ENABLE����ܾ����ա�
    * ����5�������Ƿ�ܾ�Զ����չ֡��ENABLE����ܾ����ա�
    */
   if(HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,FDCAN_REJECT,FDCAN_REJECT,DISABLE,DISABLE) != HAL_OK) /* ����FDCAN1�˲���0ȫ������  */
   {
	    user_Assert(__FILE__,__LINE__);
   }
}

/**
  * ��������: ���ò���ʼ���˲���1,��FDCAN2ʹ��
  * �������: void
  *           
  * ����ֵ��  void
  *           
  *
  * ˵��:
  *     1. ͨ������ԱFilterType��ΪFDCAN_FILTER_MASK,�ټ��ϳ�ԱFilterID1��FilterID2��Ϊ0x00,ʵ�ֽ�������CAN ID.
  *     2. FDCAN2ֻ������ʹ��һ���˲��������Գ�ԱFilterIndex��0����Ϊ�Ǵ�0��ʼ�ġ�
  *     3. FDCAN2������ʹ��FIFO1���CAN���ģ������˲�����Ҫ������FIFO1.
  */
void FDCAN2_RX_Filter0_Init(void)
{
   hfdcan2_RX_Filter0.IdType = FDCAN_STANDARD_ID;              /* ֻ���ձ�׼֡ID */
   hfdcan2_RX_Filter0.FilterIndex = 0;                         /* �˲�������0 */
   hfdcan2_RX_Filter0.FilterType = FDCAN_FILTER_MASK;          /* �˲�������(������ձ��ĵ�ID��Χ��FilterID1��FilterID2 */
   
   /* ע�⣺
    *  1.FDCAN_HandleTypeDef�������ĳ�ԱRxFifo1ElmtsNbr���ô���0ʱ����ʾ����RXFIFO1.
    *  2.
    *  3.�������RXFIFO0,��ô�˲������������RXFIFO0,��FilterConfig���븳ֵFDCAN_FILTER_TO_RXFIFO0.
    *  4.ͬ��,�������RXFIFO1�Ļ�,�˲������������RXFIFO1,��FilterConfig���븳ֵFDCAN_FILTER_TO_RXFIFO1.
    *
    */
   hfdcan2_RX_Filter0.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;  /* �˲���������RXFIFO1 */
   hfdcan2_RX_Filter0.FilterID1 = 0x00;                        /* �˲�ID1: 0x00 */
   hfdcan2_RX_Filter0.FilterID2 = 0x00;                        /* �˲�ID2:   0x00 */
   
   /* �����˲�����û�д����ɹ� */
   if(HAL_FDCAN_ConfigFilter(&hfdcan2,&hfdcan2_RX_Filter0) != HAL_OK)
   {
	   /* �����˲������ɵĻ���������� */
	   user_Assert(__FILE__,__LINE__);
   }
   /* HAL_FDCAN_ConfigGlobalFilter()
    * ����2�����ñ�׼֡ID�����յı���IDû��ƥ�����˲���ʱ��ѡ��ܾ�����(û��ƥ����ʱ,����ѡ�����FIFO0����FIFO1)��
    * ����3��������չ֡ID�����յı���IDû��ƥ�����˲���ʱ��ѡ��ܾ����ա�
    * ����4�������Ƿ�ܾ�Զ�̱�׼֡��ENABLE����ܾ����ա�
    * ����5�������Ƿ�ܾ�Զ����չ֡��ENABLE����ܾ����ա�
    */
   if(HAL_FDCAN_ConfigGlobalFilter(&hfdcan2,FDCAN_REJECT,FDCAN_REJECT,DISABLE,DISABLE) != HAL_OK) /* ����FDCAN1�˲���1ȫ������  */
   {
	   user_Assert(__FILE__,__LINE__);
   }
}

/**
  * ��������: ����FDCAN1���ж�
  * �������: void
  *           
  * ����ֵ��  void
  *           
  *
  * ˵��:
  *     1. �����յ��µ����ݾͲ����жϡ�
  *     2. ����ˮӡ�жϷ�ֹFIFO�����
  *     3. ����FIFO����жϣ���ֹFIFO������¶�ʧ���ġ�
  */
void FDCAN1_Interrupt_Init(void)
{
   /*  �ں���MX_FDCAN1_Init,��FDCAN1����ʹ��RXFIFO0(û��ʹ��RXFIFO1),�����ж�Ҳ��Ҫ�ж�RXFIFO0�ǲ����������� */
   if(HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0) != HAL_OK) /* ʹ��FDCAN�ж�(���յ�������) */
   {
	   user_Assert(__FILE__,__LINE__);
   }

   if(HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_RX_FIFO0_FULL,0) != HAL_OK) /* ʹ��FDCAN�ж�(FIFO0����)  */
   {
	   user_Assert(__FILE__,__LINE__);
   }

   if(HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_RX_FIFO0_WATERMARK,0) != HAL_OK) /* ʹ��FDCAN�ж�(ˮӡ�ж�)  */
   {
	   user_Assert(__FILE__,__LINE__);
   }
}

/**
  * ��������: ����FDCAN2���ж�(FIFO1���յ��µı��������ж�)
  * �������: void
  *           
  * ����ֵ��  void
  *           
  *
  * ˵��:
  *     1. ��������ˮӡ���ܲ����ж�
  */
void FDCAN2_Interrupt_Init(void)
{
    /*  �ں���MX_FDCAN1_Init,��FDCAN1����ʹ��RXFIFO0(û��ʹ��RXFIFO1),�����ж�Ҳ��Ҫ�ж�RXFIFO0�ǲ����������� */
    if(HAL_FDCAN_ActivateNotification(&hfdcan2,FDCAN_IT_RX_FIFO1_NEW_MESSAGE,0) != HAL_OK) /* ʹ��FDCAN�ж�(���յ�������) */
    {
    	user_Assert(__FILE__,__LINE__);
    }
    if(HAL_FDCAN_ActivateNotification(&hfdcan2,FDCAN_IT_RX_FIFO1_WATERMARK,0) != HAL_OK) /* ʹ��FDCAN�ж�(���յ�������) */
    {
    	user_Assert(__FILE__,__LINE__);
    }
    if(HAL_FDCAN_ActivateNotification(&hfdcan2,FDCAN_IT_RX_FIFO1_FULL,0) != HAL_OK) /* ʹ��FDCAN�ж�(���յ�������) */
    {
    	user_Assert(__FILE__,__LINE__);
    }
}

/**
  * ��������: ����FDCAN����FIFO��״̬�Ĵ���
  * �������:  FDCAN_HandleTypeDef *hfdcan
  *         struct fdcan_handler *fdcan
  * ����ֵ��  void
  *
  *
  * ˵��:
  *     1. FDCAN��������������RX FIFO�����ʹ��
  */
void update_RXFIFO_Status(FDCAN_HandleTypeDef *hfdcan,struct fdcan_handler *fdcan)
{
	if(fdcan == &fdcan1)
    {
		fdcan->RXFxS = hfdcan->Instance->RXF0S;  /* ��ȡRXF0S */
    }

	if(fdcan == &fdcan2)
	{
		fdcan->RXFxS = hfdcan->Instance->RXF1S;  /* ��ȡRXF1S */
	}
    /* ��ȡF0GI,F0PI,F0FL */
    fdcan->FxGI = (fdcan->RXFxS >> 8) & 0x1F;
    fdcan->FxPI = (fdcan->RXFxS >> 16) & 0x1F;
    fdcan->FxFL = fdcan->RXFxS & 0x3F;
}


/**
  * ��������:      ʹ��FDCAN1�����ݷ��ͳ�ȥ
  * �������:      CAN_ID :     ���ͱ��ĵ�ID
  *            msg_length : �������ݵĳ���(0 - 8)
  *            *msg :       �������ݵ�ָ��(uint8_t *)
  *            
  * ����ֵ: uint8_t
  *           
  *
  * ˵��:
  *     1.�������ֻ�����ĵĳ�����0 ~ 8,��֧�ִ���8�ı���(FDCANģʽ)
  *      
  */
uint8_t FDCAN1_Send_Msg(_CANMSG *msg)
{
    /* ѡ�����ݵĳ��� */
    switch (msg->len)
    {
        case 0:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_0;    /* ���ݳ���:0 */
            break;
        case 1:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_1;    /* ���ݳ���:1 */
            break;
        case 2:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_2;    /* ���ݳ���:2 */
            break;
        case 3:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_3;    /* ���ݳ���:3 */
            break;
        case 4:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_4;    /* ���ݳ���:4 */
            break;
        case 5:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_5;    /* ���ݳ���:5 */
            break;
        case 6:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_6;    /* ���ݳ���:6 */
            break;
        case 7:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_7;    /* ���ݳ���:7 */
            break;
        case 8:
            hfdcan1_TX.DataLength = FDCAN_DLC_BYTES_8;    /* ���ݳ���:8 */
            break;
        default:
            /* �������Ⱦ��˳����� */
            return 0;
    }   

    hfdcan1_TX.Identifier = msg->id;                      /* 32λID */
    hfdcan1_TX.IdType = FDCAN_STANDARD_ID;                /* ��׼ID */
    hfdcan1_TX.TxFrameType = FDCAN_DATA_FRAME;            /* ����֡ */
    hfdcan1_TX.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    hfdcan1_TX.BitRateSwitch = FDCAN_BRS_OFF;             /* �ر�����ת�� */
    hfdcan1_TX.FDFormat = FDCAN_CLASSIC_CAN;              /* ��׼CANģʽ(��FDCANģʽ) */
    hfdcan1_TX.TxEventFifoControl = FDCAN_NO_TX_EVENTS;   /* �޷����¼� */
    hfdcan1_TX.MessageMarker = 0;  

    /* Զ��֡ or  ����֡     */
    if(REMOTE_FRAME == msg->rtr )
        hfdcan1_TX.TxFrameType = FDCAN_REMOTE_FRAME;   /* Զ��֡ */
    else
        hfdcan1_TX.TxFrameType = FDCAN_DATA_FRAME;     /* ����֡ */
    
    /* ����Ҫ���͵�����ѹ�뵽TX FIFO */
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
  * ��������:      ʹ��FDCAN2�����ݷ��ͳ�ȥ
  * �������:      CAN_ID :     ���ͱ��ĵ�ID
  *            msg_length : �������ݵĳ���(0 - 8)
  *            *msg :       �������ݵ�ָ��(uint8_t *)
  *            
  * ����ֵ: uint8_t
  *           
  *
  * ˵��:
  *     1.��֧�ֱ�׼CANģʽ����֧��CANFDģʽ��
  *      
  */
uint8_t FDCAN2_Send_Msg(_CANMSG *msg)
{
    /* ѡ�����ݵĳ��� */
    switch (msg->len)
    {
        case 0:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_0;    /* ���ݳ���:0 */
            break;
        case 1:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_1;    /* ���ݳ���:1 */
            break;
        case 2:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_2;    /* ���ݳ���:2 */
            break;
        case 3:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_3;    /* ���ݳ���:3 */
            break;
        case 4:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_4;    /* ���ݳ���:4 */
            break;
        case 5:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_5;    /* ���ݳ���:5 */
            break;
        case 6:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_6;    /* ���ݳ���:6 */
            break;
        case 7:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_7;    /* ���ݳ���:7 */
            break;
        case 8:
            hfdcan2_TX.DataLength = FDCAN_DLC_BYTES_8;    /* ���ݳ���:8 */
            break;
        default:
            /* �������Ⱦ��˳����� */
            return 0;
    }   

    hfdcan2_TX.Identifier = msg->id;                      /* 32λID */
    hfdcan2_TX.IdType = FDCAN_STANDARD_ID;                /* ��׼ID */
    hfdcan2_TX.TxFrameType = FDCAN_DATA_FRAME;            /* ����֡ */
    hfdcan2_TX.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    hfdcan2_TX.BitRateSwitch = FDCAN_BRS_OFF;             /* �ر�����ת�� */
    hfdcan2_TX.FDFormat = FDCAN_CLASSIC_CAN;              /* ��׼CANģʽ(��FDCANģʽ) */
    hfdcan2_TX.TxEventFifoControl = FDCAN_NO_TX_EVENTS;   /* �޷����¼� */
    hfdcan2_TX.MessageMarker = 0;  

    /* Զ��֡ or  ����֡     */
    if(REMOTE_FRAME == msg->rtr )
        hfdcan2_TX.TxFrameType = FDCAN_REMOTE_FRAME;   /* Զ��֡ */
    else
        hfdcan2_TX.TxFrameType = FDCAN_DATA_FRAME;     /* ����֡ */

    /* ����Ҫ���͵�����ѹ�뵽TX FIFO */
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
  * ��������: FDCAN1����
  * �������: void
  *           
  * ����ֵ��  void
  *           
  *
  * ˵��:
  *    
  *      
  */
void FDCAN1_Start(void)
{
    HAL_FDCAN_Start(&hfdcan1); 
}

/**
  * ��������: FDCAN2����
  * �������: void
  *           
  * ����ֵ��  void
  *           
  *
  * ˵��:
  *    
  *      
  */
void FDCAN2_Start(void)
{
    HAL_FDCAN_Start(&hfdcan2);
}

/**
  * ��������: FIFO0�Ľ����жϻص�����
  * �������: RxFifo0ITs�����ر�־λ
  *
  * ����ֵ��  void
  *
  *
  * ˵��:
  *      1.FDCAN1ʹ��RXFIFO0
  *
  */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{	
    uint8_t rxdata[8] = {0};
    fdcan1.RXFIFO_Inter_Type = RxFifo0ITs;        /* �����жϵ����� */

    /* ����rx FIFO0 ��ʲô�жϣ��������ж���ˮӡ�ж���FIFO����жϣ� */
    if(FDCAN_IT_RX_FIFO0_WATERMARK == RxFifo0ITs   ||
       FDCAN_IT_RX_FIFO0_NEW_MESSAGE == RxFifo0ITs ||
	   FDCAN_IT_RX_FIFO0_FULL == RxFifo0ITs)
    {
    	do
    	{
    		HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0,&hfdcan1_RX,rxdata);  /* ��ȡFIFO0�н��յ������� */
    		memcpy((uint8_t *)fdcan1.rx_MSG.buffer,rxdata,8);  /* ����CAN���ĵ����� */
    		fdcan1.rx_MSG.id = hfdcan1_RX.Identifier;          /* ����CAN���ĵ�ID */
    		fdcan1.rx_MSG.len = hfdcan1_RX.DataLength >> 16;   /* ����CAN���ĵ����ݳ��� */

    		/* �ж�Զ��֡��������֡ */
    		if(hfdcan1_RX.RxFrameType == FDCAN_REMOTE_FRAME)
    			fdcan1.rx_MSG.rtr = REMOTE_FRAME;
    		else
    			fdcan1.rx_MSG.rtr = DATA_FRAME;

    		fdcan1.update_RXFIFO_Status(hfdcan,&fdcan1);  /* ����RXFIFO��״̬���˽�FIFO�Ƿ���ʣ�౨�ģ� */

    	}while(fdcan1.FxFL > 0);  /* ѭ��ֱ��FxFL����0 */

    }
}


/**
  * ��������: FIFO1�Ľ����жϻص�����
  * �������: RxFifo0ITs�����ر�־λ
  *
  * ����ֵ��  void
  *
  *
  * ˵��:
  *     1��FDCAN2ʹ��RXFIFO1
  *
  */
void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
    uint8_t rxdata[8] = {0};
    uint16_t msg_buffer[13] = {0};                /* ������Ϣ���еı��ĳ��� */
    fdcan2.RXFIFO_Inter_Type = RxFifo1ITs;        /* �����жϵ����� */

    /* FIFO1�������ж� */
    if(FDCAN_IT_RX_FIFO1_WATERMARK == RxFifo1ITs ||
       FDCAN_IT_RX_FIFO1_NEW_MESSAGE == RxFifo1ITs ||
	   FDCAN_IT_RX_FIFO1_FULL == RxFifo1ITs)
    {
    	do
    	{
    		HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO1,&hfdcan2_RX,rxdata);  /* ��ȡFIFO0�н��յ������� */
    		memcpy((uint8_t *)fdcan2.rx_MSG.buffer,rxdata,8);  /* ����CAN���ĵ����� */
    		fdcan2.rx_MSG.id = hfdcan2_RX.Identifier;          /* ����CAN���ĵ�ID */
    		fdcan2.rx_MSG.len = hfdcan2_RX.DataLength >> 16;   /* ����CAN���ĵ����ݳ��� */

    		/* �ж�Զ��֡��������֡ */
    		if(hfdcan2_RX.RxFrameType == FDCAN_REMOTE_FRAME)
    			fdcan2.rx_MSG.rtr = REMOTE_FRAME;
    		else
    			fdcan2.rx_MSG.rtr = DATA_FRAME;

    		fdcan2.update_RXFIFO_Status(hfdcan,&fdcan2);  /* ����RXFIFO��״̬(�˽�FIFO�Ƿ���ʣ�౨�ģ� */

    	}while(fdcan2.FxFL > 0);  /* ѭ��ֱ��FxFL����0 */

    }
}

HAL_StatusTypeDef CAN_Send_StdDataFrame(FDCAN_HandleTypeDef *hfdcan, uint32_t Identifier, uint8_t *msg) {
    FDCAN_TxHeaderTypeDef CAN_Tx = {
            .Identifier = Identifier,                 //��׼��ʶ��
            .IdType = FDCAN_STANDARD_ID,              //ʹ�ñ�׼֡
            .TxFrameType = FDCAN_DATA_FRAME,          //����֡
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


