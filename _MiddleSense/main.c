#include <Dist.h>
#include <stdint.h>
#include <math.h>
#include "Clock.h"
#include "msp.h"
#include "pidController.h"
#include "odometry.h"
#include "BumpInt.h"
#include "LaunchPad.h"

int targetSpeed = 10000;
int leftSpeed = 0, rightSpeed = 0;
#define cornerDistForward 900
#define cornerDistSides 900
#define leftThresh 100
uint8_t lap = 1;
uint32_t lastDistance = 1000;

void setCenterSpeedRight(uint32_t *distances)
{
    int newSpeed = targetSpeed * distances[1] / 1000.0;
    leftSpeed = newSpeed *sin((float)distances[2] * 90.0 / 1000.0 * 0.0174533) * 1.1;
    rightSpeed = newSpeed *sin((float)distances[0] * 90.0 / 1000.0 * 0.0174533);
}
void setCenterSpeedLeft(uint32_t *distances)
{
    int newSpeed = targetSpeed * distances[1] / 1000.0;
    leftSpeed = newSpeed *sin((float)distances[2] * 90.0 / 1000.0 * 0.0174533);
    rightSpeed = newSpeed *sin((float)distances[0] * 90.0 / 1000.0 * 0.0174533) * 1.1;
}

int main()
{
    uint32_t distances[3];
    distances[0] = 0;
    distances[1] = 0;
    distances[2] = 0;

    BumpInt_Init();
    LaunchPad_Init();
    Clock_Init48MHz();
    motorPWMInit(15000, 0, 0);
    EnableInterrupts();
    Dist_Init();
    Odometry_Init(0, 0, 0);
    while(1)
    {


        /*switch(lap) {

            case 1:

                getDist(distances);
                setCenterSpeedRight(distances);
                setMotorSpeed(leftSpeed, rightSpeed);

                if(distances[1] < cornerDist && distances[2] > cornerDist) {         //If we've detected a corner, turn 90 degrees
                     LaunchPad_Output(0x01);
                     lap++;
                     Clock_Delay1ms(1000);
                }
            break;
            case 2:

                getDist(distances);
                setCenterSpeedLeft(distances);
                setMotorSpeed(leftSpeed, rightSpeed);
                if(distances[1] < cornerDist && distances[0] > cornerDist) {         //If we've detected a corner, turn 90 degrees
                     LaunchPad_Output(0x02);
                     lap++;
                     Clock_Delay1ms(1000);
                }

            break;
            case 3:

                getDist(distances);
                setCenterSpeedLeft(distances);
                setMotorSpeed(leftSpeed, rightSpeed);
                if(distances[1] < cornerDist && distances[0] > cornerDist) {         //If we've detected a corner, turn 90 degrees
                     LaunchPad_Output(0x04);
                     lap++;
                     Clock_Delay1ms(1000);
                }

            break;

        }*/

        getDist(distances);
        uint32_t leftDiff = distances[0] - lastDistance;
        if(distances[1] < cornerDistForward || (leftDiff > leftThresh && distances[2] > cornerDistSides)) {
            setCenterSpeedRight(distances);
            LaunchPad_Output(0x01);
        }
        else {
            setCenterSpeedLeft(distances);
            LaunchPad_Output(0x04);
        }

        setMotorSpeed(leftSpeed, rightSpeed);

        lastDistance = distances[0];

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

