/**
 * @file      Blinker.h
 * @brief     Provide low-level functions that control LEDs on TI-RSLK MAX robot.
 * @details   Four LED are connected to Port 8<br>
 1) Hardware uses positive logic low current LEDs<br>
 2) Positioned on the four corners of the robot just for fun<br>
 3) Software outputs to these four LEDs<br>
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      June 28, 2019
 *
<table>
<caption id="Blinker">Four LEDs on the RSLK interface board</caption>
<tr><th>Pin  <th>Sensor
<tr><td>P8.7 <td>Back right red LED
<tr><td>P8.6 <td>Back left red LED
<tr><td>P8.5 <td>Front right yellow LED
<tr><td>P8.0 <td>Front left yellow LED
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
#ifndef _BLINKER_H_
#define _BLINKER_H_

#include <stdint.h>

/**
 * Initialize blinker LEDs sensors on TI-RSLK MAX<br>
 * Make P8.7, P8.6, P8.5 and P8.0 outputs<br>
 *
 * @param none
 * @return none
 * @brief  Initialize blinker LEDs
 */
void Blinker_Init(void);
#define FR_RGHT 0x20 ///< Front right blinker LED
#define FR_LEFT 0x01 ///< Front left blinker LED
#define BK_RGHT 0x80 ///< Back right blinker LED
#define BK_LEFT 0x40 ///< Back left blinker LED

/**
 * Output to four LED blinkers on TI-RSLK MAX
 * @param data to write to LEDs (uses bits 7,6,5,0)<br>
 * bit 7 Red back right LED<br>
 * bit 6 Red back left LED<br>
 * bit 5 Yellow front right LED<br>
 * bit 0 Yellow front left LED
 * @note use these four defines:
 * FR_RGHT, FR_LEFT, BK_RGHT, or BK_LEFT.
 * Output 0 to turn all off
 * @return none
 * @brief  Initialize blinker LEDs
 */
void Blinker_Output(uint8_t data);

#endif
