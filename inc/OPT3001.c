// OPT3001.c
// Runs on MSP432, RSLK
// Provide low-level functions that interface OPT3001 to the robot.
// derived from ClosedCube_OPT3001.cpp ClosedCube_OPT3001.h by AA for ClosedCube
// edited for MSP432 by Valvano and Valvano 3/1/2020

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
/*

Arduino library for Texas Instruments OPT3001 Digital Ambient Light Sensor
Written by AA for ClosedCube
---

The MIT License (MIT)

Copyright (c) 2015 ClosedCube Limited

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <stdint.h>
#include <msp.h>
#include "../inc/OPT3001.h"
#include "../inc/I2CB1.h"
#include "../inc/Clock.h"

/* J3.25 P4.2
RSLK BMP1 (bumper switch 1), switch to ground and software internal pullup
BP-BASSENSORSMKII Sensor OPT_INT
because the INT pin functionality on OPT3001 Ambient Light Sensor is open-drain, there is not an electrical conflict between the signals

J3.27 P4.5
RSLK BMP3 (bumper switch 3), switch to ground and software internal pullup
BP-BASSENSORSMKII Sensor OPT_V+
this one is different from the other power-down configurations because the OPT3001 is powered directly from P4.5, which works because of its very low quiescent current (3.7 µA)
a) To use the OPT3001 Ambient Light Sensor and not BMP3-5: Un-bolt and remove the left bump switch assembly (BMP3-5) and make P4.5 a GPIO output high to power the Sensor. Warning: Do not make P4.5 a GPIO output high and then press BMP3.
b) To use BMP3 and not the OPT3001 Ambient Light Sensor: Make P4.5 a GPIO input from BMP3 (pressing BMP3 temporarily un-powers OPT3001 Ambient Light Sensor)

 */
// OPT3001 hardware from BP-BASSENSORSMKII Sensor Booster Pack
// GND     ground        pins 3,7
// OPT_V+  OPT3001 VDD   pin 1 <- P4.5 power to sensor
// SDA     OPT3001 SDA   pin 6 <> P6.4 I2C data SDA
// SCL     OPT3001 SCL   pin 4 <- P6.5 I2C clock SCL
// OPT_INT OPT3001 INT   pin 5 -> P4.2 interrupt
// ADDR    OPT3001 ADDR  pin 2 = ground to set address 0x44
//

#define OPT3001_ADDRESS 0x44
typedef enum{
  RESULT      = 0x00,
  CONFIG      = 0x01,
  LOW_LIMIT   = 0x02,
  HIGH_LIMIT  = 0x03,
  MANUFACTURER_ID = 0x7E,
  DEVICE_ID       = 0x7F,
} OPT3001_Commands;

typedef union{
  struct{
    uint8_t FaultCount : 2;
    uint8_t MaskExponent : 1;
    uint8_t Polarity : 1;
    uint8_t Latch : 1;
    uint8_t FlagLow : 1;
    uint8_t FlagHigh : 1;
    uint8_t ConversionReady : 1;
    uint8_t OverflowFlag : 1;
    uint8_t ModeOfConversionOperation : 2;
    uint8_t ConvertionTime : 1;
    uint8_t RangeNumber : 4;
  };
  uint16_t rawData;
} OPT3001_Config;

void static writeData(OPT3001_Commands command){
  I2CB1_Send(OPT3001_ADDRESS, &command, 1);
}

void static readData(uint16_t* data){
  uint8_t buffer[2];
  I2CB1_Recv(OPT3001_ADDRESS, buffer, 2);
  *data = buffer[1] + ((uint16_t)buffer[0] << 8);
}

OPT3001_Result static readRegister(OPT3001_Commands command) {
   writeData(command);
   OPT3001_Result result;
   readData(&result.rawData);
   return result;
}

void static i2cWrite2B(uint8_t reg, uint16_t data){
  uint8_t buffer[] = {
    reg,
    (data>>8)&0xFF,
    data&0xFF,
    };
  I2CB1_Send(OPT3001_ADDRESS, buffer, sizeof(buffer));
  Clock_Delay1us(10);
}
void static writeConfig(OPT3001_Config config) {
  i2cWrite2B(CONFIG,config.rawData);
}

OPT3001_Config static readConfig(void){
  OPT3001_Config config;
  writeData(CONFIG);
  readData(&config.rawData);
  return config;
}

OPT3001_Config sensorConfig;
void OPT3001_Init(void){
  // Drive P4.5 high to power the OPT3001.
  P4->SEL0 &= ~0x24;
  P4->SEL1 &= ~0x24;  // 1) configure P4.5, P4.2 as GPIO
  P4->DIR &= ~0x04;   // Make P4.2 be an input for the flag.
  P4->OUT |= 0x04;    // open drain requires a pullup
  P4->REN |= 0x04;
  P4->OUT |= 0x20;
  P4->DIR |= 0x20;
  Clock_Delay1ms(1); // needed??
  OPT3001_Config newConfig;

  newConfig.RangeNumber = 0x0C; // full automatic scale
  newConfig.ConvertionTime = 0; // 0 = 100 ms,   1 = 800 ms
  newConfig.Latch = 1;
  /*Latch field (read or write).
The latch field controls the functionality of the interrupt reporting mechanisms: the INT pin, the
flag high field (FH), and flag low field (FL). This bit selects the reporting style between a
latched window-style comparison and a transparent hysteresis-style comparison.
0 = The device functions in transparent hysteresis-style comparison operation, where the three
interrupt reporting mechanisms directly reflect the comparison of the result register with the
high- and low-limit registers with no user-controlled clearing event. See the Interrupt Operation,
INT Pin, and Interrupt Reporting Mechanisms section for further details.
1 = The device functions in latched window-style comparison operation, latching the interrupt
reporting mechanisms until a user-controlled clearing event.*/
  newConfig.ModeOfConversionOperation = 3; //Continuous conversions
  //00 = Shutdown (default)
  //01 = Single-shot
  //10, 11 = Continuous conversions
  writeConfig(newConfig);
  sensorConfig = readConfig();
}
// lux = 0.01*(2^Exponent)*Result
OPT3001_Result OPT3001_ReadLight(void){
  return readRegister(RESULT);
}


