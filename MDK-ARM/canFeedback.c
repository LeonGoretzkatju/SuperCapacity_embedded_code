#include "stm32f1xx_hal.h"
#include "bxCAN.h"
#include <string.h>
#include "canFeedback.h"
#include "pathControl.h"
#include "sys.h"
#include "task.h"
#define true 1
pathData_t* pathData1;
canFeedback_t feedbackData;
canFeedback_t1 feedbackData1;
void canFeedbackThd(void *pvParameters){
	  CANTxFrame txmsg;
		CANTxFrame txmsg1;
  while(true) {

//    now = chVTGetSystemTime();
//    next = now + US2ST(1000);

//    chSysLock();

//    feedbackData1.Pchassis = (uint16_t) pathData1->outPower;
//    feedbackData1.Pjudge = (uint16_t) pathData1->judgePower;
		feedbackData1.mode_type = (uint8_t)pathData1->mode;
    feedbackData.Vin = (uint8_t)pathData1->voltage->vinMv / 1000;
    feedbackData.capEnergy = (uint16_t) pathData1->capEnergy;
    feedbackData.pathType = (uint8_t) pathData1->path;

    memcpy(&txmsg.data4[0], &feedbackData.Vin, sizeof(feedbackData.Vin));
		memcpy(&txmsg.data4[1], &feedbackData.pathType, sizeof(feedbackData.pathType));
		memcpy(&txmsg.data2[0], &feedbackData.capEnergy, sizeof(feedbackData.capEnergy));
		memcpy(&txmsg1.data4[0], &feedbackData1, sizeof(feedbackData1));

//   canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, MS2ST(100));
//		CAN1_Send_Msg(txmsg,12);
//		CAN1_Send_Msg(txmsg1,12);
//int i = 0;
////		for (i = 0;i < 2;i++){
////			CAN_Send_Msg(&txmsg1.data2[i],16);
////		}
////		  CAN1_Send_Msg(&txmsg1.data4[0],8);
////			CAN_Send_Msg(&txmsg1.);
//		for (i = 0;i <2; i++){
//				CAN1_Send_Msg(&txmsg.data4[i],8);
//		}
//		CAN_Send_Msg(&txmsg.data2[0],16);
		while(1)
		{
			int i = 0;
			for (i = 0;i <2; i++){
				CAN1_Send_Msg(&txmsg.data4[i],8);
			}
			CAN_Send_Msg(&txmsg.data2[0],16);
			CAN1_Send_Msg(&txmsg1.data4[0],8);
			vTaskDelay(500);
		}
  }

}
void canFeedbackInit(void) {

  pathData1 = returnPathData();
	void canFeedbackThd(void *pvParameters);
}
