/*
 * pidController.c
 *
 *  Created on: Apr 24, 2023
 *      Author: jsb19
 */

#include "pidController.h"

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

int DesiredL = 0;                  // desired rotations per minute
int DesiredR = 0;                  // desired rotations per minute
#define DESIREDMAX 120                   // maximum rotations per minute
#define DESIREDMIN  30                   // minimum rotations per minute (works poorly at 30 RPM due to 16-bit timer overflow)
uint16_t ActualL;                        // actual rotations per minute
uint16_t ActualR;                        // actual rotations per minute
uint16_t LeftDuty = 500;                // duty cycle of left wheel (0 to 14,998)
uint16_t RightDuty = 500;               // duty cycle of right wheel (0 to 14,998)

#define TACHBUFF 10                      // number of elements in tachometer array
uint16_t LeftTach[TACHBUFF];             // tachometer period of left wheel (number of 0.0833 usec cycles to rotate 1/360 of a wheel rotation)
enum TachDirection LeftDir;              // direction of left rotation (FORWARD, STOPPED, REVERSE)
int32_t LeftSteps;                       // number of tachometer steps of left wheel (units of 220/360 = 0.61 mm traveled)
uint16_t RightTach[TACHBUFF];            // tachometer period of right wheel (number of 0.0833 usec cycles to rotate 1/360 of a wheel rotation)
enum TachDirection RightDir;             // direction of right rotation (FORWARD, STOPPED, REVERSE)
int32_t RightSteps;                      // number of tachometer steps of right wheel (units of 220/360 = 0.61 mm traveled)

#define PWMMIN 2
#define PWMMAX 14998
float Error_L;
float Error_R;
float Ki=0;  //integral gain
float Kp=50;
float Kd=10;
float smoothingFactor = .1;
int32_t t;
float UR, UL;  // PWM duty 0 to 14,998
float totalErrorLeft;
float totalErrorRight;
float oldErrorLeft;
float oldErrorRight;
int i;

void initPIDMotorControl(int leftDesired, int rightDesired)
{
    UR = RightDuty;
    UL = LeftDuty;
    Clock_Init48MHz();// set system clock to 48 MHz
    Tachometer_Init();

    motorPWMInit(15000, 0, 0);
    EnableInterrupts();
    t = 0;

    totalErrorLeft = 0;
    totalErrorRight = 0;
    oldErrorLeft = 0;
    oldErrorRight = 0;
    ActualL = 0;
    ActualR = 0;

    i = 0;

    DesiredL = leftDesired;
    DesiredR = rightDesired;
}

void setMotorSpeedDesired(int leftSpeed, int rightSpeed)
{
    DesiredL = leftSpeed;
    DesiredR = rightSpeed;
}

void updateController(void)
{
    Tachometer_Get(&LeftTach[i], &LeftDir, &LeftSteps, &RightTach[i], &RightDir, &RightSteps);

    i = i + 1;

    if(i >= TACHBUFF)
    {

        //This section of the code checks the wheel state every second (10*100ms)
        i = 0;

        //Sense state of wheels (in RPM) and take the average of the last n values
        // (1/tach step/cycles) * (12,000,000 cycles/sec) * (60 sec/min) * (1/360 rotation/step)
        ActualL = (1-smoothingFactor) * (2000000/avg(LeftTach, TACHBUFF)) + smoothingFactor * ActualL;
        ActualR = (1 - smoothingFactor) * (2000000/avg(RightTach, TACHBUFF)) + smoothingFactor * ActualR;

        //Calculate Error signals
        Error_L = ((float)DesiredL - (float)ActualL);
        Error_R = ((float)DesiredR - (float)ActualR);

        totalErrorLeft += Error_L;
        totalErrorRight += Error_R;


         //PID Control Law
         //Initially, only the I-term is implemented, you must add the P and D terms.
        UL += Kp * (float)Error_L + Ki * (float)totalErrorLeft + Kd * (float)(Error_L - oldErrorLeft);
        UR += Kp * (float)Error_R + Ki * (float)totalErrorRight + Kd * (float)(Error_R - oldErrorRight);

        int newUL = UL, newUR = UR;

        if(DesiredL < 0)
            newUL *= -1;
        else if(DesiredL == 0)
            newUL = 0;
        if(DesiredR < 0)
            newUR *= -1;
        else if(DesiredR == 0)
            newUR = 0;

        setMotorSpeed(newUL, newUR);
    }
    t = t +100; // keep track of timer
}

void setK(float KpNew, float KiNew, float KdNew)
{
    Kp = KpNew;
    Ki = KiNew;
    Kd = KdNew;
}

float getKp(void)
{
    return Kp;
}
float getKi(void)
{
    return Ki;
}
float getKd(void)
{
    return Kd;
}
