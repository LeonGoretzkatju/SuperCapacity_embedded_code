#include "TLI4970.h"
#include "spi.h"
#include "adc.h"
#include "gpio.h"
#include "sys.h"
#include "stdint.h"
extern uint16_t Get_Adc();
void currentSensorInit(currentSensor* sensor,GPIO_InitTypeDef* cfg) {
	if(cfg->Pin == (GPIO_PIN_5|GPIO_PIN_7))
    cfg->Mode = GPIO_MODE_AF_PP;
  else if(cfg->Pin == GPIO_PIN_6)
    cfg->Mode = GPIO_MODE_INPUT;
    cfg->Pull = GPIO_NOPULL;
}

u8 SPI1_ReadByte(u8 data)
{
    
    HAL_SPI_Receive(&hspi1,&data,1, 1000);       
 	return data;          		    //返回收到的数据		
}
uint16_t currentSensorSample(currentSensor* sensor,GPIO_InitTypeDef* cfg) {

  static uint16_t data;
	data = Get_Adc(ADC_CHANNEL_1);
//  spiAcquireBus(sensor->driver);
//  spiStart(sensor->driver, cfg);
//  spiSelect(sensor->driver);
//  spiReceive(sensor->driver, 1, &data);
//  spiUnselect(sensor->driver);
//  spiReleaseBus(sensor->driver);
	SPI1_ReadByte(data);
  if (((data >> 15) & 1) == TLI4970_NORMAL_ID) {
    uint16_t parity = data & 0x1FFF;
    parity ^= parity >> 8;
    parity ^= parity >> 4;
    parity ^= parity >> 2;
    parity ^= parity >> 1;
    parity = (~parity) & 1;         //get parity bit, equals to 1 if data is even
    if (((data >> 14) & 1) == parity) {
      sensor->error = 0;
      sensor->rawCurrent = data & 0x1FFF;
    }
  } else {
    uint16_t parity = data & 0xFFFF;
    parity ^= parity >> 8;
    parity ^= parity >> 4;
    parity ^= parity >> 2;
    parity ^= parity >> 1;
    parity = (~parity) & 1;         //get parity bit, equals to 1 if data is even
    if (((data >> 14) & 1) == parity) {
      sensor->error = data;
    }
  }

  return sensor->rawCurrent;

}
void currentSensorUpdate(currentSensor* sensorA,GPIO_InitTypeDef* cfgA,
                         currentSensor* sensorB,GPIO_InitTypeDef* cfgB) {

  int32_t avgA = 0;
  int32_t avgB = 0;

  for (uint8_t i = 0; i < CURRENTSAMPLES; i++) {

    avgA += currentSensorSample(sensorA, cfgA);
    avgB += currentSensorSample(sensorB, cfgB);
  }

  sensorA->current = ((avgA / CURRENTSAMPLES) - TLI4970_D025_OFFSET) * TLI4970_D025_LSB2MA;
  sensorB->current = ((avgB / CURRENTSAMPLES) - TLI4970_D025_OFFSET) * TLI4970_D025_LSB2MA;

}
