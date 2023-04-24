/*
 * PID_Motor.h
 *
 *  Created on: Apr 23, 2023
 *      Author: Bronco
 */

#ifndef PID_MOTOR_H_
#define PID_MOTOR_H_

void PID_Motor_Init(int32_t Kp_input, int32_t Ki_input, int32_t Kd_input);

void PID_Motor_Target(int16_t Left_RPM, int16_t Right_RPM);

void PID_Motor_Forward(uint16_t *LeftRPMPtr, uint16_t *RightRPMPtr, int32_t *LeftD, int32_t *RightD);

void PID_Motor_Stop(void);

void PID_Motor_Turn_Right(void);

#endif /* PID_MOTOR_H_ */
