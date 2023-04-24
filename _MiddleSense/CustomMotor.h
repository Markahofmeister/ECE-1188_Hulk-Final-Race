#ifndef CUSTOMMOTOR_H_
#define CUSTOMMOTOR_H_

#include <stdint.h>
#include "msp.h"
#include "CortexM.h"

//Init function
void motorPWMInit(uint16_t period, uint16_t duty1, uint16_t duty2);

//Motor PWM and direction accessors
void PWM_LeftMotor(uint16_t duty);
void PWM_RightMotor(uint16_t duty);
void PWM_LeftMotorBackwards(uint16_t duty);
void PWM_RightMotorBackwards(uint16_t duty);

void setMotorSpeed(int leftDuty, int rightDuty);


#endif /* CUSTOMMOTOR_H_ */
