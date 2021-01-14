#include "LED.h"
#include "main.h"
#include "gpio.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
void LED_init(void){
	HAL_GPIO_TogglePin(GPIOA, LED_Pin); 
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}
