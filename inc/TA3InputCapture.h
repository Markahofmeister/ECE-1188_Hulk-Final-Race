/**
 * @file      TA3InputCapture.h
 * @brief     Initialize Timer A3
 * @details   Use Timer A3 in capture mode to request interrupts on rising
 * edges of P10.4 (TA3CCP0) and P8.2 (TA3CCP2) and call user functions.<br>
 * Use Timer A3 in capture mode to request interrupts on rising
 * edges of P10.4 (TA3CCP0) and P10.5 (TA3CCP1) and call user functions.
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      June 28, 2019
 ******************************************************************************/

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019

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

// external signal connected to P8.2 (TA3CCP2) (trigger on rising edge)
// external signal connected to P10.5 (TA3CCP1) (trigger on rising edge)
// external signal connected to P10.4 (TA3CCP0) (trigger on rising edge)

/*!
 * @defgroup Timer
 * @brief
 * @{*/
#ifndef TA3INPUTCAPTURE_H_
#define TA3INPUTCAPTURE_H_


/**
 * Initialize Timer A3 in edge time mode to request interrupts on
 * the rising edges of P10.4 (TA3CCP0) and P8.2 (TA3CCP2).  The
 * interrupt service routines acknowledge the interrupt and call
 * a user function.
 * @param task0 is a pointer to a user function called when P10.4 (TA3CCP0) edge occurs<br>
 *        parameter is 16-bit up-counting timer value when P10.4 (TA3CCP0) edge occurred (units of 0.083 usec)<br>
 * @param task2 is a pointer to a user function called when P8.2 (TA3CCP2) edge occurs<br>
 *        parameter is 16-bit up-counting timer value when P8.2 (TA3CCP2) edge occurred (units of 0.083 usec)
 * @return none
 * @note  Assumes low-speed subsystem master clock is 12 MHz
 * @brief  Initialize Timer A3 interrupts on P10.4 and P8.2
 */
void TimerA3Capture_Init02(void(*task0)(uint16_t time), void(*task2)(uint16_t time));

/**
 * Initialize Timer A3 in edge time mode to request interrupts on
 * the rising edges of P10.4 (TA3CCP0) and P10.5 (TA3CCP1).  The
 * interrupt service routines acknowledge the interrupt and call
 * a user function.
 * @param task0 is a pointer to a user function called when P10.4 (TA3CCP0) edge occurs<br>
 *        parameter is 16-bit up-counting timer value when P10.4 (TA3CCP0) edge occurred (units of 0.083 usec)<br>
 * @param task1 is a pointer to a user function called when P10.5 (TA3CCP1) edge occurs<br>
 *        parameter is 16-bit up-counting timer value when P10.5 (TA3CCP1) edge occurred (units of 0.083 usec)
 * @return none
 * @note  Assumes low-speed subsystem master clock is 12 MHz
 * @brief  Initialize Timer A3 interrupts on P10.4 and P10.5
 */
void TimerA3Capture_Init01(void(*task0)(uint16_t time), void(*task1)(uint16_t time));

#endif /* TA3INPUTCAPTURE_H_ */
