#ifndef DIST_H_
#define DIST_H_

#include <stdint.h>
#include "msp.h"
#include "Clock.h"
#include "I2CB1.h"
#include "CortexM.h"
#include "opt3101.h"
#include "LPF.h"

void Dist_Init(void);
void getDist(uint32_t *distanceBuf);


#endif /* DIST_H_ */
