// TMP117.c
// Runs on MSP432, RSLK
// Provide low-level functions that interface TMP117 to the robot.
// derived from TMP117.cpp TMP117.h by Nils Minor
// edited by Valvano and Valvano 2/29/2020

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
#include <stdint.h>
#include <msp.h>
#include "../inc/TMP117.h"
#include "../inc/I2CB1.h"
#include "../inc/Clock.h"

/* RSLK pin conflict on J3.28 P4.7
   RSLK BMP5 (bumper switch 5)
   BP-BASSENSORSMKII Sensor TMP_LMT_V+
The TMP117 is powered directly from P4.7, which works because of its very low quiescent current (3.5-220 µA)
a) To use the TMP117 Temperature Sensor and not BMP3-5: Un-bolt and remove the left bump switch assembly (BMP3-5) and make P4.7 a GPIO output high to power the Sensor. Warning: Do not make P4.7 a GPIO output high and then press BMP5.
b) To use BMP5 and not the TMP117 Temperature Sensor: Make P4.7 a GPIO input from BMP5 (pressing BMP5 temporarily un-powers TMP116 Temperature Sensor)

RSLK pin conflict on J3.26 P4.4
  RSLK reserved P4.4 for TExaS Scope input
  BP-BASSENSORSMKII uses P4.4 for Sensor TMP_ALERT
Conflict resolution: do not use both TExaS Scope and Sensor TMP_ALERT

 */
// TMP117 hardware from BP-BASSENSORSMKII Sensor Booster Pack
// GND       ground
// TMP_V+    TMP117 V+    pin 5 <- P4.7 power to sensor
// SDA       TMP117 SDA   pin 6 <> P6.4 I2C data SDA
// SCL       TMP117 SCL   pin 1 <- P6.5 I2C clock SCL
// TMP_ALERT TMP117 ALERT pin 3 -> P4.4 alert
// ADD0      TMP117 ADD0  pin 4 = ground to set address 0x48
// RESOLUTION is 0.0078125 C
 /*  Conversion Cycle Time in CC Mode
              AVG       0       1       2       3
      CONV  averaging  (0)     (8)     (32)   (64)
        0             15.5ms  125ms   500ms    1s     C15mS5
        1             125ms   125ms   500ms    1s     C125mS
        2             250ms   250ms   500ms    1s     C250mS
        3             500ms   500ms   500ms    1s     C500mS
        4             1s      1s      1s       1s     C1S
        5             4s      4s      4s       4s     C4S
        6             8s      8s      8s       8s     C8S
        7             16s     16s     16s      16s    C16S
*/
#define TMP117_ADDR     0x48    // 1001000

#define TMP117_REG_TEMPERATURE          0x00
#define TMP117_REG_CONFIGURATION        0x01
#define TMP117_REG_TEMP_HIGH_LIMIT      0x02
#define TMP117_REG_TEMP_LOW_LIMIT       0x03

#define TMP117_REG_EEPROM_UNLOCK        0x04
#define TMP117_REG_EEPROM1              0x05
#define TMP117_REG_EEPROM2              0x06
#define TMP117_REG_EEPROM3              0x08

#define TMP117_REG_TEMPERATURE_OFFSET   0x07
#define TMP117_REG_DEVICE_ID            0x0F
enum TMP117_PMODE     {THERMAL = 0, ALERT, DATA};                                 //!<  Pin mode
enum TMP117_CMODE     {CONTINUOUS = 0, SHUTDOWN = 1, ONESHOT = 3};                //!<  Conversion mode
enum TMP117_CONVT     {C15mS5 = 0, C125mS, C250mS, C500mS, C1S, C4S, C8S, C16S};  //!<  Conversion time
enum TMP117_AVE       {NOAVE = 0, AVE8, AVE32, AVE64};                            //!<  Averaging mode
enum TMP117_ALERT     {NOALERT = 0, HIGHALERT, LOWALERT};                         //!<  Alert type

uint16_t static i2cRead2B(uint8_t address){
  uint8_t buffer[2];
  uint16_t reg_value;
  I2CB1_Send(TMP117_ADDR, &address, 1);
  I2CB1_Recv(TMP117_ADDR, buffer, 2);
  reg_value = buffer[1] + ((uint16_t)buffer[0] << 8);
  return reg_value;
}
void static i2cWrite2B(uint8_t reg, uint16_t data){
  uint8_t buffer[] = {
    reg,
    (data>>8)&0xFF,
    data&0xFF,
    };
  I2CB1_Send(TMP117_ADDR, buffer, sizeof(buffer));
  Clock_Delay1us(10);
}
uint16_t static readConfig(void){
  uint16_t reg_value;
  reg_value = i2cRead2B(TMP117_REG_CONFIGURATION);
  return reg_value;
}
void  static writeConfig(uint16_t config_data){
  i2cWrite2B(TMP117_REG_CONFIGURATION, config_data);
}

void static setConvMode(enum TMP117_CMODE cmode){
   uint16_t reg_value;
   reg_value = readConfig();
   reg_value &= ~(3<<10);         // clear bits
   reg_value |= (cmode&0x03)<<10; // set bits
   writeConfig(reg_value);
}
void static setConvTime(enum TMP117_CONVT convtime){
  uint16_t reg_value;
  reg_value = readConfig();
  reg_value &= ~(7 << 7);           // clear bits
  reg_value |= (convtime&0x07)<<7;  // set bits
  writeConfig(reg_value);
}
void static setAveraging(enum TMP117_AVE ave){
  uint16_t reg_value;
  reg_value = readConfig();
  reg_value &= ~(3 << 5) ;      // clear bits
  reg_value |= (ave&0x03)<<5;   // set bits
  writeConfig(reg_value);
}
void static setOffsetTemperature(int16_t offset){
  i2cWrite2B(TMP117_REG_TEMPERATURE_OFFSET, offset);
}
void static setAlertMode(enum TMP117_PMODE mode){
  uint16_t reg_value;
  reg_value = readConfig();
  if (mode == THERMAL) {
    reg_value |= 1 << 4;    // change to thermal mode
    reg_value &= ~(1 << 2); // set pin as alert flag
    reg_value &= ~(1 << 3); // alert pin low active
  }
  else if (mode == ALERT) {
    reg_value &= ~(1 << 4); // change to alert mode
    reg_value &= ~(1 << 2); // set pin as alert flag
    reg_value &= ~(1 << 3); // alert pin low active
  }
  else if (mode == DATA) {
    reg_value |= 1 << 2;    // set pin as data ready flag
  }
  writeConfig(reg_value);
}
void (*Ptemperature)(int16_t t);
void TMP117_Init(void){
  // Drive P4.7 high to power the TMP117.
  P4->SEL0 &= ~0x90;
  P4->SEL1 &= ~0x90;    // 1) configure P4.7, P4.4 as GPIO
  P4->DIR &= ~0x10;     // Make P4.4 be an input for the data ready flag.
  P4->OUT |= 0x10;  // open drain requires a pullup
  P4->REN |= 0x10;
  P4->OUT |= 0x80;
  P4->DIR |= 0x80;
  Clock_Delay1ms(1); // needed??
  setConvMode(CONTINUOUS);
  setConvTime(C125mS);
  setAveraging(AVE64);
  setAlertMode(DATA);
  setOffsetTemperature(0);
}
void TMP117_Arm(void (*pt)(int16_t t)){
  // Drive P4.7 high to power the TMP117.
  Ptemperature = pt;
  TMP117_Init();

  P4->IES |= 0x10;      // P4.4 is falling edge event
  P4->IFG = 0x00;       // clear flags (reduce possibility of extra interrupt)
  P4->IE |= 0x10;       // arm interrupt on P4.4
  NVIC->IP[9] = (NVIC->IP[9]&0xFF00FFFF)|0x00000000; // priority 0
  NVIC->ISER[1] = 0x00000040;  // enable interrupt 38 in NVIC
}
void TMP117_Reset(void) {
  writeConfig(0x02); // When bit set to 1 it triggers software reset with a duration of 2 ms
  Clock_Delay1ms(2); // needed??
}
int16_t TMP117_ReadTemperature(void){
  int16_t temperature = i2cRead2B(TMP117_REG_TEMPERATURE);
  return temperature ;
}


// triggered on temperature alert, falling edge, new data ready
void PORT4_IRQHandler(void){
  (*Ptemperature) (TMP117_ReadTemperature());
  setConvMode(ONESHOT);
  P4->IFG = 0x00;  // clear all flags
}
