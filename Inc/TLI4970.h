#include "stdint.h"
#include "spi.h"
#ifndef TLI4970_H_
#define TLI4970_H_

#define TLI4970_NORMAL_ID       (uint16_t)0
#define TLI4970_D025_OFFSET     4096
#define TLI4970_D025_LSB2MA     12.5f

#define CURRENTSAMPLES          20

typedef struct currentSensor{

  SPIDriver* driver;
  uint16_t error;
  uint16_t rawData;
  uint16_t rawCurrent;
  int32_t current;      //mA

} currentSensor;

typedef struct spiDriver_t{

  SPIDriver* driver;
  GPIO_InitTypeDef port;
  uint16_t pad;
  uint16_t data;
  uint8_t flag;

} spiDriver_t;
void currentSensorInit(currentSensor* sensor,GPIO_InitTypeDef* cfg);
void currentSensorUpdate(currentSensor* sensorA,GPIO_InitTypeDef* cfgA,
                         currentSensor* sensorB, GPIO_InitTypeDef* cfgB);
#endif
