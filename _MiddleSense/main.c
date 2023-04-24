#include <Dist.h>
#include <stdint.h>
#include <math.h>
#include "msp.h"
#include "pidController.h"
#include "odometry.h"
#include "BumpInt.h"
#include "LaunchPad.h"

int targetSpeed = 10000;
int leftSpeed = 0, rightSpeed = 0;

void setCenterSpeed(uint32_t *distances)
{
    int newSpeed = targetSpeed * distances[1] / 1000.0;
    leftSpeed = newSpeed *sin((float)distances[2] * 90.0 / 1000.0 * 0.0174533) * 1.1;
    rightSpeed = newSpeed *sin((float)distances[0] * 90.0 / 1000.0 * 0.0174533);
}

int main()
{
    uint32_t distances[3];

    BumpInt_Init();
    LaunchPad_Init();
    Clock_Init48MHz();
    motorPWMInit(15000, 0, 0);
    EnableInterrupts();
    Dist_Init();
    Odometry_Init(0, 0, 0);
    while(1)
    {
        getDist(distances);
        setCenterSpeed(distances);
        setMotorSpeed(leftSpeed, rightSpeed);

    }

}

void PORT4_IRQHandler(void){            // Deal with Crashes

    uint8_t bsMask = 0xED;
    Clock_Delay1us(10);         // software debounce
    P4->IFG &= ~bsMask;         // acknowledge and clear flag
    P2->OUT ^= 0x02;             // toggle red LED on RGB LED
    setMotorSpeed(0,0);
    Clock_Delay1ms(1000);         // Must wait 1 second, as per project requirements
    LaunchPad_Output(0x02);

    while(1);               //Remove and implement crash recovery

    /*                            // How to deal with a crash?
    Motor_Backward(2500,2500);
    Clock_Delay1ms(1000);
    */
}

