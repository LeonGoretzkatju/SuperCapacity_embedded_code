#ifndef PATHMONITOR_H_
#define PATHMONITOR_H_
#include "TLI4970.h"
#include "gpio.h"
#include "main.h"
#include "spi.h"
//GPIO_InitTypeDef aSpiCfg;
//GPIO_InitTypeDef bSpiCfg;
void pathMonitorInit(void);
typedef struct pathStatus{

  currentSensor pathA;
  currentSensor pathB;

}pathStatus;


//extern pathStatus current;
#endif
