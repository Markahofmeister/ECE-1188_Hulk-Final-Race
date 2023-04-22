// OdometryTestmain.c
// Tests Odometry.c
// This system will need calibration to your particular robot, adjust N D W C for your robot
// Runs on the MSP432
// Daniel and Jonathan Valvano
// February 10, 2019

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
#include "msp.h"
#include <stdio.h>
#include "..\inc\UART0.h"
#include "..\inc\LaunchPad.h"
#include "..\inc\bump.h"
#include "..\inc\clock.h"
#include "..\inc\TimerA1.h"
#include "..\inc\Motor.h"
#include "..\inc\Tachometer.h"
//#include "..\inc\SSD1306.h"
#include "..\inc\odometry.h"
#include "..\inc\blinker.h"

extern int32_t MyX,MyY;               // position in 0.0001cm
extern int32_t MyTheta;               // direction units 2*pi/16384 radians (-pi to +pi)
extern enum RobotState Action;
uint32_t result;
int main(void){
  Clock_Init48MHz();  // maximum bus clock
  Bump_Init();        // RSLK bump switches
// UCA0RXD (VCP receive) connected to P1.2
// UCA0TXD (VCP transmit) connected to P1.3
  UART0_Initprintf();    // Send data to PC, 115200 bps
  LaunchPad_Init();      // initialize LaunchPad I/O
  Motor_Init();
  Motor_Stop(); Action = ISSTOPPED;
  Blinker_Init();
  Odometry_Init(0,0,NORTH); // facing North
  Tachometer_Init();
  TimerA1_Init(&UpdatePosition,20000); // every 40ms
  //SSD1306_Init(SSD1306_SWITCHCAPVCC);
  //SSD1306_Clear();
  //SSD1306_SetCursor(0,0); SSD1306_OutString("RSLK 1.1, Valvano");
  //SSD1306_SetCursor(0,1); SSD1306_OutString("Odometry test");
  //SSD1306_SetCursor(0,2); SSD1306_OutString("North at (0,0)");
  //SSD1306_SetCursor(0,4); SSD1306_OutString("Hit bump to start");
  WaitUntilBumperTouched();
  while(1){
    Odometry_Init(0,0,NORTH);        Display(); // facing North, 90
    result = ForwardUntilY(400000);  Display(); // 0,40 cm
    result = SoftLeftUntilTh(WEST);  Display(); // 180 or -180
    result = ForwardUntilX(-400000); Display(); // -40,40 cm
    result = SoftLeftUntilTh(SOUTH); Display(); // -90
    result = ForwardUntilY(0);       Display(); // -40,0 cm
    result = SoftLeftUntilTh(EAST);  Display(); // 0
    result = ForwardUntilX(0);       Display(); // 0,0 cm
    result = SoftLeftUntilTh(NORTH); Display(); // 90
    StopUntilBumperTouched();
  }
}
