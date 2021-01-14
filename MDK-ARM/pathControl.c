#include "TLI4970.h"
#include "pathMonitor.h"
#include "buckProg.h"
#include "MCP4726.h"
#include "adc.h"
#include "pathControl.h"
#include "pathSwitch.h"
#include "LED.h"
#include "bxCAN.h"
#include "math.h"
#include "voltMonitor.h"
#include "task.h"
#include "Task_Create.h"
#define true 1
pathData_t pathData;
pidData_t buckPid;
//receivedata_t receivedata;
userCommand_t* userCommand;
extern pathStatus currentPath;
uint32_t timeoutCount = 0;
pathData_t* returnPathData(void) {

	return &pathData;

}
void pidCalcThd(void *pvParameters){
	static systime_t now = 0;
  static systime_t next = 0;

  buckPid.kP = 1.0;
  buckPid.kI = 0.066;
  buckPid.kD = 0.025;
  buckPid.last_error = 0;
  buckPid.maxTarget = pathData.riseThresh - pathData.chargeMargin;
  buckPid.maxOut = 1500;
  buckPid.maxMilliVolt = 23200;

  while(true) {

		now = xTaskGetTickCount();
		next = now + US2ST(100);

    buckPid.fb = pathData.buckPower;
    buckPid.error = buckPid.target - pathData.buckPower;
    buckPid.p = buckPid.error * buckPid.kP;
    buckPid.d = (buckPid.error - buckPid.last_error) * buckPid.kD;
    buckPid.i = buckPid.accumError * buckPid.kI;
    buckPid.out = buckPid.p + buckPid.i + buckPid.d;
    buckPid.out = buckPid.out > buckPid.maxOut ? buckPid.maxOut : buckPid.out;
    buckPid.accumError += buckPid.out == buckPid.maxOut ? 0 :
    buckPid.error * buckPid.kI;																	//anti integral windup
    buckPid.accumError = pathData.voltage->vcapMv > buckPid.maxMilliVolt ? 0 : buckPid.accumError;
    buckPid.out = buckPid.out < 0 ? 0 : buckPid.out;
    pathData.buckProg->mV = buckPid.out;
    buckPid.last_error = buckPid.error;

    vTaskDelay(next-now);

  }
}
//TaskHandle_t pathRampThd_Handler;
void pathRampThd(void *pvParameters){
	while (!INCLUDE_vTaskSuspend){
	for(uint16_t i = RISETHRESH; i >= MARGINFINAL ; i--) {
			static systime_t now = 0;
			static systime_t next = 0;
      pathData.chargeMargin = i;
			vTaskDelay(next-now);
  	}
	vTaskSuspend(pathRampThd);
	}
}
void pathCalcThd(void *pvParameters){
	static systime_t now = 0;
  static systime_t next = 0;
	static int32_t target;

  while(true) {

    now = xTaskGetTickCount();
    next = now + US2ST(100);
    pathData.judgePower = -pathData.current->pathA.current * pathData.voltage->vinMv / 1000000;
    pathData.outPower = pathData.voltage->ioutMa * pathData.voltage->voutMv / 1000000;
		pathData.ChassisPower = userCommand->ChassisPower;
    pathData.buckPower = -pathData.current->pathB.current * pathData.voltage->vinMv / 1000000;

    pathData.capEnergy = pathData.capacitance / 2 *
    										 ((pow((pathData.voltage->vcapMv), 2) / 1000000) - (pow((pathData.VcapMin), 2) / 1000000));

//    pathData.riseThresh = userCommand->robotType == 1 ? 75 : 110;
		pathData.riseThresh = userCommand->chassis_max_power;


    target = pathData.path == JUDGE ?
    				 pathData.riseThresh - pathData.chargeMargin - pathData.outPower :		//Judge Power
						 pathData.riseThresh - pathData.chargeMargin;													//Cap Power
						 //0;
    target = target > buckPid.maxOut ? buckPid.maxOut : target;
    target = target < 0 ? 0 : target;

    buckPid.target = target;

    vTaskDelay(next-now);

  }
}
void pathSwitchThd(void *pvParameters){
	static systime_t now = 0;
  static systime_t next = 0;
	pathData.riseThresh = 75;
  pathData.fallThresh = 55;
  pathData.deadTime = 200;
  pathData.chargeMargin = 75;				//10
  pathData.capacitance = 11.1;
  pathData.VcapMin = 18000;
	pathData.VcapMax = 23000;
  pathData.sysInit = 0;

  pathData.buckProg = buckData();
  pathData.current = &currentPath;
  pathData.voltage = &voltData;
	void pathCalcThd(void *pvParameters);
	void pidCalcThd(void *pvParameters);
	void pathRampThd(void *pvParameters);
	while (true){
		now = xTaskGetTickCount();
    next = now + US2ST(100);
		timeoutCount += 1 - userCommand->msgUpdated;
		if (userCommand->msgUpdated)
		{											//Switch path according to chassis command
//    	pathData.path = userCommand->pathType;
// 			userCommand->msgUpdated = 0;
			timeoutCount = 0;
//			if(pathData.voltage->vcapMv < pathData.VcapMin )
//			{
//				pathData.path = JUDGE;
//			}
//			else if(pathData.voltage->vcapMv >= 1.5f*pathData.VcapMin)
//			{
//				if(pathData.outPower < userCommand->chassis_max_power*0.8f)
//				{
//					pathData.path = JUDGE;
//				}
//				else if(pathData.outPower >= userCommand->chassis_max_power)
//				{
//					pathData.path = BOOST;
//				}	
//			}
//			else
//			{
//				pathData.path = JUDGE;
//			}
		switch (pathData.mode)
			{
			case 0:
				{
					if(pathData.voltage->vcapMv < 1.5f*pathData.VcapMin)
					{
						pathData.path = JUDGE;
					}
					else
					{
						pathData.mode = 1;
					}
				}break;
			case 1:
				{
				if(pathData.voltage->vcapMv > pathData.VcapMin)
					{	if(pathData.outPower < userCommand->chassis_max_power*0.8f)
						{
							pathData.path = JUDGE;
						}
						else if(pathData.outPower >= userCommand->chassis_max_power)
						{
							pathData.path = BOOST;
						}	
					}
				else
					{
						pathData.mode = 0;
					}
				}break;
			default:
				{
					pathData.mode = 0;
				}break;
			}
    } 
		else if (timeoutCount > TIMEOUTCNT) 
		{							//Timeout, switch to judge power
    	pathData.path = JUDGE;
    }

//    if (pathData.voltage->vcapMv < pathData.VcapMin) {	//SuperCap voltage low, switch to judge power
//    	pathData.path = JUDGE;
//    }
//		if(pathData.voltage->vcapMv < pathData.VcapMin )
//		{
//			pathData.path = JUDGE;
//		}
//		else
//		{
//		if(pathData.outPower < userCommand->chassis_max_power*0.8f)
//		{
//			pathData.path = JUDGE;
//		}
//		else if(pathData.outPower >= userCommand->chassis_max_power)
//		{
//			pathData.path = BOOST;
//		}	
//		}	
//		else
//		{	
//			pathData.path = BOOST;
//		}
		
    pathSwitch(pathData.path);
		vTaskDelay(next-now);
	}
}

extern userCommand_t* returnCommand(void);
void pathControlInit(void) {

  pathData.path = JUDGE;

  userCommand = returnCommand();

  pathSwitchInit();

//  chThdCreateStatic(pathSwitchThd_wa, sizeof(pathSwitchThd_wa),
//                    NORMALPRIO + 12, pathSwitchThd, NULL);
	void pathSwitchThd(void *pvParameters);
}
