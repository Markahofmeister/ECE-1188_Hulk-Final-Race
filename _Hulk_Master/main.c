/*
 * main file of Team Hulk's final project
 * Runs on MSP432
 * Authors: Maya Hegde, Mark Hofmeister, Bronco York
 * created 4/22/2023
 */

#include <stdint.h>
#include "msp.h"
#include "Clock.h"
#include "I2CB1.h"
#include "CortexM.h"
#include "LPF.h"
#include "opt3101.h"
#include "LaunchPad.h"
#include "BumpInt.h"
#include "Motor.h"
#include "UART0.h"
#include "FFT.h"


// this batch configures for UART link to PC
void UartSetCur(uint8_t newX, uint8_t newY){
  if(newX == 6){
    UART0_OutString("\n\rTxChannel= ");
    UART0_OutUDec(newY-1);
    UART0_OutString(" Distance= ");
  }else{
    UART0_OutString("\n\r");
  }
}
void UartClear(void){
    UART0_OutString("\n\r");
};
#define Init UART0_Init
#define Clear UartClear
#define SetCursor UartSetCur
#define OutString UART0_OutString
#define OutChar UART0_OutChar
#define OutUDec UART0_OutUDec
#define OutSDec UART0_OutSDec


uint32_t Distances[3];
uint32_t FilteredDistances[3];
uint32_t Amplitudes[3];
uint32_t Noises[3];
uint32_t TxChannel;
uint32_t StartTime;
uint32_t TimeToConvert; // in msec
bool pollDistanceSensor(void){
  if(OPT3101_CheckDistanceSensor()){
    TxChannel = OPT3101_GetMeasurement(Distances,Amplitudes);
    return true;
  }
  return false;
}


/*
 * The below two functions filter the raw left and right opt3101 data into tangential distances from the wall.
 *
 */

// calibrated for 500mm track
// right is raw sensor data from right sensor
// return calibrated distance from center of Robot to right wall
int32_t Right(int32_t right){
  return  (right*(59*right + 7305) + 2348974)/32768;
}
// left is raw sensor data from left sensor
// return calibrated distance from center of Robot to left wall
int32_t Left(int32_t left){
  return (1247*left)/2048 + 22;
}

// assumes track is 500mm
int32_t Mode=1; // 0 stop, 1 run
int32_t Error;
int32_t Ki=1;  // integral controller gain
int32_t Kp=2;  // proportional controller gain //was 4
int32_t UR, UL;  // PWM duty 0 to 14,998

#define TOOCLOSE 800 //was 200
#define DESIRED 1000 //was 250
int32_t SetPoint = 1000; // mm //was 250
int32_t LeftDistance,CenterDistance,RightDistance; // mm
#define TOOFAR  // was 400

#define PWMNOMINAL 2500 // was 2500
#define SWING 2000 //was 1000
#define PWMMIN (PWMNOMINAL-SWING)
#define PWMMAX (PWMNOMINAL+SWING)
void Controller(void){ // runs at 100 Hz
  if(Mode){
    if((LeftDistance>DESIRED)&&(RightDistance>DESIRED)){
      SetPoint = (LeftDistance+RightDistance)/2;
    }else{
      SetPoint = DESIRED;
    }
    if(LeftDistance < RightDistance ){
      Error = LeftDistance-SetPoint;
    }else {
      Error = SetPoint-RightDistance;
    }
 //   UR = UR + Ki*Error;      // adjust right motor
    UR = PWMNOMINAL+Kp*Error; // proportional control
    UL = PWMNOMINAL-Kp*Error; // proportional control
    if(UR < (PWMNOMINAL-SWING)) UR = PWMNOMINAL-SWING; // 3,000 to 7,000
    if(UR > (PWMNOMINAL+SWING)) UR = PWMNOMINAL+SWING;
    if(UL < (PWMNOMINAL-SWING)) UL = PWMNOMINAL-SWING; // 3,000 to 7,000
    if(UL > (PWMNOMINAL+SWING)) UL = PWMNOMINAL+SWING;
    Motor_Forward(UL,UR);

  }
}

void Controller_Right(void){ // runs at 100 Hz
  if(Mode){
    if((RightDistance>DESIRED)){
      SetPoint = (RightDistance)/2;
    }else{
      SetPoint = DESIRED;
    }
    /*if(LeftDistance < RightDistance ){
      Error = LeftDistance-SetPoint;
    }else {
      Error = SetPoint-RightDistance;
    }*/

    Error = SetPoint-RightDistance;
    //UR = UR + Ki*Error;      // adjust right motor
    UR = PWMNOMINAL+Kp*Error; // proportional control
    UR = UR + Ki*Error;      // adjust right motor
    UL = PWMNOMINAL-Kp*Error; // proportional control
    if(UR < (PWMNOMINAL-SWING)) UR = PWMNOMINAL-SWING; // 3,000 to 7,000
    if(UR > (PWMNOMINAL+SWING)) UR = PWMNOMINAL+SWING;
    if(UL < (PWMNOMINAL-SWING)) UL = PWMNOMINAL-SWING; // 3,000 to 7,000
    if(UL > (PWMNOMINAL+SWING)) UL = PWMNOMINAL+SWING;

    //turns left if the center measurement and right measurement is small enough that we will hit the wall if we don't turn
    if((RightDistance<250) && (CenterDistance <250)){
        UL = 0;
        UR = PWMNOMINAL;
    }

    Motor_Forward(UL,UR);

  }
}

/*void Pause(void){int i;
  while(BumpInt_Read()){ // wait for release
    Clock_Delay1ms(200); LaunchPad_Output(0); // off
    Clock_Delay1ms(200); LaunchPad_Output(1); // red
  }
  while(BumpInt_Read()==0){// wait for touch
    Clock_Delay1ms(100); LaunchPad_Output(0); // off
    Clock_Delay1ms(100); LaunchPad_Output(3); // red/green
  }
  while(BumpInt_Read()){ // wait for release
    Clock_Delay1ms(100); LaunchPad_Output(0); // off
    Clock_Delay1ms(100); LaunchPad_Output(4); // blue
  }
  for(i=1000;i>100;i=i-200){
    Clock_Delay1ms(i); LaunchPad_Output(0); // off
    Clock_Delay1ms(i); LaunchPad_Output(2); // green
  }
  // restart Jacki
  UR = UL = PWMNOMINAL;    // reset parameters
  Mode = 1;

}*/

// triggered on touch, falling edge
// Indicates a crash
void PORT4_IRQHandler(void){

    uint8_t bsMask = 0xED;
    Clock_Delay1us(10);         // software debounce
    P4->IFG &= ~bsMask;         // acknowledge and clear flag
    P2->OUT ^= 0x02;             // toggle red LED on RGB LED
    Motor_Stop();
    Clock_Delay1ms(1000);         // Must wait 1 second, as per project requirements

    while(1);               //Remove and implement crash recovery

    /*                            // How to deal with a crash?
    Motor_Backward(2500,2500);
    Clock_Delay1ms(1000);
    */

}

// Function to organize UART initialization, clearing, and printing of Intitialization data
void UART_Init_Print(void) {
      Init();                   // UART baud rate = 115,200
      Clear();
      OutString("OPT3101");
      SetCursor(0, 1);
      OutString("L=");
      SetCursor(0, 2);
      OutString("C=");
      SetCursor(0, 3);
      OutString("R=");
      SetCursor(0, 4);
      OutString("Wall follow");
      SetCursor(0, 5);
      OutString("SP=");
      SetCursor(0, 6);
      OutString("Er=");
      SetCursor(0, 7);
      OutString("U =");
}

/*
 * The parameters are used for a 500mm width track, so this must be changed/tunes
 */
void main(void){ // wallFollow wall following implementation

  int i = 0;                // Used for...what?
  uint32_t channel = 1;     // Variable to hold which channel of the optical sensor we're reading

  Motor_Init();             //Initializations for motor, clock,
  DisableInterrupts();
  Clock_Init48MHz();
  BumpInt_Init();
  LaunchPad_Init(); // built-in switches and LEDs

  Motor_Stop(); // initialize and stop

  Mode = 1;
  I2CB1_Init(30);           // I2C baud rate = 400kHz
  UART_Init_Print();        // UART baud rate = 115,200

  OPT3101_Init();           // Setup OPT3101
  OPT3101_Setup();
  OPT3101_CalibrateInternalCrosstalk();
  OPT3101_ArmInterrupts(&TxChannel, Distances, Amplitudes);
  TxChannel = 3;            //Begin measurement of channel 3
  OPT3101_StartMeasurementChannel(channel);

  LPF_Init(100,8);          // Setup FIR Filter
  LPF_Init2(100,8);
  LPF_Init3(100,8);

  UR = UL = PWMNOMINAL;     //initial power

  EnableInterrupts();

  // Begin Main loop
  while(1){
    if(TxChannel <= 2){ // 0,1,2 means new data
      if(TxChannel==0){
        if(Amplitudes[0] > 1000){
          LeftDistance = FilteredDistances[0] = Left(LPF_Calc(Distances[0]));
        }else{
          LeftDistance = FilteredDistances[0] = 500;
        }
      }else if(TxChannel==1){
        if(Amplitudes[1] > 1000){
          CenterDistance = FilteredDistances[1] = LPF_Calc2(Distances[1]);
        }else{
          CenterDistance = FilteredDistances[1] = 500;
        }
      }else {
        if(Amplitudes[2] > 1000){
          RightDistance = FilteredDistances[2] = Right(LPF_Calc3(Distances[2]));
        }else{
          RightDistance = FilteredDistances[2] = 500;
        }
      }
      SetCursor(2, TxChannel+1);
      OutUDec(FilteredDistances[TxChannel]); OutChar(','); OutUDec(Amplitudes[TxChannel]);
      TxChannel = 3; // 3 means no data
      channel = (channel+1)%3;
      OPT3101_StartMeasurementChannel(channel);
      i = i + 1;
    }
    //Controller_Right();
    Controller();
    if(i >= 100){
      i = 0;
      SetCursor(3, 5);
      OutUDec(SetPoint);
      SetCursor(3, 6);
      OutSDec(Error);
      SetCursor(3, 7);
      OutUDec(UL); OutChar(','); OutUDec(UR);
    }

    WaitForInterrupt();
  }     //end main loop
}

