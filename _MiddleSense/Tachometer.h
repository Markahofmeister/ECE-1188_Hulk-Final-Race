/**
 * @file      Tachometer.h
 * @brief     Take tachometer angle and distance measurements
 * @details   Provide mid-level functions that initialize ports,
 * take angle and distance measurements, and report total travel
 * statistics.
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      June 28, 2019
 *

<table>
<caption id="tach_interface">Romi Encoder connections</caption>
<tr><th>MSP432  <th>Romi Encoder  <th>comment
<tr><td>P10.5 (J5) <td>ELA           <td>Left Encoder A, used for speed
<tr><td>P5.2 (J5) <td>ELB           <td>Left Encoder B, used for direction
<tr><td>P10.4 (J5)<td>ERA           <td>Right Encoder A, used for speed
<tr><td>P5.0 (J5)<td>ERB           <td>Right Encoder B, used for direction
</table>
 ******************************************************************************/

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

/*!
 * @defgroup RSLK_Input_Output
 * @brief
 * @{*/

#ifndef TACHOMETER_H_
#define TACHOMETER_H_


/**
 * \brief specifies the direction of the motor rotation, relative to the front of the robot
 */
enum TachDirection{
  FORWARD, /**< Wheel is making robot move forward */
  STOPPED, /**< Wheel is stopped */
  REVERSE  /**< Wheel is making robot move backward */
};

/**
 * Initialize GPIO pins for input, which will be
 * used to determine the direction of rotation.
 * Initialize the input capture interface, which
 * will be used to measure the speed of rotation.
 * @param none
 * @return none
 * @brief  Initialize tachometer interface
 */
void Tachometer_Init(void);

/**
 * Get the most recent tachometer measurements.
 * @param leftTach is pointer to store last measured tachometer period of left wheel (units of 0.083 usec)
 * @param leftDir is pointer to store enumerated direction of last movement of left wheel
 * @param leftSteps is pointer to store total number of forward steps measured for left wheel (360 steps per ~220 mm circumference)
 * @param rightTach is pointer to store last measured tachometer period of right wheel (units of 0.083 usec)
 * @param rightDir is pointer to store enumerated direction of last movement of right wheel
 * @param rightSteps is pointer to store total number of forward steps measured for right wheel (360 steps per ~220 mm circumference)
 * @return none
 * @note Assumes Tachometer_Init() has been called<br>
 * @note Assumes Clock_Init48MHz() has been called
 * @brief Get the most recent tachometer measurement
 */
void Tachometer_Get(uint16_t *leftTach, enum TachDirection *leftDir, int32_t *leftSteps,
                    uint16_t *rightTach, enum TachDirection *rightDir, int32_t *rightSteps);

#endif /* TACHOMETER_H_ */
