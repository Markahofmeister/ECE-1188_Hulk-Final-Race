/*
 * PID_Motor.c
 *
 *  Created on: Apr 23, 2023
 *      Author: Bronco
 */

#include <stdint.h>
#include <stdio.h>
#include "PID_Motor.h"
#include "../inc/Motor.h"
#include "Tachometer.h"
#include "../inc/UART0.h"

int32_t Kp_motor, Ki_motor, Kd_motor;
int32_t UR, UL;  // PWM duty 0 to 7500
uint16_t DesiredL; // desired rotations per minute
uint16_t DesiredR;
uint16_t ActualL;                        // actual rotations per minute
uint16_t ActualR;                        // actual rotations per minute

#define TACHBUFF 10                      // number of elements in tachometer array
uint16_t LeftTach[TACHBUFF];             // tachometer period of left wheel (number of 0.0833 usec cycles to rotate 1/360 of a wheel rotation)
enum TachDirection LeftDir;              // direction of left rotation (FORWARD, STOPPED, REVERSE)
int32_t LeftSteps;                       // number of tachometer steps of left wheel (units of 220/360 = 0.61 mm traveled)
uint16_t RightTach[TACHBUFF];            // tachometer period of right wheel (number of 0.0833 usec cycles to rotate 1/360 of a wheel rotation)
enum TachDirection RightDir;             // direction of right rotation (FORWARD, STOPPED, REVERSE)
int32_t RightSteps;                      // number of tachometer steps of right wheel (units of 220/360 = 0.61 mm traveled)

#define PWMMIN 2
#define PWMMAX 7500
int32_t Error_L;
int32_t Error_R;
int32_t Error_L_sum = 0;
int32_t Error_R_sum = 0;
int32_t Error_L_prev = 0;
int32_t Error_R_prev = 0;
int32_t Error_L_chng = 0;
int32_t Error_R_chng = 0;
int32_t t;
uint8_t i_motor = 0;

//uint16_t avg(uint16_t *array, int length)
//{
//  int i_motor;
//  uint32_t sum = 0;
//
//  for(i_motor=0; i_motor<length; i_motor=i_motor+1)
//  {
//    sum = sum + array[i_motor];
//  }
//  return (sum/length);
//}

void PID_Motor_Init(int32_t Kp_motor_input, int32_t Ki_motor_input, int32_t Kd_motor_input){
    Motor_Init();
    Kp_motor = Kp_motor_input;
    Ki_motor = Ki_motor_input;
    Kp_motor = Kd_motor_input;
}

void PID_Motor_Target(int16_t Left_RPM, int16_t Right_RPM){
    UL = Left_RPM*40;
    UR = Right_RPM*40;
    DesiredL = Left_RPM;
    DesiredR = Right_RPM;
}

void PID_Motor_Forward(uint16_t *LeftRPMPtr, uint16_t *RightRPMPtr){
    setMotorSpeed(UL,UR);
    Tachometer_Get(&LeftTach[i_motor], &LeftDir, &LeftSteps, &RightTach[i_motor], &RightDir, &RightSteps);
    i_motor = i_motor + 1;

    if(i_motor >= TACHBUFF){
        //This section of the code checks the wheel state every second (10*100ms)
        i_motor = 0;

        //Sense state of wheels (in RPM) and take the average of the last n values
        // (1/tach step/cycles) * (12,000,000 cycles/sec) * (60 sec/min) * (1/360 rotation/step)
        ActualL = 2000000/avg(LeftTach, TACHBUFF);
        ActualR = 2000000/avg(RightTach, TACHBUFF);

        // Update Pointers
        *LeftRPMPtr = ActualL;
        *RightRPMPtr = ActualR;

        //Calculate Error signals
        Error_L = DesiredL - ActualL;
        Error_R = DesiredR - ActualR;

        Error_L_chng = Error_L - Error_L_prev;
        Error_R_chng = Error_R - Error_R_prev;

        //PID Control Law
        //Initially, only the I-term is implemented, you must add the P and D terms.
        UL = Kp_motor*Error_L + (UL + (Ki_motor*Error_L)) + Kd_motor*Error_L_chng;  // adjust left motor
        UR = Kp_motor*Error_R + (UR + (Ki_motor*Error_R)) + Kd_motor*Error_R_chng;  // adjust right motor

        Error_L_prev = Error_L;
        Error_R_prev = Error_R;
        setMotorSpeed(UL,UR);

        printf("\n%5d, %5d, %5d, %5d, %5d, %5d, %5d",t,DesiredL,ActualL,Error_L,DesiredR,ActualR,Error_R);
    }
}

void PID_Motor_Stop(void){
    setMotorSpeed(0,0);
}

