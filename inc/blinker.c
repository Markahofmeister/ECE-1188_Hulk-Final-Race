// Blinker.c
// Runs on MSP432
// Provide low-level functions that output to LEDs on the robot.
// Daniel Valvano and Jonathan Valvano
// June 30, 2019

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
#include "blinker.h"
// Blinker LEDS
// Front right P8.5 Yellow LED
// Front left  P8.0 Yellow LED
// Back right  P8.7 Red LED
// Back left   P8.6 Red LED
// ------------Blinker_Init------------
// Initialize four LED blinkers on TIRSLK 1.1
// Input: none
// Output: none
void Blinker_Init(void){
  P8->SEL0 &= ~0xE1;
  P8->SEL1 &= ~0xE1;    // configure P8.0,P8.5-P8.7 as GPIO
  P8->DIR |= 0xE1;      // make P8.0,P8.5-P8.7 out
  P8->OUT &= ~0xE1;     // all LEDs off
}

//------------Blinker_Output------------
// Output to four LED blinkers on TIRSLK
// Input: data to write to LEDs (uses bits 7,6,5,0)
//   bit 7 Red back right LED
//   bit 6 Red back left LED
//   bit 5 Yellow front right LED
//   bit 0 Yellow front left LED
// Output: none
void Blinker_Output(uint8_t data){  // write four outputs bits of P8
  P8->OUT = (P8->OUT&0x1E)|data;
}
