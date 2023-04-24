#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

#include <CustomMotor.h>
#include "Clock.h"
#include "CortexM.h"
#include "../inc/TimerA1.h"
#include "Tachometer.h"

void initPIDMotorControl(int leftDesired, int rightDesired);
void setMotorSpeedDesired(int leftSpeed, int rightSpeed);
void updateController(void);

void setK(float Kp, float Ki, float Kd);
float getKp(void);
float getKi(void);
float getKd(void);

#endif /* PIDCONTROLLER_H_ */
