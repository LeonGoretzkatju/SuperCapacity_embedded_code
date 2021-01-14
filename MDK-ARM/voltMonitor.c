#include "adc.h"
#include "delay.h"
#include "gpio.h"
#include "voltMonitor.h"
#include "sys.h"
#include "pathControl.h"
#include "task.h"
#define true 1
extern void adcDriverUpdate();
void voltMonThd(void *pvParameters){
	static systime_t now = 0;
  static systime_t next = 0;
	while (true){
		now = xTaskGetTickCount();
	  next = now + US2ST(1000);
		adcDriverUpdate(&voltData);
		vTaskDelay(next-now);
	}
}

voltages* voltMonData(void) {

    return &voltData;
}

void voltMoninit(void) {
	void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle);
	void voltMonThd(void *pvParameters);
}

