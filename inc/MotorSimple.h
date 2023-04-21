/**
 * @file      MotorSimple.h
 * @brief     Set motor speed and direction
 * @details   Provide mid-level functions that initialize ports and
 * set motor speeds to move the robot. Simple solution that does NOT
 * use PWM hardware. Rather the PWM outputs are created by software
 * delays using the SysTick timer.<br>
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      June 28, 2019

<table>
<caption id="motorconnections2">TI-RSLK MAX v1.1 motor connections</caption>
<tr><th>Pin<th>J<th>MSP432<th>Motor function
<tr><td>P5.4<td>J3.29<td>GPIO<td>Left motor direction
<tr><td>P2.7<td>J4.40<td>TA0CCP4<td>Left motor PWM
<tr><td>P3.7<td>J4.31<td>GPIO<td>Left motor enable
<tr><td>P5.5<td>J3.30<td>GPIO<td>Right motor direction
<tr><td>P2.6<td>J4.39<td>TA0CCP3<td>Right motor PWM
<tr><td>P3.6<td>J2.11<td>GPIO<td>Right motor enable
</table>


 This example accompanies the book
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
/*!
 * @defgroup RSLK_Input_Output
 * @brief
 * @{*/

#ifndef MOTORSIMPLE_H_
#define MOTORSIMPLE_H_
// *******Lab 12 solution*******

/**
 * Initializes the 6 GPIO lines and puts driver to sleep.<br>
 * This simple set of drivers does not use hardware timer PWM.
 * Rather, it uses software delays with SysTick and consumes all the
 * processor time. This function assumes pin configuration of RSLK 1.1
 * @param none
 * @return none
 * @note Returns right away.
 * @brief  Initialize motor interface
 */
void Motor_InitSimple(void);



/**
 * Stops both motors, puts driver to sleep.
 * This function assumes pin configuration of RSLK 1.1
 * @param none
 * @return none
 * @note Returns right away.
 * @brief  Stops the motors
 */
void Motor_StopSimple(void);


/**
 * Drives both motors forward at duty (100 to 9900).
 * Runs for time duration (units=10ms), and then stops.
 * Stop the motors and return if any bumper switch is active.
 * Returns after time*10ms or if a bumper switch is hit.
 * This function assumes pin configuration of RSLK 1.1
 * @param duty duty cycle of both wheels (100 to 9900)
 * @param time duration (10ms) to run
 * @return none
 * @note Does not return until time has elapsed or any bumper switch is active.
 * @brief  Run both motors forward
 */
void Motor_ForwardSimple(uint16_t duty, uint32_t time);


/**
 * Drives both motors backward at duty (100 to 9900).
 * Runs for time duration (units=10ms), and then stops.
 * Runs even if any bumper switch is active.
 * Returns after time*10ms.
 * This function assumes pin configuration of RSLK 1.1
 * @param duty duty cycle of both wheels (100 to 9900)
 * @param time duration (10ms) to run
 * @return none
 * @note Does not return until time has elapsed.
 * @brief  Run both motors backward
 */
void Motor_BackwardSimple(uint16_t duty, uint32_t time);


/**
 * Drives just the left motor forward at duty (100 to 9900).
 * Right motor is stopped (sleeping).
 * Runs for time duration (units=10ms), and then stops.
 * Stop the motors and return if any bumper switch is active.
 * Returns after time*10ms or if a bumper switch is hit.
 * This function assumes pin configuration of RSLK 1.1
 * @param duty duty cycle of left wheel (100 to 9900)
 * @param time duration (10ms) to run
 * @return none
 * @note Does not return until time has elapsed or any bumper switch is active.
 * @brief  Run left motor forward
 */
void Motor_LeftSimple(uint16_t duty, uint32_t time);


/**
 * Drives just the right motor forward at duty (100 to 9900).
 * Left motor is stopped (sleeping).
 * Runs for time duration (units=10ms), and then stops.
 * Stop the motors and return if any bumper switch is active.
 * Returns after time*10ms or if a bumper switch is hit.
 * This function assumes pin configuration of RSLK 1.1
 * @param duty duty cycle of right wheel (100 to 9900)
 * @param time duration (10ms) to run
 * @return none
 * @note Does not return until time has elapsed or any bumper switch is active.
 * @brief  Run right motor forward
 */
void Motor_RightSimple(uint16_t duty, uint32_t time);



#endif /* MOTORSIMPLE_H_ */
