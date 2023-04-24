/*
 * PID_Motor.c
 *
 *  Created on: Apr 23, 2023
 *      Author: Bronco
 */

#include <stdint.h>
#include <stdio.h>
#include "PID_Motor.h"
#include "Motor.h"
#include "Tachometer.h"
#include "../inc/UART0.h"

int32_t Kp, Ki, Kd;
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
uint8_t i = 0;

uint16_t avg(uint16_t *array, int length)
{
  int i;
  uint32_t sum = 0;

  for(i=0; i<length; i=i+1)
  {
    sum = sum + array[i];
  }
  return (sum/length);
}

void PID_Motor_Init(int32_t Kp_input, int32_t Ki_input, int32_t Kd_input){
    Motor_Init();
    Kp = Kp_input;
    Ki = Ki_input;
    Kp = Kd_input;
}

void PID_Motor_Target(int16_t Left_RPM, int16_t Right_RPM){
    UL = Left_RPM*40;
    UR = Right_RPM*40;
    DesiredL = Left_RPM;
    DesiredR = Right_RPM;
}

void PID_Motor_Forward(){
    Motor_Forward(UL,UR);
    Tachometer_Get(&LeftTach[i], &LeftDir, &LeftSteps, &RightTach[i], &RightDir, &RightSteps);
    i = i + 1;

    if(i >= TACHBUFF){
        //This section of the code checks the wheel state every second (10*100ms)
        i = 0;

        //Sense state of wheels (in RPM) and take the average of the last n values
        // (1/tach step/cycles) * (12,000,000 cycles/sec) * (60 sec/min) * (1/360 rotation/step)
        ActualL = 2000000/avg(LeftTach, TACHBUFF);
        ActualR = 2000000/avg(RightTach, TACHBUFF);

        //Calculate Error signals
        Error_L = DesiredL - ActualL;
        Error_R = DesiredR - ActualR;

        Error_L_chng = Error_L - Error_L_prev;
        Error_R_chng = Error_R - Error_R_prev;

        //PID Control Law
        //Initially, only the I-term is implemented, you must add the P and D terms.
        UL = Kp*Error_L + (UL + (Ki*Error_L)) + Kd*Error_L_chng;  // adjust left motor
        UR = Kp*Error_R + (UR + (Ki*Error_R)) + Kd*Error_R_chng;  // adjust right motor

        Error_L_prev = Error_L;
        Error_R_prev = Error_R;
        Motor_Forward(UL,UR);

        printf("\n%5d, %5d, %5d, %5d, %5d, %5d, %5d",t,DesiredL,ActualL,Error_L,DesiredR,ActualR,Error_R);
    }
}

void PID_Motor_Stop(void){
    Motor_Stop();
}


