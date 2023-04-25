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
#define cornerDistForward 1200
#define cornerDistSides 400
#define leftThresh 200

uint32_t divideValue = 1800;

uint32_t lastDistance = 1000;

uint32_t avg(uint32_t *array, int length)
{
  int i;
  uint32_t sum = 0;

  for(i=0; i<length; i=i+1)
  {
    sum = sum + array[i];
  }
  return (sum/length);
}

void setCenterSpeedRight(uint32_t *distances)
{
    uint16_t speedThresh = 2000;
    float newSpeed = targetSpeed * ((float)distances[1] / (float)divideValue);
    leftSpeed = newSpeed * (sin((float)distances[2] * 90.0 / divideValue * 0.0174533) + 0.2) * 1.1;
    if(leftSpeed < speedThresh) {
        leftSpeed = speedThresh;
    }
    rightSpeed = newSpeed * (sin((float)distances[0] * 90.0 / divideValue * 0.0174533) + 0.2);
    if(rightSpeed < speedThresh) {
        rightSpeed = speedThresh;
    }
}
void setCenterSpeedLeft(uint32_t *distances)
{
    uint16_t speedThresh = 2000;
    float newSpeed = targetSpeed * ((float)distances[1] / (float)divideValue);
    leftSpeed = newSpeed * (sin((float)distances[2] * 90.0 / divideValue * 0.0174533)+0.2);
    if(leftSpeed < speedThresh) {
        leftSpeed = speedThresh;
    }
    rightSpeed = newSpeed * (sin((float)distances[0] * 90.0 / divideValue * 0.0174533)+0.2) * 1.1;
    if(rightSpeed < speedThresh) {
       rightSpeed = speedThresh;
   }
}

uint32_t distances[3];
uint32_t left_distance_buffer_prev[3] = {800,800,800};
uint32_t left_distance_buffer[3];
int32_t leftDiff;


int main()
{

    int8_t i=0;
    int8_t j=0;

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


        getDist(distances);

        uint32_t hallwayWidth = distances[0] + distances[2];
        if(hallwayWidth < 1100) {
            divideValue = 600;
            setMaxDist(600);
        }
        else {
            divideValue = 1800;
            setMaxDist(1800);
        }

        if(i%3 == 0){
            left_distance_buffer[i/3] = distances[0];
        }

        if (i==6){
            leftDiff = avg(left_distance_buffer,3) - avg(left_distance_buffer_prev,3);
            for(j=0; j<3; j++){
                left_distance_buffer_prev[j] = left_distance_buffer[j];
            }
            i=-1;
        }

        if(distances[1] < cornerDistForward || (leftDiff > leftThresh && distances[2] > cornerDistSides)) {
            setCenterSpeedRight(distances);
            LaunchPad_Output(0x01);
            //Clock_Delay1ms(100);
        }
        else {
            setCenterSpeedLeft(distances);
            LaunchPad_Output(0x04);
        }

        setMotorSpeed(leftSpeed, rightSpeed);

        lastDistance = distances[0];
        i++;

    }

}

void PORT4_IRQHandler(void){            // Deal with Crashes

    uint8_t bsMask = 0xED;
    Clock_Delay1us(20);         // software debounce
    P4->IFG &= ~bsMask;         // acknowledge and clear flag
    P2->OUT ^= 0x02;             // toggle red LED on RGB LED
    setMotorSpeed(0,0);
    Clock_Delay1ms(1000);         // Must wait 1 second, as per project requirements
    LaunchPad_Output(0x02);
    uint8_t bumpInput = BumpInt_Read();

    //if((bumpInput &  )
    setMotorSpeed(-1000, -10000);    //back up right


    Clock_Delay1ms(250);

}

