#include "bxCAN.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_can.h"
#include "can.h"
#include "gpio.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#define CAN_FILTER_NUM 28U
#define BOOST 2
#define TRUE 1
#define CAN_CONTROL_ID 0x301

userCommand_t commandInput;
CANRxFrame* const rxmsg;
userCommand_t* returnCommand(void) {

	return &commandInput;

}

void canProcessControl(void)
{
	if (hcan.pRxMsg->Data[0] <= BOOST)
	{
  commandInput.pathType = hcan.pRxMsg->Data[0];
  commandInput.chassis_max_power = hcan.pRxMsg->Data[1];
	commandInput.ChassisPower = *(float *)(&hcan.pRxMsg->Data[4]);
	HAL_CAN_Receive(&hcan,CAN_FIFO0,10);
	}

}

static void canRxCallback( CANRxFrame* const rxmsg)
{
  switch(rxmsg->SID)
  {
      case CAN_CONTROL_ID:
    	  canProcessControl();
        break;
  }
}

void can_processInit(void *pvParameters){
	int j = j+1;
	CAN_FilterConfTypeDef  can_filter[CAN_FILTER_NUM];
//  static CanTxMsgTypeDef Tx1Message;
//  static CanRxMsgTypeDef Rx1Message;
	uint8_t i;
  for (i = 0; i < CAN_FILTER_NUM; i++)
  {
    can_filter[i].FilterNumber = i;
    can_filter[i].FilterMode = 0; //CAN_FilterMode_IdMask
    can_filter[i].FilterScale = 1; //CAN_FilterScale_32bit
    can_filter[i].FilterFIFOAssignment = 0;
		can_filter[i].FilterIdHigh         = 0x0000;
		can_filter[i].FilterIdLow          = 0x0000;
		can_filter[i].FilterMaskIdHigh     = 0x0000;
		can_filter[i].FilterMaskIdLow      = 0x0000;
    can_filter[i].FilterActivation = ENABLE;
		can_filter[i].FilterFIFOAssignment = CAN_FilterFIFO0;
  }
  HAL_CAN_ConfigFilter(&hcan, &can_filter[CAN_FILTER_NUM]);
//	hcan.pTxMsg = &Tx1Message;
//  hcan.pRxMsg = &Rx1Message;
	delay_ms(200);
	void can_receive_start(void);
	
}
void can_receive_start(void){
	
	HAL_CAN_Receive_IT(&hcan, CAN_FIFO0);
//	HAL_CAN_IRQHandler(&hcan);
}
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
    u16 i=0;
    hcan.pTxMsg->StdId=0X12;        //标准标识符
    hcan.pTxMsg->ExtId=0x12;        //扩展标识符(29位)
    hcan.pTxMsg->IDE=CAN_ID_STD;    //使用标准帧
    hcan.pTxMsg->RTR=CAN_RTR_DATA;  //数据帧
    hcan.pTxMsg->DLC=len;                
    for(i=0;i<len;i++)
    hcan.pTxMsg->Data[i]=msg[i];
    if(HAL_CAN_Transmit(&hcan,10)!=HAL_OK) return 1;     //发送
    return 0;		
}
u16 CAN_Send_Msg(u16* msg,u16 len)
{	
    u16 i=0;
    hcan.pTxMsg->StdId=0X12;        //标准标识符
    hcan.pTxMsg->ExtId=0x12;        //扩展标识符(29位)
    hcan.pTxMsg->IDE=CAN_ID_STD;    //使用标准帧
    hcan.pTxMsg->RTR=CAN_RTR_DATA;  //数据帧
    hcan.pTxMsg->DLC=len;                
    for(i=0;i<len;i++)
    hcan.pTxMsg->Data[i]=msg[i];
    if(HAL_CAN_Transmit(&hcan,10)!=HAL_OK) return 1;     //发送
    return 0;		
}
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan){
	if(hcan->Instance == CAN1)
	{
		canRxCallback(rxmsg);
	}
}
