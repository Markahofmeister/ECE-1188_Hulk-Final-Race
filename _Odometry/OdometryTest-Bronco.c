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
#include "..\inc\odometry.h"

extern int32_t MyX,MyY;               // position in 0.0001cm
extern int32_t MyTheta;               // direction units 2*pi/16384 radians (-pi to +pi)
extern enum RobotState Action;
uint32_t result;
int main(void){
  Clock_Init48MHz();  // maximum bus clock
  Bump_Init();        // RSLK bump switches
  UART0_Initprintf();    // Send data to PC, 115200 bps
  printf("\nOdometry Demo");
  LaunchPad_Init();      // initialize LaunchPad I/O
  Motor_Init();
  Motor_Stop(); Action = ISSTOPPED;
  Odometry_Init(0,0,NORTH); // facing North
  Tachometer_Init();
  TimerA1_Init(&UpdatePosition,20000); // every 40ms


  //SSD1306_Init(SSD1306_SWITCHCAPVCC);
  //SSD1306_Clear();
  //SSD1306_SetCursor(0,0); SSD1306_OutString("RSLK 1.1, Valvano");
  //SSD1306_SetCursor(0,1); SSD1306_OutString("Odometry test");
  //SSD1306_SetCursor(0,2); SSD1306_OutString("North at (0,0)");
  //SSD1306_SetCursor(0,4); SSD1306_OutString("Hit bump to start");
  printf("\nNorth at (0,0)");
  printf("\nHit bump to start");
  WaitUntilBumperTouched();
  while(1){
    Odometry_Init(0,0,NORTH);        Display_Serial(); // facing North, 90
    result = ForwardUntilY(400000);  Display_Serial(); // 0,40 cm
    result = SoftLeftUntilTh(WEST);  Display_Serial(); // 180 or -180
    result = ForwardUntilX(-400000); Display_Serial(); // -40,40 cm
    result = SoftLeftUntilTh(SOUTH); Display_Serial(); // -90
    result = ForwardUntilY(0);       Display_Serial(); // -40,0 cm
    result = SoftLeftUntilTh(EAST);  Display_Serial(); // 0
    result = ForwardUntilX(0);       Display_Serial(); // 0,0 cm
    result = SoftLeftUntilTh(NORTH); Display_Serial(); // 90
    StopUntilBumperTouched();
  }
}
