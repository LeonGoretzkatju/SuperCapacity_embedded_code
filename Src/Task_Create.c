#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "main.h"


#define STK_SIZE_128 128 
#define STK_SIZE_256 256
#define STK_SIZE_1024 1024
#define START_TASK_PRIO  11  				//任务优先级
TaskHandle_t StartTask_Handler; 			//任务句柄
void Start_Task(void *pvParameters); 		//任务函数

#define CAN_PROCESSINIT_PRIO  0 				//任务优先级
TaskHandle_t can_processInit_Handler; 			//任务句柄
void can_processInit(void *pvParameters); 		//任务函数

//#define CAN_RECEIVE_START_PRIO  7  				//任务优先级
//TaskHandle_t can_receive_start_Handler; 			//任务句柄
//void can_receive_start(void *pvParameters); 		//任务函数

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
	xTaskCreate((TaskFunction_t )Start_Task, 			//任务函数
				(const char* )"Start_Task", 			//任务名称
				(uint16_t )STK_SIZE_128, 				//任务堆栈大小
				(void* )NULL, 							//传递给任务函数的参数
				(UBaseType_t )START_TASK_PRIO, 			//任务优先级
				(TaskHandle_t* )&StartTask_Handler); 	//任务句柄

}

void Start_Task(void *pvParameters){
	taskENTER_CRITICAL();
	xTaskCreate((TaskFunction_t )can_processInit, 			//任务函数
				(const char* )"can_processInit", 			//任务名称
				(uint16_t )STK_SIZE_128, 				//任务堆栈大小
				(void* )NULL, 							//传递给任务函数的参数
				(UBaseType_t )CAN_PROCESSINIT_PRIO, 			//任务优先级
				(TaskHandle_t* )&can_processInit_Handler); 	//任务句柄
//	xTaskCreate((TaskFunction_t )can_receive_start, 			//任务函数
//				(const char* )"can_receive_start", 			//任务名称
//				(uint16_t )STK_SIZE_256, 				//任务堆栈大小
//				(void* )NULL, 							//传递给任务函数的参数
//				(UBaseType_t )CAN_RECEIVE_START_PRIO, 			//任务优先级
//				(TaskHandle_t* )&can_receive_start_Handler); 	//任务句柄
	xTaskCreate((TaskFunction_t )pathMonitorThd, 			//任务函数
				(const char* )"pathMonitorThd", 			//任务名称
				(uint16_t )STK_SIZE_128, 				//任务堆栈大小
				(void* )NULL, 							//传递给任务函数的参数
				(UBaseType_t )PATHMONITORTHD_PRIO, 			//任务优先级
				(TaskHandle_t* )&pathMonitorThd_Handler); 	//任务句柄
	xTaskCreate((TaskFunction_t )BuckProgThd, 			//任务函数
				(const char* )"BuckProgThd", 			//任务名称
				(uint16_t )STK_SIZE_128, 				//任务堆栈大小
				(void* )NULL, 							//传递给任务函数的参数
				(UBaseType_t )BUCKPROGTHD_PRIO, 			//任务优先级
				(TaskHandle_t* )&BuckProgThd_Handler); 	//任务句柄
	xTaskCreate((TaskFunction_t )voltMonThd, 			//任务函数
				(const char* )"voltMonThd", 			//任务名称
				(uint16_t )STK_SIZE_128, 				//任务堆栈大小
				(void* )NULL, 							//传递给任务函数的参数
				(UBaseType_t )VOLTMONTHD_PRIO, 			//任务优先级
				(TaskHandle_t* )&voltMonThd_Handler); 	//任务句柄

	xTaskCreate((TaskFunction_t )pathSwitchThd, 			//任务函数
				(const char* )"pathSwitchThd", 			//任务名称
				(uint16_t )STK_SIZE_128, 				//任务堆栈大小
				(void* )NULL, 							//传递给任务函数的参数
				(UBaseType_t )PATHSWITCHTHD_PRIO, 			//任务优先级
				(TaskHandle_t* )&pathSwitchThd_Handler); 	//任务句柄
	xTaskCreate((TaskFunction_t )pathCalcThd, 			//任务函数
				(const char* )"pathCalcThd", 			//任务名称
				(uint16_t )STK_SIZE_128, 				//任务堆栈大小
				(void* )NULL, 							//传递给任务函数的参数
				(UBaseType_t )pathCalcThd_PRIO, 			//任务优先级
				(TaskHandle_t* )&pathCalcThd_Handler); 	//任务句柄
	xTaskCreate((TaskFunction_t )pidCalcThd, 			//任务函数
				(const char* )"pidCalcThd", 			//任务名称
				(uint16_t )STK_SIZE_128, 				//任务堆栈大小
				(void* )NULL, 							//传递给任务函数的参数
				(UBaseType_t )pidCalcThd_PRIO, 			//任务优先级
				(TaskHandle_t* )&pidCalcThd_Handler); 	//任务句柄
	xTaskCreate((TaskFunction_t )pathRampThd, 			//任务函数
				(const char* )"pathRampThd", 			//任务名称
				(uint16_t )STK_SIZE_128, 				//任务堆栈大小
				(void* )NULL, 							//传递给任务函数的参数
				(UBaseType_t )pathRampThd_PRIO, 			//任务优先级
				(TaskHandle_t* )&pathRampThd_Handler); 	//任务句柄
	vTaskDelay(500);
	vTaskSuspend(StartTask_Handler); 					//删除开始任务			

	taskEXIT_CRITICAL(); 								//退出临界区		
}
