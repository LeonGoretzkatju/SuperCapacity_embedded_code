#ifndef BUCKPROG_H_
#define BUCKPROG_H_

#include "MCP4726.h"

#define MV2DAC      1.241212f

dac_t* buckData(void);
void buckInit(void);

#endif /* 2_ROUTINE_INC_BUCKPROG_H_ */
