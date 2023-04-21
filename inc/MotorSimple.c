// MotorSimple.c
// Runs on MSP432
// Provide mid-level functions that initialize ports and set motor speeds to move the robot.
// Mark Hofmeister
// 2/2/2023

#include <stdint.h>
#include "msp.h"
#include "../inc/SysTick.h"

//**************RSLK1.1***************************
// Left motor direction connected to P5.4 (J3.29)
// Left motor PWM connected to P2.7/TA0CCP4 (J4.40)
// Left motor enable connected to P3.7 (J4.31)

// Right motor direction connected to P5.5 (J3.30)
// Right motor PWM connected to P2.6/TA0CCP3 (J4.39)
// Right motor enable connected to P3.6 (J2.11)


void SysTick_Wait1us(uint32_t delay){               //uses SysTick_Wait to delay program for 1us * delay

    uint32_t i;
    for(i = 0; i < delay; i++) {
        SysTick_Wait(48);
    }

}


/*void SysTick_Wait10ms(uint32_t delay) {             //uses delay 1us function to delay 10ms for all simple function calls

    uint32_t i;
    for(i = 0; i < delay; i++) {
         SysTick_Wait1us(10000);
    }
}*/



// Initializes the 6 GPIO lines and puts driver to sleep
// Returns right away
void Motor_InitSimple(void){

    //0x30 = 0011 0000
    uint8_t mask_direction = 0x30;
    P5->SEL0 &= ~mask_direction;            // initialize P5.4 and P5.5 (direction, DIR) as GPIO with &= 0x30 mask
    P5->SEL1 &= ~mask_direction;
    P5->DIR |= mask_direction;              // initialize P5.4 and P5.5 as outputs
    P5->OUT &= ~mask_direction;             //default output to 0 (forwards)

    // 0xC0 = 1100 0000
    uint8_t mask_PWM = 0xC0;
    P2->SEL0 &= ~mask_PWM;                  // initialize P2.6 and P2.7 (PWM, EN) as GPIO with 0xC0
    P2->SEL1 &= ~mask_PWM;
    P2->DIR |= mask_PWM;                    // initialize P2.6 and P2.7 as outputs
    P2->OUT &= ~mask_PWM;                   //default output to 0

    // 0xC0 = 1100 0000
    uint8_t mask_en = 0xC0;
    P3->SEL0 &= ~mask_en;                   // initialize P3.6 and P3.7 (enable, nSLP) as GPIO with 0xC0
    P3->SEL1 &= ~mask_en;
    P3->DIR |= mask_en;                     // initialize P2.6 and P2.7 as outputs
    P3->OUT &= ~mask_en;                    //default output to low current sleep mode


}


// Stops both motors, puts driver to sleep
// Returns right away
void Motor_StopSimple(void){

  P1->OUT &= ~0xC0;             //not sure what this is for
  P2->OUT &= ~0xC0;   // off
  P3->OUT &= ~0xC0;   // low current sleep mode
}

// Drives both motors forward at duty (100 to 9900)
// Runs for time duration (units=10ms), and then stops
// Returns after time*10ms or if a bumper switch is hit

void Motor_ForwardSimple(uint16_t duty, uint32_t time){

    //P5.4 and P5.5 = direction
    //P2.6 and P2.7 = PWM
    //P3.6 and P3.7 = enable

    uint8_t mask_direction = 0x30;          //Declare masks for direction, PWM, enable.
    uint8_t mask_PWM = 0xC0;
    uint8_t mask_en = 0xC0;

    P5->OUT &= ~mask_direction;              //Set direction to forwards on both motors
    P3->OUT |= mask_en;                     //enable full power mode

    uint32_t H,L,i;                         //declare high and low integers, as well as for loop increment
    H = duty;
    L = 10000 - H;

    for(i = 0; i < time; i++) {          //Each for loop iteration should  take 10ms. Therefore, it should be of duration time.
        P2->OUT |= mask_PWM;                        //turn on motors
        SysTick_Wait1us(H);
        P2->OUT &= ~mask_PWM;                       //turn off motors
        SysTick_Wait1us(L);
    }

   //Once bot has driven forwards, turn off both motors and enter sleep mode.

    Motor_StopSimple();


}

// Drives both motors backward at duty (100 to 9900)
// Runs for time duration (units=10ms), and then stops
// Runs even if any bumper switch is active
// Returns after time*10ms

void Motor_BackwardSimple(uint16_t duty, uint32_t time){

    //P5.4 and P5.5 = direction
    //P2.6 and P2.7 = PWM
    //P3.6 and P3.7 = enable

    uint8_t mask_direction = 0x30;          //Declare masks for direction, PWM, enable.
    uint8_t mask_PWM = 0xC0;
    uint8_t mask_en = 0xC0;

    P5->OUT |= mask_direction;              //Set direction to backwards on both motors
    P3->OUT |= mask_en;                     //enable full power mode

    uint32_t H,L,i;                         //declare high and low integers, as well as for loop increment
    H = duty;
    L = 10000 - H;

    for(i = 0; i < time; i++) {          //Each for loop iteration should  take 10ms. Therefore, it should be of duration time.
        P2->OUT |= mask_PWM;                        //turn on motors
        SysTick_Wait1us(H);
        P2->OUT &= ~mask_PWM;                       //turn off motors
        SysTick_Wait1us(L);
    }

   //Once bot has driven backwards, turn off both motors and enter sleep mode.

   Motor_StopSimple();


}

// Drives just the left motor forward at duty (100 to 9900)
// Right motor is stopped (sleeping)
// Runs for time duration (units=10ms), and then stops
// Returns after time*10ms or if a bumper switch is hit

void Motor_LeftSimple(uint16_t duty, uint32_t time){

    //P5.4 and P5.5 = direction
    //P2.6 and P2.7 = PWM
    //P3.6 and P3.7 = enable

    uint8_t mask_direction_right = 0x20;          //Declare masks for direction, PWM, enable.
    uint8_t mask_direction_left = 0x10;
    //~0x20 = 1101 1111 --> Right motor backwards.
    //0x10  = 0001 0000 --> Left motor forwards.
    uint8_t mask_PWM = 0xC0;
    uint8_t mask_en = 0xC0;

    P5->OUT &= ~mask_direction_right;              //Set direction to backwards on right motor
    P5->OUT |= mask_direction_left;              //Set direction to backwards on right motor
    P3->OUT |= mask_en;                     //enable full power mode

    uint32_t H,L,i;                         //declare high and low integers, as well as for loop increment
    H = duty;
    L = 10000 - H;

    for(i = 0; i < time; i++) {          //Each for loop iteration should  take 10ms. Therefore, it should be of duration time.
        P2->OUT |= mask_PWM;                        //turn on motors
        SysTick_Wait1us(H);
        P2->OUT &= ~mask_PWM;                       //turn off motors
        SysTick_Wait1us(L);
    }

   //Once bot has turned left, turn off both motors and enter sleep mode.

    Motor_StopSimple();

}

// Drives just the right motor forward at duty (100 to 9900)
// Left motor is stopped (sleeping)
// Runs for time duration (units=10ms), and then stops
// Returns after time*10ms or if a bumper switch is hit

void Motor_RightSimple(uint16_t duty, uint32_t time){

    //P5.4 and P5.5 = direction
    //P2.6 and P2.7 = PWM
    //P3.6 and P3.7 = enable

    uint8_t mask_direction_left = 0x20;          //Declare masks for direction, PWM, enable.
    uint8_t mask_direction_right = 0x10;
    //0x20  = 0010 0000--> left motor backwards.
    //~0x10 = 1110 1111--> right motor forwards.
    uint8_t mask_PWM = 0xC0;
    uint8_t mask_en = 0xC0;

    P5->OUT &= ~mask_direction_right;              //Set direction to backwards on right motor
    P5->OUT |= mask_direction_left;              //Set direction to backwards on right motor
    P3->OUT |= mask_en;                     //enable full power mode

    uint32_t H,L,i;                         //declare high and low integers, as well as for loop increment
    H = duty;
    L = 10000 - H;

    for(i = 0; i < time; i++) {          //Each for loop iteration should  take 10ms. Therefore, it should be of duration time.
        P2->OUT |= mask_PWM;                        //turn on motors
        SysTick_Wait1us(H);
        P2->OUT &= ~mask_PWM;                       //turn off motors
        SysTick_Wait1us(L);
    }

   //Once bot has turned right, turn off both motors and enter sleep mode.

    Motor_StopSimple();

}
