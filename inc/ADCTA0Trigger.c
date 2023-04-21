// ADC14TA0Trigger.c
// Runs on MSP432, TI-RSLK MAX 1.1
// ADC input, timer trigger interrupting system, 14-bit conversion,
// No jitter
// Jonathan Valvano
// July 11, 2019

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

void (*FinishedTask)(uint16_t result);// user function called when conversion complete

// ***************** ADC0_InitTA0TriggerCh6 ****************
// Activate Timer A0 to periodically trigger ADC conversions
// on P4.7 = A6. Run the user task when each conversion is
// complete.
// Inputs:  task is a pointer to a user function
//          period in units (1/SMCLK), 16 bits
// Outputs: none
void ADC0_InitTA0TriggerCh6(void(*task)(uint16_t result), uint16_t period){
  FinishedTask = task;             // user function
  // ***************** timer initialization *****************
  TIMER_A0->CTL &= ~0x0030;               // halt Timer A0
  // bits15-10=XXXXXX, reserved
  // bits9-8=10,       clock source to SMCLK
  // bits7-6=00,       input clock divider /1
  // bits5-4=00,       stop mode
  // bit3=X,           reserved
  // bit2=0,           set this bit to clear
  // bit1=0,           interrupt disable
  // bit0=0,           clear interrupt pending
  TIMER_A0->CTL = 0x0200;
  // bits15-14=00,     no capture mode
  // bits13-12=XX,     capture/compare input select
  // bit11=X,          synchronize capture source
  // bit10=X,          synchronized capture/compare input
  // bit9=X,           reserved
  // bit8=0,           compare mode
  // bits7-5=011,      set/reset output mode
  // bit4=0,           disable capture/compare interrupt
  // bit3=X,           read capture/compare input from here
  // bit2=X,           output this value in output mode 0
  // bit1=X,           capture overflow status
  // bit0=0,           clear capture/compare interrupt pending
  TIMER_A0->CCTL[1] = 0x0060;
  TIMER_A0->CCR[1] = (period - 1)/2;        // defines when output signal is set, triggering ADC conversion
  TIMER_A0->CCR[0] = (period - 1);          // defines when output signal is cleared
  TIMER_A0->EX0 &= ~0x0007;               // configure for input clock divider /1
  // ***************** reference initialization *****************
  while(REF_A->CTL0&0x0400){};         // wait for the reference to be idle before reconfiguring (see REF4 in errata)
  REF_A->CTL0 = 0x0039;                // 1) configure reference for static 2.5V
//debug code
/*  REFCTL0 = 0x003B;                // 1) configure reference for static 2.5V and constant output on P5.6 (see REF7 in errata)
  P5SEL1 |= 0x40;                  // analog mode on P5.6
  P5SEL0 |= 0x40;*/
//end of debug code
  // 15-14 reserved                          00b (reserved)
  // 13    REFBGRDY   bandgap voltage ready   0b (read only)
  // 12    REFGENRDY  reference voltage ready 0b (read only)
  // 11    BGMODE     bandgap mode            0b (read only)
  // 10    REFGENBUSY no reconfigure if busy  0b (read only)
  // 9     REFBGACT   bandgap active          0b (read only)
  // 8     REFGENACT  reference active        0b (read only)
  // 7     REFBGOT    trigger bandgap         0b = no software trigger set
  // 6     REFGENOT   trigger reference       0b = no software trigger set
  // 5-4   REFVSEL    voltage level select   11b = 2.5V
  // 3     REFTCOFF   temperature disable     1b = temperature sensor disabled to save power
  // 2     reserved                           0b (reserved)
  // 1     REFOUT     reference output buffer 0b = reference output not on P5.6 (see also REFBURST in ADC14CTL1, P5SEL1, and P5SEL0)
  // 0     REFON      reference enable        1b = reference enabled in static mode
  // ***************** ADC initialization *****************
  while((REF_A->CTL0&0x1000) == 0){};  // wait for the reference to stabilize before continuing (optional, see REF8 in errata)
  ADC14->CTL0 &= ~0x00000002;        // 2) ADC14ENC = 0 to allow programming
  while(ADC14->CTL0&0x00010000){};   // 3) wait for BUSY to be zero
  // 31-30 ADC14PDIV  predivider,            00b = Predivide by 1
  // 29-27 ADC14SHSx  SHM source            001b = TA0_C1 bit
  // 26    ADC14SHP   SHM pulse-mode          1b = SAMPCON the sampling timer
  // 25    ADC14ISSH  invert sample-and-hold  0b =  not inverted
  // 24-22 ADC14DIVx  clock divider         000b = /1
  // 21-19 ADC14SSELx clock source select   100b = SMCLK
  // 18-17 ADC14CONSEQx mode select          10b = Repeat-single-channel
  // 16    ADC14BUSY  ADC14 busy              0b (read only)
  // 15-12 ADC14SHT1x sample-and-hold time 0011b = 32 clocks
  // 11-8  ADC14SHT0x sample-and-hold time 0011b = 32 clocks
  // 7     ADC14MSC   multiple sample         0b = one sample per SHI rising edge
  // 6-5   reserved                          00b (reserved)
  // 4     ADC14ON    ADC14 on                1b = powered up
  // 3-2   reserved                          00b (reserved)
  // 1     ADC14ENC   enable conversion       0b = ADC14 disabled
  // 0     ADC14SC    ADC14 start             0b = No start (yet)
  ADC14->CTL0 = 0x0C243310;          // 4) repeat single, SMCLK, on, disabled, /1, 32 SHM
  // 20-16 STARTADDx  start addr          00000b = ADC14MEM0
  // 15-6  reserved                  0000000000b (reserved)
  // 5-4   ADC14RES   ADC14 resolution       11b = 14 bit, 16 clocks
  // 3     ADC14DF    data read-back format   0b = Binary unsigned
  // 2     REFBURST   reference buffer burst  0b = reference on continuously
  // 1-0   ADC14PWRMD ADC power modes        00b = Regular power mode
  ADC14->CTL1 = 0x00000030;          // 5) ADC14MEM0, 14-bit, ref on, regular power
  // 15   ADC14WINCTH Window comp threshold   0b = not used
  // 14   ADC14WINC   Comparator enable       0b = Comparator disabled
  // 13   ADC14DIF    Differential mode       0b = Single-ended mode enabled
  // 12   reserved                            0b (reserved)
  // 11-8 ADC14VRSEL  V(R+) and V(R-)      0001b = V(R+) = VREF, V(R-) = AVSS
  // 7    ADC14EOS    End of sequence         1b = End of sequence
  // 6-5  reserved                           00b (reserved)
  // 4-0  ADC14INCHx  Input channel        0110b = A6, P4.7
  ADC14->MCTL[0] = 0x00000186;       // 6) 0 to 2.5V, channel 6
  ADC14->IER0 |= 0x00000001;         // 7) enable ADC14IFG0 interrupt
  ADC14->IER1 = 0;                   //    disable these interrupts
  P4->SEL1 |= 0x80;                  // 8) analog mode on A6, P4.7
  P4->SEL0 |= 0x80;
  ADC14->CTL0 |= 0x00000002;         // 9) enable
  // ***************** interrupt initialization *****************
  NVIC->IP[6] = (NVIC->IP[6]&0xFFFFFF00)|0x00000040; // priority 2
// interrupts enabled in the main program after all devices initialized
  NVIC->ISER[0] = 0x01000000;   // enable interrupt 24 in NVIC
  TIMER_A0->CTL |= 0x0014;                // reset and start Timer A0 in up mode
}

void ADC14_IRQHandler(void){ uint16_t result;
  if((ADC14->IFGR0&0x00000001) == 0x00000001){
    result = ADC14->MEM[0];        // read the result and acknowledge ADC completion
    (*FinishedTask)(result);       // execute user task
  }
}
