#include "pathMonitor.h"
#include "spi.h"
#include "stdint.h"
#include "pathControl.h"
#include "main.h"
#include "task.h"
#include "sys.h"
#define true 1
GPIO_InitTypeDef aSpiCfg;
GPIO_InitTypeDef bSpiCfg;
static systime_t now = 0;
static systime_t next = 0;
pathStatus* pathMonitorData(void) {

	return &currentPath;

}
void pathMonitorThd(void *pvParameters){
	while (true){
		now = xTaskGetTickCount();
		next = now + US2ST(1000);
		currentSensorUpdate(&(currentPath.pathA), &aSpiCfg,
                        &(currentPath.pathB), &bSpiCfg);
		vTaskDelay(next-now);
	}
}
void pathMonitorInit(void) {
		extern void currentSensorInit();
	  currentSensorInit(&(currentPath.pathA), &aSpiCfg);
		currentSensorInit(&(currentPath.pathB), &bSpiCfg);
		void pathMonitorThd(void *pvParameters);
}
