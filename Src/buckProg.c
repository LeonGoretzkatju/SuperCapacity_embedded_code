#include "buckProg.h"
#include "MCP4726.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "sys.h"
#include "pathControl.h"
#include "task.h"
#define true 1
static dac_t dac;
void BuckProgThd(void *pvParameters){
	static systime_t now = 0;
  static systime_t next = 0;
	while (true){
		now = xTaskGetTickCount();
		next = now + US2ST(1000);
		dac.dacVal = (uint16_t) dac.mV * MV2DAC;
		dacUpdate(&dac);
		vTaskDelay(next-now);
	}
};
dac_t* buckData(void) {

	return &dac;

}
void buckInit(void) {
  dacInit(&dac);
	void BuckProgThd(void *pvParameters);
}
