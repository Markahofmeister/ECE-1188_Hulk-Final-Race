// Motor.c
// Runs on MSP432
// Provide mid-level functions that initialize ports and
// set motor speeds to move the robot. Lab 13 solution
// Daniel Valvano
// July 11, 2019


// Left motor direction connected to P5.4 (J3.29)
// Left motor PWM connected to P2.7/TA0CCP4 (J4.40)
// Left motor enable connected to P3.7 (J4.31)
// Right motor direction connected to P5.5 (J3.30)
// Right motor PWM connected to P2.6/TA0CCP3 (J4.39)
// Right motor enable connected to P3.6 (J2.11)

#include <stdint.h>
#include "msp.h"
#include "CortexM.h"
#include "PWM.h"



// *******Lab 13 solution*******

// ------------Motor_Init------------
// Initialize GPIO pins for output, which will be
// used to control the direction of the motors and
// to enable or disable the drivers.
// The motors are initially stopped, the drivers
// are initially powered down, and the PWM speed
// control is uninitialized.
// Input: none
// Output: none
void Motor_Init(void){
  
    //0x30 = 0011 0000
    uint8_t mask_direction = 0x30;
    P5->SEL0 &= ~mask_direction;            // initialize P5.4 and P5.5 (direction, DIR) as GPIO with &= 0x30 mask
    P5->SEL1 &= ~mask_direction;
    P5->DIR |= mask_direction;              // initialize P5.4 and P5.5 as outputs
    P5->OUT &= ~mask_direction;             //default output to 0 (forwards)

    // 0xC0 = 1100 0000
    uint8_t mask_en = 0xC0;
    P3->SEL0 &= ~mask_en;                   // initialize P3.6 and P3.7 (enable, nSLP) as GPIO with 0xC0
    P3->SEL1 &= ~mask_en;
    P3->DIR |= mask_en;                     // initialize P2.6 and P2.7 as outputs
    P3->OUT &= ~mask_en;                    //default output to low current sleep mode

    //CHANGE BELOW
    // 0xC0 = 1100 0000
    /*uint8_t mask_PWM = 0xC0;
    P2->SEL0 &= ~mask_PWM;                  // initialize P2.6 and P2.7 (PWM, EN) as GPIO with 0xC0
    P2->SEL1 &= ~mask_PWM;
    P2->DIR |= mask_PWM;                    // initialize P2.6 and P2.7 as outputs
    P2->OUT &= ~mask_PWM;                   //default output to 0*/

    //Use PWM library to initialize PWM on P2.6 and P2.7
    uint16_t initPeriod = 7500;        //period of 100Hz obtained by initPeriod * 1.333 us = (7500) * (4/3 us) = 10ms
    uint16_t initDutyRight = 0;
    uint16_t initDutyLeft = 0;

    PWM_Init34(initPeriod, initDutyRight, initDutyLeft);   //initialize PWM with 10ms period and 0% duty cycle


}

// ------------Motor_Stop------------
// Stop the motors, power down the drivers, and
// set the PWM speed control to 0% duty cycle.
// Input: none
// Output: none
void Motor_Stop(void){

  PWM_Duty3(0);
  PWM_Duty4(0);
  P3->OUT &= ~0xC0;                     //enable full power mode
  
}

// ------------Motor_Forward------------
// Drive the robot forward by running left and
// right wheels forward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Forward(uint16_t leftDuty, uint16_t rightDuty){ 

    P5->OUT &= ~0x30;             //default output to 0 (forwards)
    P3->OUT |= 0xC0;                     //enable full power mode
    PWM_Duty3(rightDuty);
    PWM_Duty4(leftDuty);

}

// ------------Motor_Right------------
// Turn the robot to the right by running the
// left wheel forward and the right wheel
// backward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Right(uint16_t leftDuty, uint16_t rightDuty){ 

    P5->OUT &= ~0x30;             //default output to 0 (forwards)
    P3->OUT |= 0xC0;                     //enable full power mode
    PWM_Duty3(rightDuty);
    PWM_Duty4(0);

}

// ------------Motor_Left------------
// Turn the robot to the left by running the
// left wheel backward and the right wheel
// forward with the given duty cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Left(uint16_t leftDuty, uint16_t rightDuty){ 

    P5->OUT &= ~0x30;             //default output to 0 (forwards)
    P3->OUT |= 0xC0;                     //enable full power mode
    PWM_Duty3(0);
    PWM_Duty4(leftDuty);

}

// ------------Motor_Backward------------
// Drive the robot backward by running left and
// right wheels backward with the given duty
// cycles.
// Input: leftDuty  duty cycle of left wheel (0 to 14,998)
//        rightDuty duty cycle of right wheel (0 to 14,998)
// Output: none
// Assumes: Motor_Init() has been called
void Motor_Backward(uint16_t leftDuty, uint16_t rightDuty){ 

    P5->OUT |= 0x30;             //default output to 1 (backwards)
    P3->OUT |= 0xC0;                     //enable full power mode
    PWM_Duty3(rightDuty);
    PWM_Duty4(leftDuty);

}
