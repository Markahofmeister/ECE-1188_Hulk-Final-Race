#include <CustomMotor.h>

#define LEFT_DIRECTION     0x10  //P5.4
#define LEFT_EN            0x80  //P2.7
#define LEFT_SLEEP         0x20  //P3.5
#define RIGHT_DIRECTION    0x20  //P5.5
#define RIGHT_EN           0x40  //P2.6
#define RIGHT_SLEEP        0x40  //P3.6

void Motor_Init(void){
// Initializes the 6 GPIO lines and puts driver to sleep
// Returns right away
// initialize P5.4 and P5.5 and make them outputs

  // write this as part of Lab 12

    //P5 Init
    P5->SEL0 &= ~(LEFT_DIRECTION + RIGHT_DIRECTION);
    P5->SEL1 &= ~(LEFT_DIRECTION + RIGHT_DIRECTION);
    P5->DS |= (LEFT_DIRECTION + RIGHT_DIRECTION);
    P5->DIR |= (LEFT_DIRECTION + RIGHT_DIRECTION);
    P5->OUT &= ~(LEFT_DIRECTION + RIGHT_DIRECTION);

    //P3 Init
    P3->SEL0 &= ~(LEFT_SLEEP + RIGHT_SLEEP);
    P3->SEL1 &= ~(LEFT_SLEEP + RIGHT_SLEEP);
    P3->DS |= (LEFT_SLEEP + RIGHT_SLEEP);
    P3->DIR |= (LEFT_SLEEP + RIGHT_SLEEP);
    P3->OUT |= (LEFT_SLEEP + RIGHT_SLEEP);

}

void motorPWMInit(uint16_t period, uint16_t duty1, uint16_t duty2)
{

    //Initialize the motor ports
    Motor_Init();

    //Set up P2 ports for timer input
    P2->DIR |= (LEFT_EN + RIGHT_EN);
    P2->SEL0 |= (LEFT_EN + RIGHT_EN);
    P2->SEL1 &= ~(LEFT_EN + RIGHT_EN);

    //Set up the timers with the perid and duty cycle
    TIMER_A0->CCTL[0] = 0x0080;
    TIMER_A0->CCR[0] = period;
    TIMER_A0->EX0 = 0x0000;

    TIMER_A0->CCTL[3] = 0x0040;
    TIMER_A0->CCR[3] = duty2;

    TIMER_A0->CCTL[4] = 0x0040;
    TIMER_A0->CCR[4] = duty1;
    TIMER_A0->CTL = 0x02F0;

}

void PWM_LeftMotor(uint16_t duty)
{
    //Motor go forward with new duty cycle
    P5->OUT &= ~LEFT_DIRECTION;
    TIMER_A0->CCR[4] = duty;
}

void PWM_RightMotor(uint16_t duty)
{
    //Motor go forward with new duty cycle
    P5->OUT &= ~RIGHT_DIRECTION;
    TIMER_A0->CCR[3] = duty;
}

void PWM_LeftMotorBackwards(uint16_t duty)
{
    //Motor go backward with new duty cycle
    P5->OUT |= LEFT_DIRECTION;
    TIMER_A0->CCR[4] = duty;
}
void PWM_RightMotorBackwards(uint16_t duty)
{
    //Motor go backward with new duty cycle
    P5->OUT |= RIGHT_DIRECTION;
    TIMER_A0->CCR[3] = duty;
}

void setMotorSpeed(int leftDuty, int rightDuty)
{
    if(leftDuty >= 0)
        PWM_LeftMotor(leftDuty);
    else
        PWM_LeftMotorBackwards(-1 * leftDuty);
    if(rightDuty >= 0)
        PWM_RightMotor(rightDuty);
    else
        PWM_RightMotorBackwards(-1 * rightDuty);
}
