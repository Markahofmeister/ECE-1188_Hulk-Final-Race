/**
 * @file      Ultrasound.h
 * @brief     Take ultrasonic distance measurements
 * @details   Provide mid-level functions that initialize ports, start
 * an ultrasonic sensor measurement, and finish an ultrasonic
 * sensor measurement using the HC-SR04 ultrasonic distance
 * sensor.<br>
 * 5V regulator output connected to HC-SR04 Vcc (+5V) <br>
 * 22k top connected to HC-SR04 Echo (digital output from sensor)<br>
 * 22k bottom connected to 33k top and MSP432 P6.6 (J4.36) (digital input to MSP432)<br>
 * 33k bottom connected to ground<br>
 * Ground connected to HC-SR04 ground and MSP432 ground (J3.22)<br>
 * MSP432 P5.6 (J4.37) (digital output from MSP432) connected to HC-SR04 trigger
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      June 28, 2019
<table>
<caption id="Ultrasoundpins">Ultrasonic distance measurement</caption>
<tr><th>Pin  <th>Connection
<tr><td>P5.6 <td>digital output from MSP432, connected to HC-SR04 trigger
<tr><td>P6.6 <td>digital input to MSP432 (use 22k/33kresistor divider to lower voltage)
</table>
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


/*!
 * @defgroup RSLK_Input_Output
 * @brief
 * @{*/


#ifndef ULTRASOUND_H_
#define ULTRASOUND_H_


/**
 * Initialize a GPIO pin for output, which will be
 * used to trigger the ultrasonic sensor.
 * Initialize the input capture interface, which
 * will be used to take the measurement.
 * @param none
 * @return none
 * @brief  Initialize ultrasonic sensor interface
 */
void Ultrasound_Init(void);

/**
 * Start a measurement using the ultrasonic sensor.
 * If a measurement is currently in progress, return
 * immediately.
 * @param none
 * @return none
 * @note Assumes Ultrasound_Init() has been called<br>
 * @note Assumes Clock_Init48MHz() has been called
 * @brief  Start an ultrasonic distance measurement
 */
void Ultrasound_Start(void);

/**
 * Query the HC-SR04 ultrasonic distance sensor for a
 * measurement.  If no measurement is currently in
 * progress, start one and return zero immediately.
 * If the measurement is not yet complete, return
 * zero immediately.  If the measurement is complete,
 * store the result in the pointers provided and
 * return one.
 * @param distMm is pointer to store measured distance (units mm)
 * @param distIn is pointer to store measured distance (units 10*in)
 * @return one if measurement is ready and pointers are valid<br>
 *         zero if measurement is not ready and pointers unchanged
 * @note Assumes Ultrasound_Init() has been called<br>
 * @note Assumes Clock_Init48MHz() has been called
 * @brief  End or resume an ultrasonic distance measurement
 */
int Ultrasound_End(uint16_t *distMm, uint16_t *distIn);

#endif /* ULTRASOUND_H_ */
