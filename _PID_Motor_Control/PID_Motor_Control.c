// Lab17_Control.c
// Runs on MSP432
// Implementation of the control system.
// Daniel and Jonathan Valvano
// July 11, 2019

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

#include <stdint.h>
#include <stdio.h>
#include "msp.h"
#include "../inc/Clock.h"
#include "../inc/CortexM.h"
#include "../inc/PWM.h"
#include "../inc/UART0.h"
#include "../inc/Motor.h"
#include "../inc/Bump.h"
#include "../inc/TimerA1.h"
#include "../inc/Tachometer.h"


//****************************************************************************
// incremental speed control from Lab 16

// ------------avg------------
// Simple math function that returns the average
// value of an array.
// Input: array is an array of 16-bit unsigned numbers
//        length is the number of elements in 'array'
// Output: the average value of the array
// Note: overflow is not considered
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

uint16_t DesiredL = 100;                  // desired rotations per minute
uint16_t DesiredR = 100;                  // desired rotations per minute
#define DESIREDMAX 120                   // maximum rotations per minute
#define DESIREDMIN  30                   // minimum rotations per minute (works poorly at 30 RPM due to 16-bit timer overflow)
uint16_t ActualL;                        // actual rotations per minute
uint16_t ActualR;                        // actual rotations per minute
uint16_t LeftDuty = 3750;                // duty cycle of left wheel (0 to 14,998)
uint16_t RightDuty = 3750;               // duty cycle of right wheel (0 to 14,998)

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
int32_t Error_L_prev = 0;
int32_t Error_R_prev = 0;
int32_t Error_L_chng = 0;
int32_t Error_R_chng = 0;
int32_t Kp=20;  //proportional gain
int32_t Ki=0;  //integral gain
int32_t Kd=2;  //derivative gain
int32_t t;
int32_t UR, UL;  // PWM duty 0 to 14,998

void main(void)
{
  int i = 0;
  Clock_Init48MHz();// set system clock to 48 MHz

  UART0_Initprintf(); // BAUD Rate = 115,200 bps
  printf("\n\rLab 17 speed controller\n\r");

  Bump_Init();
  Tachometer_Init();
  Motor_Init();
  EnableInterrupts();

  //Start motors with nominal values
  UR = RightDuty;
  UL = LeftDuty;

  t = 0;

  while(1)
  {
    Motor_Stop();

    i = 0;
    Motor_Forward(UL,UR);
    while(Bump_Read() == 0)
    {

      Tachometer_Get(&LeftTach[i], &LeftDir, &LeftSteps, &RightTach[i], &RightDir, &RightSteps);

      i = i + 1;

      if(i >= TACHBUFF)
      {

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
         UL = Kp*Error_L + (UL + (Ki*Error_L/1024)) + Kd*Error_L_chng;  // adjust left motor
         UR = Kp*Error_R + (UR + (Ki*Error_R/1024)) + Kd*Error_R_chng;  // adjust right motor



        Error_L_prev = Error_L;
        Error_R_prev = Error_R;
        Motor_Forward(UL,UR);

        printf("\n%5d, %5d, %5d, %5d, %5d, %5d, %5d",t,DesiredL,ActualL,Error_L,DesiredR,ActualR,Error_R);
        /*
        printf("\nt = %d ms\n\r",t);
        printf("%5d Target RPM(Left)  %5d Target RPM(Right)\n\r",DesiredL,DesiredR);
        printf("%5d Actual RPM(Left)  %5d Actual RPM(Right)\n\r",ActualL,ActualR);
        printf("%5d Error  RPM(Left)  %5d Error  RPM(Right)\n\r",Error_L,Error_R);
        */

      }
      Clock_Delay1ms(100); // delay ~0.1 sec at 48 MHz
      t = t +100; // keep track of timer
    }
    Motor_Stop();
  }
}
