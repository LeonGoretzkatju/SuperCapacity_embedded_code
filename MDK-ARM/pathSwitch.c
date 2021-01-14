#include "pathSwitch.h"
#include "LED.h"
#include "stm32f1xx_hal.h"
#include "gpio.h"
GPIO_InitTypeDef GPIO_Struct;
#define false 0
#define pal_lld_writegroup(port, mask, offset, bits)                        \
  do {                                                                      \
    (void)port;                                                             \
    (void)mask;                                                             \
    (void)offset;                                                           \
    (void)bits;                                                             \
  } while (false)

void pathSwitchInit(void){
	GPIO_Struct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_Struct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Struct.Pull = GPIO_PULLUP;
	pal_lld_writegroup(GPIOB, 0x07, 12, PATHDIODE);
//	HAL_GPIO_WritePin(GPIOB,GPIO_Struct.Pin ,PATHDIODE);
}
void pathSwitch(pathEnum path){

  switch(path) {

  case DIODE :
    pal_lld_writegroup(GPIOB, 0x07, 12, PATHDIODE);
    break;

  case JUDGE :
		pal_lld_writegroup(GPIOB, 0x07, 12, PATHJUDGE);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 0);
    break;

  case BOOST :
		pal_lld_writegroup(GPIOB, 0x07, 12, PATHBOOST);
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, 1);
    break;
  }

}
