// Lab21_OPT3101_TestMain.c
//*****************************************************************************
// Lab21 main for Robot with OPT3101 time of flight sensor
// MSP432 with RSLK Max and OPT3101
// Daniel and Jonathan Valvano
// July 7, 2020
//****************************************************************************
/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2020
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2020, Jonathan Valvano, All rights reserved.

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
// see opt3101.h for OPT3101 hardware connections
// see Nokia5110.h LCD hardware connections
// see SSD1306.h for OLED hardware connections
// see UART0.h for UART0 hardware connections

#include <stdint.h>
#include "msp.h"
#include "../inc/Clock.h"
#include "../inc/I2CB1.h"
#include "../inc/CortexM.h"
#include "../inc/LPF.h"
#include "../inc/opt3101.h"
#include "../inc/LaunchPad.h"
#include "../inc/Bump.h"
#include "../inc/Motor.h"
#include "../inc/UART0.h"
#include "../inc/SSD1306.h"
#include "../inc/FFT.h"
// Select one of the following three output possibilities
// define USENOKIA
#define USEOLED 1
//#define USEUART

#ifdef USENOKIA
// this batch configures for LCD
#include "../inc/Nokia5110.h"
#define Init Nokia5110_Init
#define Clear Nokia5110_Clear
#define SetCursor Nokia5110_SetCursor
#define OutString Nokia5110_OutString
#define OutChar Nokia5110_OutChar
#define OutUDec Nokia5110_OutUDec
#define OutSDec Nokia5110_OutSDec
#endif

#ifdef USEOLED
// this batch configures for OLED

void OLEDinit(void){SSD1306_Init(SSD1306_SWITCHCAPVCC);}
#define Init OLEDinit
#define Clear SSD1306_Clear
#define SetCursor SSD1306_SetCursor
#define OutChar SSD1306_OutChar
#define OutString SSD1306_OutString
#define OutUDec SSD1306_OutUDec
#define OutSDec SSD1306_OutSDec
#endif

#ifdef USEUART
// this batch configures for UART link to PC
#include "../inc/UART0.h"
void UartSetCur(uint8_t newX, uint8_t newY){
  if(newX == 6){
    UART0_OutString("\n\rTxChannel= ");
    UART0_OutUDec(newY-1);
    UART0_OutString(" Distance= ");
  }else{
    UART0_OutString("\n\r");
  }
}
void UartClear(void){UART0_OutString("\n\r");};
#define Init UART0_Init
#define Clear UartClear
#define SetCursor UartSetCur
#define OutString UART0_OutString
#define OutChar UART0_OutChar
#define OutUDec UART0_OutUDec
#define OutSDec UART0_OutSDec
#endif


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
// SysTick is just for debug profile, it can be removed
void main1(void){ // interrupt implementation
  int i = 0;
  uint32_t channel = 1;
  DisableInterrupts();
  Clock_Init48MHz();
  SysTick->LOAD = 0x00FFFFFF;           // maximum reload value
  SysTick->CTRL = 0x00000005;           // enable SysTick with no interrupts
  I2CB1_Init(30); // baud rate = 12MHz/30=400kHz
  Init();
  Clear();
  OutString("OPT3101");
  SetCursor(0, 1);
  OutString("Left =");
  SetCursor(0, 2);
  OutString("Centr=");
  SetCursor(0, 3);
  OutString("Right=");
  SetCursor(0, 4);
  OutString("Interrupts");
  SetCursor(0, 5);
  OutString("NL=");
  SetCursor(0, 6);
  OutString("NC=");
  SetCursor(0, 7);
  OutString("NR=");
  OPT3101_Init();
  OPT3101_Setup();
  OPT3101_CalibrateInternalCrosstalk();
  OPT3101_ArmInterrupts(&TxChannel, Distances, Amplitudes);
  StartTime = SysTick->VAL;
  TxChannel = 3;
  OPT3101_StartMeasurementChannel(channel);
  LPF_Init(100,32);
  LPF_Init2(100,32);
  LPF_Init3(100,32);
  EnableInterrupts();
  while(1){
    if(TxChannel <= 2){ // 0,1,2 means new data
      TimeToConvert = ((StartTime-SysTick->VAL)&0x00FFFFFF)/48000; // msec
      StartTime = SysTick->VAL;
      if(TxChannel==0){
        FilteredDistances[0] = LPF_Calc(Distances[0]);
      }else if(TxChannel==1){
        FilteredDistances[1] = LPF_Calc2(Distances[1]);
      }else {
        FilteredDistances[2] = LPF_Calc3(Distances[2]);
      }
      SetCursor(6, TxChannel+1);
      OutUDec(FilteredDistances[TxChannel]);
      TxChannel = 3; // 3 means no data
      channel = (channel+1)%3;
      OPT3101_StartMeasurementChannel(channel);
      i = i + 1;
    }
    if(i >= 300){
      i = 0;
      SetCursor(3, 5);
      OutUDec((uint16_t)Noise());  OutChar(','); OutUDec(Amplitudes[0]);
      SetCursor(3, 6);
      OutUDec((uint16_t)Noise2()); OutChar(','); OutUDec(Amplitudes[1]);
      SetCursor(3, 7);
      OutUDec((uint16_t)Noise3()); OutChar(','); OutUDec(Amplitudes[2]);
    }
    WaitForInterrupt();
  }
}

void main2(void){ // busy-wait implementation
  uint32_t channel = 1;
  Clock_Init48MHz();
  SysTick->LOAD = 0x00FFFFFF;           // maximum reload value
  SysTick->CTRL = 0x00000005;           // enable SysTick with no interrupts
  I2CB1_Init(30); // baud rate = 12MHz/30=400kHz
  Init();
  Clear();
  OutString("OPT3101");
  SetCursor(0, 1);
  OutString("Left =");
  SetCursor(0, 2);
  OutString("Centr=");
  SetCursor(0, 3);
  OutString("Right=");
  SetCursor(0, 4);
  OutString("Busy-wait");
  OPT3101_Init();
  OPT3101_Setup();
  OPT3101_CalibrateInternalCrosstalk();
  OPT3101_StartMeasurementChannel(channel);
  StartTime = SysTick->VAL;
  while(1){
    if(pollDistanceSensor()){
      TimeToConvert = ((StartTime-SysTick->VAL)&0x00FFFFFF)/48000; // msec
      if(TxChannel <= 2){
        SetCursor(6, TxChannel+1);
        OutUDec(Distances[TxChannel]);
      }
      channel = (channel+1)%3;
      OPT3101_StartMeasurementChannel(channel);
      StartTime = SysTick->VAL;
    }
  }
}

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

void mainUNO(void){ // main3interrupt implementation
  int i = 0;
  uint32_t channel = 1;
  DisableInterrupts();
  Clock_Init48MHz();
  SysTick->LOAD = 0x00FFFFFF;           // maximum reload value
  SysTick->CTRL = 0x00000005;           // enable SysTick with no interrupts
  I2CB1_Init(30); // baud rate = 12MHz/30=400kHz
  Init();
  Clear();
  OutString("OPT3101");
#ifndef USEUART
  SetCursor(0, 1);
  OutString("Left =");
  SetCursor(0, 2);
  OutString("Centr=");
  SetCursor(0, 3);
  OutString("Right=");
  SetCursor(0, 4);
  OutString("Interrupts");
  SetCursor(0, 5);
  OutString("NL=");
  SetCursor(0, 6);
  OutString("NC=");
  SetCursor(0, 7);
  OutString("NR=");
#endif
  OPT3101_Init();
  OPT3101_Setup();
  OPT3101_CalibrateInternalCrosstalk();
  OPT3101_ArmInterrupts(&TxChannel, Distances, Amplitudes);
  StartTime = SysTick->VAL;
  TxChannel = 3;
  OPT3101_StartMeasurementChannel(channel);
  LPF_Init(100,8);
  LPF_Init2(100,8);
  LPF_Init3(100,8);
#ifdef USEUART
  OutString("Digital filter size=8\n");
#endif
  EnableInterrupts();
  while(1){
    if(TxChannel <= 2){ // 0,1,2 means new data
      if(TxChannel==0){
        FilteredDistances[0] = Left(LPF_Calc(Distances[0]));
      }else if(TxChannel==1){
        FilteredDistances[1] = LPF_Calc2(Distances[1]);
      }else {
        FilteredDistances[2] = Right(LPF_Calc3(Distances[2]));
      }
#ifndef USEUART
      SetCursor(6, TxChannel+1);
      OutUDec(FilteredDistances[TxChannel]);
#endif
      TxChannel = 3; // 3 means no data
      channel = (channel+1)%3;
      OPT3101_StartMeasurementChannel(channel);
      i = i + 1;
    }
    if(i >= 90){ // every 3 seconds
      i = 0;
      Noises[0] = Noise();
      Noises[1] = Noise2();
      Noises[2] = Noise3();
#ifdef USEUART
      OutChar('\n');
      SetCursor(0, 1);
      OutString("Left ="); OutUDec(Distances[0]); OutChar(','); OutUDec(FilteredDistances[0]);
      SetCursor(0, 2);
      OutString("Centr="); OutUDec(Distances[1]); OutChar(','); OutUDec(FilteredDistances[1]);
      SetCursor(0, 3);
      OutString("Right="); OutUDec(Distances[2]); OutChar(',');  OutUDec(FilteredDistances[2]);
      SetCursor(3, 5);
      OutString("NL="); OutUDec((uint16_t)Noises[0]);  OutChar(','); OutUDec(Amplitudes[0]);
      SetCursor(3, 6);
      OutString("NC="); OutUDec((uint16_t)Noises[1]); OutChar(','); OutUDec(Amplitudes[1]);
      SetCursor(3, 7);
      OutString("NR="); OutUDec((uint16_t)Noises[2]); OutChar(','); OutUDec(Amplitudes[2]);
#else
      SetCursor(3, 5);
      OutUDec((uint16_t)Noises[0]);  OutChar(','); OutUDec(Amplitudes[0]);
      SetCursor(3, 6);
      OutUDec((uint16_t)Noises[1]); OutChar(','); OutUDec(Amplitudes[1]);
      SetCursor(3, 7);
      OutUDec((uint16_t)Noises[2]); OutChar(','); OutUDec(Amplitudes[2]);
#endif
    }
    WaitForInterrupt();
  }
}


#define N 1024
uint32_t Data[N];
#define M 1024
uint16_t Histogram[M];
uint32_t Sum;      // sum of data
uint32_t Sum2;     // sum of (data-average)^2
uint32_t Average;  // average of data = sum/N
uint32_t Variance; // =sum2/(N-1)
uint32_t Sigma;    // standard deviation = sqrt(Variance)

int Program21_1(void){ //Program21_1(void){ // example program 21.1, RSLK1.1
  int32_t n; uint32_t min,max,s=1;
  int i = 0;
  uint32_t channel = 1;
  DisableInterrupts();
  Clock_Init48MHz();
  SysTick->LOAD = 0x00FFFFFF;           // maximum reload value
  SysTick->CTRL = 0x00000005;           // enable SysTick with no interrupts
  I2CB1_Init(30); // baud rate = 12MHz/30=400kHz

  OPT3101_Init();
  OPT3101_Setup();
  OPT3101_CalibrateInternalCrosstalk();
  OPT3101_ArmInterrupts(&TxChannel, Distances, Amplitudes);
  StartTime = SysTick->VAL;
  TxChannel = 3;
  OPT3101_StartMeasurementChannel(channel);
  LPF_Init(200,256);
  LPF_Init2(200,256);
  LPF_Init3(200,256);
  EnableInterrupts();
  UART0_Init();               // initialize UART0 115,200 baud rate
  LaunchPad_Init();
  UART0_OutString("Program 21.1 FIR filter test\nValvano June 2020, RSLK1.1\n10Hz sampling\nConnect OPT3101 to RSLK\n");
  EnableInterrupts();
  while(1){
    UART0_OutString("\nOPT3101 resolution test\n ");
    UART0_OutString("Prime filter \n");
    LPF_Init(Distances[0],s);
    for(n=0; n<256; n++){ // prime filter
      if(TxChannel <= 2){ // 0,1,2 means new data
        if(TxChannel==0){
          FilteredDistances[0] = Left(LPF_Calc(Distances[0]));
        }else if(TxChannel==1){
          FilteredDistances[1] = LPF_Calc2(Distances[1]);
        }else {
          FilteredDistances[2] = Right(LPF_Calc3(Distances[2]));
        }
        TxChannel = 3; // 3 means no data
        channel = (channel+1)%3;
        OPT3101_StartMeasurementChannel(channel);
        i = i + 1;
      }
    }
    UART0_OutString("Collect "); UART0_OutUDec(N); UART0_OutString(" samples. ");
    UART0_OutUDec(s); UART0_OutString("-point average\n");
    Sum = Sum2 = 0;
    for(n=0; n<N; ){
      if(TxChannel <= 2){ // 0,1,2 means new data
        if(TxChannel==0){
          FilteredDistances[0] = Left(LPF_Calc(Distances[0]));
          Sum = Sum+FilteredDistances[0];    // distance in mm
          Data[n] = FilteredDistances[0];
          n++;
        }else if(TxChannel==1){
          FilteredDistances[1] = LPF_Calc2(Distances[1]);
        }else {
          FilteredDistances[2] = Right(LPF_Calc3(Distances[2]));
        }
        TxChannel = 3; // 3 means no data
        channel = (channel+1)%3;
        OPT3101_StartMeasurementChannel(channel);
        i = i + 1;
      }
    }
    Average = Sum/N;
    for(n=0; n<N; n++){
      Sum2 = Sum2+(Data[n]-Average)*(Data[n]-Average); // 20bits*100 = 27 bits
    }
    Variance = (100*Sum2)/(N-1);
    Sigma = sqrt(Variance);

    min = 16384; max = 0;
    for(n=0; n<N; n++){
      if(Data[n] < min)
        min = Data[n]; // smallest
      if(Data[n] > max)
        max = Data[n]; // largest
      }
    for(n=0;n<M;n++){
      Histogram[n] = 0;
    }
    for(n=0; n<N; n++){
      if((Data[n]>=min)&&(Data[n]<min+M)){
        Histogram[Data[n]-min]++;
      }
    }
    UART0_OutString("Probability Mass Function (PMF)\n Data  Count\n");
    for(n=0;n<M;n++){
      if(Histogram[n]){
        UART0_OutUDec5(n+min); UART0_OutString("  \t");UART0_OutUDec(Histogram[n]);UART0_OutChar('\n');
      }
    }

    UART0_OutString("FIR filter "); UART0_OutUDec(s); UART0_OutString("-point average\n");
    UART0_OutString("Average      ="); UART0_OutUDec(Average); UART0_OutChar('\n');
    UART0_OutString("Range        ="); UART0_OutUDec(max-min); UART0_OutChar('\n');
    UART0_OutString("Variance     ="); UART0_OutUFix2(Variance); UART0_OutChar('\n');
    UART0_OutString("Sigma        ="); UART0_OutUFix1(Sigma); UART0_OutChar('\n');
    UART0_OutString("Average/Sigma="); UART0_OutUFix1((100*Average)/Sigma); UART0_OutChar('\n');

//    while(LaunchPad_Input()==0x00){}; // wait for touch
//    while(LaunchPad_Input()!=0x00){}; // wait for release
    if(s<256) s = s*2; // 1,2,4,8,16,32,64,128,256...
    else s = 1;
  }
}
// assumes track is 500mm
int32_t Mode=0; // 0 stop, 1 run
int32_t Error;
int32_t Ki=1;  // integral controller gain
int32_t Kp=4;  // proportional controller gain //was 4
int32_t UR, UL;  // PWM duty 0 to 14,998

#define TOOCLOSE 200 //was 200
#define DESIRED 250 //was 250
int32_t SetPoint = 250; // mm //was 250
int32_t LeftDistance,CenterDistance,RightDistance; // mm
#define TOOFAR 400 // was 400

#define PWMNOMINAL 5000 // was 2500
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

void Pause(void){int i;
  while(Bump_Read()){ // wait for release
    Clock_Delay1ms(200); LaunchPad_Output(0); // off
    Clock_Delay1ms(200); LaunchPad_Output(1); // red
  }
  while(Bump_Read()==0){// wait for touch
    Clock_Delay1ms(100); LaunchPad_Output(0); // off
    Clock_Delay1ms(100); LaunchPad_Output(3); // red/green
  }
  while(Bump_Read()){ // wait for release
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

}

void main(void){ // wallFollow wall following implementation
  int i = 0;
  uint32_t channel = 1;
  DisableInterrupts();
  Clock_Init48MHz();
  Bump_Init();
  LaunchPad_Init(); // built-in switches and LEDs
  Motor_Stop(); // initialize and stop
  Mode = 0;
  I2CB1_Init(30); // baud rate = 12MHz/30=400kHz
  Init();
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
  OPT3101_Init();
  OPT3101_Setup();
  OPT3101_CalibrateInternalCrosstalk();
  OPT3101_ArmInterrupts(&TxChannel, Distances, Amplitudes);
  TxChannel = 3;
  OPT3101_StartMeasurementChannel(channel);
  LPF_Init(100,8);
  LPF_Init2(100,8);
  LPF_Init3(100,8);
  UR = UL = PWMNOMINAL; //initial power
  Pause();
  EnableInterrupts();
  while(1){
    if(Bump_Read()){ // collision
      Mode = 0;
      Motor_Stop();
      Pause();
    }
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
    Controller_Right();
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
  }
}
// MSP432 memory limited to q=11, N=2048
#define q   8       /* for 2^8 points */
#define NN   (1<<q)  /* 256-point FFT, iFFT */
complex_t a[NN], scratch[NN];
uint32_t PlotOffset,PlotData;
void main4(void){ // main4 is DFT of left distance
  int i = 0; int k=0;
  uint32_t channel = 1;
  DisableInterrupts();
  Clock_Init48MHz();
  SysTick->LOAD = 0x00FFFFFF;           // maximum reload value
  SysTick->CTRL = 0x00000005;           // enable SysTick with no interrupts
  I2CB1_Init(30); // baud rate = 12MHz/30=400kHz
  Init();
  UART0_Init();               // initialize UART0 115,200 baud rate

  OPT3101_Init();
  OPT3101_Setup();
  OPT3101_CalibrateInternalCrosstalk();
  OPT3101_ArmInterrupts(&TxChannel, Distances, Amplitudes);
  StartTime = SysTick->VAL;
  TxChannel = 3;
  OPT3101_StartMeasurementChannel(channel);
  LPF_Init(125,8);
  LPF_Init2(100,8);
  LPF_Init3(100,8);
  EnableInterrupts();
  uint32_t warmUpTime=0; // let OPT3101 rise to stable temperature
  SSD1306_Clear();
  i = 0; SSD1306_ClearBuffer();
  PlotOffset = 100; // 100 to 163 mm
  while(warmUpTime < 27000){ // 27000*0.1sec*60min/sec=15min
    if(TxChannel <= 2){ // 0,1,2 means new data
      if(TxChannel==0){ // fs=10 Hz
        FilteredDistances[0] = Left(LPF_Calc(Distances[0]));
        if(FilteredDistances[0] < PlotOffset){
          PlotData = 0;
        }else{
          PlotData = FilteredDistances[0]-PlotOffset;
          if(PlotData > 63){
            PlotData = 63;
          }
        }
        SSD1306_DrawPixel(i,63-PlotData,WHITE); // range PlotOffset to PlotOffset+63mm
        SSD1306_DisplayBuffer();
        i++;
      }else if(TxChannel==1){
        FilteredDistances[1] = LPF_Calc2(Distances[1]);
      }else {
        FilteredDistances[2] = Right(LPF_Calc3(Distances[2]));
      }
     // SetCursor(6, TxChannel+1);
     // OutUDec(FilteredDistances[TxChannel]);
     // SetCursor(0, 6); OutUDec(warmUpTime);
      TxChannel = 3; // 3 means no data
      channel = (channel+1)%3;
      OPT3101_StartMeasurementChannel(channel);
      if(i == 128){
        i = 0;
        SSD1306_ClearBuffer();
      }
      warmUpTime++;
    }
  }
  Clear();
  OutString("OPT3101");
  SetCursor(0, 1);
  OutString("Left =");
  SetCursor(0, 2);
  OutString("Centr=");
  SetCursor(0, 3);
  OutString("Right=");
  SetCursor(0, 4);
  OutString("Interrupts");
  SetCursor(0, 5);
  OutString("NL=");
  SetCursor(0, 6);
  OutString("NC=");
  SetCursor(0, 7);
  OutString("NR=");
  while(1){
    if(TxChannel <= 2){ // 0,1,2 means new data
      if(TxChannel==0){ // fs=10 Hz
        //  FilteredDistances[0] = Left(LPF_Calc(Distances[0]));
        FilteredDistances[0] = LPF_Calc(Distances[0]);
        a[k].Real = (float)FilteredDistances[0]; // units in mm
        a[k].Imag = 0.0;
        k++;
      }else if(TxChannel==1){
        FilteredDistances[1] = LPF_Calc2(Distances[1]);
      }else {
        FilteredDistances[2] = Right(LPF_Calc3(Distances[2]));
      }
      SetCursor(6, TxChannel+1);
      OutUDec(FilteredDistances[TxChannel]);
      TxChannel = 3; // 3 means no data
      channel = (channel+1)%3;
      OPT3101_StartMeasurementChannel(channel);
      i = i + 1;
    }
    if(i >= 300){
      i = 0;
      SetCursor(3, 5);
      OutUDec((uint16_t)Noise());  OutChar(','); OutUDec(Amplitudes[0]);
      SetCursor(3, 6);
      OutUDec((uint16_t)Noise2()); OutChar(','); OutUDec(Amplitudes[1]);
      SetCursor(3, 7);
      OutUDec((uint16_t)Noise3()); OutChar(','); OutUDec(Amplitudes[2]);
    }
    if(k == NN){
      UART0_OutString("OPT3101, fs=10Hz, N=");  UART0_OutUDec(NN);UART0_OutString(" samples\n");
      UART0_OutString("Time(ms),Distance(mm)\n");
      for(int n=0; n<NN; n++){
          UART0_OutUFix1(n); UART0_OutString(", ");
        int32_t data = (int32_t)a[n].Real;
        UART0_OutSDec(data);UART0_OutChar('\n');
      }
      StartTime = SysTick->VAL;
      fft(a, NN, scratch);
      TimeToConvert = ((StartTime-SysTick->VAL)&0x00FFFFFF)/48; // usec
      UART0_OutString("\nTime to execute FFT "); UART0_OutUDec(TimeToConvert); UART0_OutString(" us");
      UART0_OutString("\nFreq(Hz),Magnitude(mm)\n");
      UART0_OutUFix2(0); UART0_OutString(", ");
      int32_t data = (int32_t)(100.0*sqrt(a[0].Real*a[0].Real+a[0].Imag*a[0].Imag)/(float)NN);
      UART0_OutUFix2(data);UART0_OutChar('\n');
      for(int n=1; n<NN/2; n++){
        UART0_OutUFix2(1000*n/NN); UART0_OutString(", ");
        int32_t data = (int32_t)(100.0*sqrt(a[n].Real*a[n].Real+a[n].Imag*a[n].Imag)/(float)(NN/2));
        UART0_OutUFix2(data);UART0_OutChar('\n');
      }
      k = 0;
      OPT3101_StartMeasurementChannel(channel);
    }
    WaitForInterrupt();
  }
}

