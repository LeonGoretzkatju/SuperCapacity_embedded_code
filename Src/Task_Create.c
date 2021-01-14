#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "main.h"


#define STK_SIZE_128 128 
#define STK_SIZE_256 256
#define STK_SIZE_1024 1024
#define START_TASK_PRIO  11  				//�������ȼ�
TaskHandle_t StartTask_Handler; 			//������
void Start_Task(void *pvParameters); 		//������

#define CAN_PROCESSINIT_PRIO  0 				//�������ȼ�
TaskHandle_t can_processInit_Handler; 			//������
void can_processInit(void *pvParameters); 		//������

//#define CAN_RECEIVE_START_PRIO  7  				//�������ȼ�
//TaskHandle_t can_receive_start_Handler; 			//������
//void can_receive_start(void *pvParameters); 		//������

#define PATHMONITORTHD_PRIO 6
TaskHandle_t pathMonitorThd_Handler;
void pathMonitorThd(void *pvParameters);

#define BUCKPROGTHD_PRIO 8
TaskHandle_t BuckProgThd_Handler;
void BuckProgThd(void *pvParameters);

#define VOLTMONTHD_PRIO 8
TaskHandle_t voltMonThd_Handler;
void voltMonThd(void *pvParameters);

#define PATHSWITCHTHD_PRIO 4
TaskHandle_t pathSwitchThd_Handler;
void pathSwitchThd(void *pvParameters);

#define pathCalcThd_PRIO 3
TaskHandle_t pathCalcThd_Handler;
void pathCalcThd(void *pvParameters);

#define pidCalcThd_PRIO 5
TaskHandle_t pidCalcThd_Handler;
void pidCalcThd(void *pvParameters);

#define pathRampThd_PRIO 1
TaskHandle_t pathRampThd_Handler;
void pathRampThd(void *pvParameters);

#define canFeedbackThd_PRIO 6
TaskHandle_t canFeedbackThd_Handler;
void canFeedbackThd(void *pvParameters);

void App_Task_Create(void)
{
	xTaskCreate((TaskFunction_t )Start_Task, 			//������
				(const char* )"Start_Task", 			//��������
				(uint16_t )STK_SIZE_128, 				//�����ջ��С
				(void* )NULL, 							//���ݸ��������Ĳ���
				(UBaseType_t )START_TASK_PRIO, 			//�������ȼ�
				(TaskHandle_t* )&StartTask_Handler); 	//������

}

void Start_Task(void *pvParameters){
	taskENTER_CRITICAL();
	xTaskCreate((TaskFunction_t )can_processInit, 			//������
				(const char* )"can_processInit", 			//��������
				(uint16_t )STK_SIZE_128, 				//�����ջ��С
				(void* )NULL, 							//���ݸ��������Ĳ���
				(UBaseType_t )CAN_PROCESSINIT_PRIO, 			//�������ȼ�
				(TaskHandle_t* )&can_processInit_Handler); 	//������
//	xTaskCreate((TaskFunction_t )can_receive_start, 			//������
//				(const char* )"can_receive_start", 			//��������
//				(uint16_t )STK_SIZE_256, 				//�����ջ��С
//				(void* )NULL, 							//���ݸ��������Ĳ���
//				(UBaseType_t )CAN_RECEIVE_START_PRIO, 			//�������ȼ�
//				(TaskHandle_t* )&can_receive_start_Handler); 	//������
	xTaskCreate((TaskFunction_t )pathMonitorThd, 			//������
				(const char* )"pathMonitorThd", 			//��������
				(uint16_t )STK_SIZE_128, 				//�����ջ��С
				(void* )NULL, 							//���ݸ��������Ĳ���
				(UBaseType_t )PATHMONITORTHD_PRIO, 			//�������ȼ�
				(TaskHandle_t* )&pathMonitorThd_Handler); 	//������
	xTaskCreate((TaskFunction_t )BuckProgThd, 			//������
				(const char* )"BuckProgThd", 			//��������
				(uint16_t )STK_SIZE_128, 				//�����ջ��С
				(void* )NULL, 							//���ݸ��������Ĳ���
				(UBaseType_t )BUCKPROGTHD_PRIO, 			//�������ȼ�
				(TaskHandle_t* )&BuckProgThd_Handler); 	//������
	xTaskCreate((TaskFunction_t )voltMonThd, 			//������
				(const char* )"voltMonThd", 			//��������
				(uint16_t )STK_SIZE_128, 				//�����ջ��С
				(void* )NULL, 							//���ݸ��������Ĳ���
				(UBaseType_t )VOLTMONTHD_PRIO, 			//�������ȼ�
				(TaskHandle_t* )&voltMonThd_Handler); 	//������

	xTaskCreate((TaskFunction_t )pathSwitchThd, 			//������
				(const char* )"pathSwitchThd", 			//��������
				(uint16_t )STK_SIZE_128, 				//�����ջ��С
				(void* )NULL, 							//���ݸ��������Ĳ���
				(UBaseType_t )PATHSWITCHTHD_PRIO, 			//�������ȼ�
				(TaskHandle_t* )&pathSwitchThd_Handler); 	//������
	xTaskCreate((TaskFunction_t )pathCalcThd, 			//������
				(const char* )"pathCalcThd", 			//��������
				(uint16_t )STK_SIZE_128, 				//�����ջ��С
				(void* )NULL, 							//���ݸ��������Ĳ���
				(UBaseType_t )pathCalcThd_PRIO, 			//�������ȼ�
				(TaskHandle_t* )&pathCalcThd_Handler); 	//������
	xTaskCreate((TaskFunction_t )pidCalcThd, 			//������
				(const char* )"pidCalcThd", 			//��������
				(uint16_t )STK_SIZE_128, 				//�����ջ��С
				(void* )NULL, 							//���ݸ��������Ĳ���
				(UBaseType_t )pidCalcThd_PRIO, 			//�������ȼ�
				(TaskHandle_t* )&pidCalcThd_Handler); 	//������
	xTaskCreate((TaskFunction_t )pathRampThd, 			//������
				(const char* )"pathRampThd", 			//��������
				(uint16_t )STK_SIZE_128, 				//�����ջ��С
				(void* )NULL, 							//���ݸ��������Ĳ���
				(UBaseType_t )pathRampThd_PRIO, 			//�������ȼ�
				(TaskHandle_t* )&pathRampThd_Handler); 	//������
	vTaskDelay(500);
	vTaskSuspend(StartTask_Handler); 					//ɾ����ʼ����			

	taskEXIT_CRITICAL(); 								//�˳��ٽ���		
}
