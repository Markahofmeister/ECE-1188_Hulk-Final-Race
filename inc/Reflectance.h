/**
 * @file      Reflectance.h
 * @brief     Provide functions for the QTR-8RC reflectance sensor array. Pololu part number 3672.
 * @details   Provide functions to take measurements using a QTRX reflectance
 * sensor array (Pololu part number 3672).  This works by outputting to
 * the sensor, waiting, then reading the digital value of each of the
 * eight phototransistors.  The more reflective the target surface is,
 * the faster the voltage decays. The time to wait is a function of<br>
 1) reflectance of surface<br>
 2) electrical resistance and capacitance of the GPIO pin<br>
 3) distance to surface<br>
 * For these reasons, you will have to experimentally verify which
 * time works best for your system.
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      June 28, 2019
 *
<table>
<caption id="QTR_ports">QTRX reflectance sensor array</caption>
<tr><th>Pin   <th>QTRX function
<tr><td>P5.3  <td>reflectance IR LED output
<tr><td>P7.0  <td>reflectance sensor 1 right, robot off road to left
<tr><td>P7.1  <td>reflectance sensor 2
<tr><td>P7.2  <td>reflectance sensor 3
<tr><td>P7.3  <td>reflectance sensor 4 center
<tr><td>P7.4  <td>reflectance sensor 5 center
<tr><td>P7.5  <td>reflectance sensor 6
<tr><td>P7.6  <td>reflectance sensor 7
<tr><td>P7.7  <td>reflectance sensor 8 left, robot off road to right
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
#ifndef REFLECTANCE_H_
#define REFLECTANCE_H_


/**
 * Initialize the GPIO pins associated with the QTR-8RC.
 * One output to IR LED, 8 inputs from the sensor array.
 * Initially, the IR outputs are off.
 * @param  none
 * @return none
 * @brief  Initialize the GPIO pins for the QTR-8RC reflectance sensor.
 */
void Reflectance_Init(void);


/**
 * <b>Read the eight sensors</b>:<br>
  1) Turn on the 8 IR LEDs<br>
  2) Pulse the 8 sensors high for 10 us<br>
  3) Make the sensor pins input<br>
  4) Wait <b>time</b> us<br>
  5) Read sensors (white is 0, black is 1)<br>
  6) Turn off the 8 IR LEDs<br>
 * @param  time delay value in us
 * @return 8-bit result
 * @note Assumes Reflectance_Init() has been called
 * @brief  Read the eight sensors.
 */
uint8_t Reflectance_Read(uint32_t time);

/**
 * <b>Read the two center sensors</b>:<br>
  1) Turn on the 8 IR LEDs<br>
  2) Pulse the 8 sensors high for 10 us<br>
  3) Make the sensor pins input<br>
  4) Wait <b>time</b> us<br>
  5) Read sensors (white is 0, black is 1)<br>
  6) Turn off the 8 IR LEDs<br>
  <table>
<caption id="QTR_center">Two bit output from QTR-8RC reflectance sensor array</caption>
<tr><th>(Left,Right) <th>Sensors        <th>Meaning
<tr><td>1,1          <td>both sensors   <td>on line
<tr><td>1,0          <td>left left      <td>off to right
<tr><td>0,1          <td>just right     <td>off to left
<tr><td>0,0          <td>neither        <td>lost
</table>
 * @param  time delay value in us
 * @return 2-bit result, 0 (off road), 1 off to left, 2 off to right, 3 on road
 * @note Assumes Reflectance_Init() has been called
 * @brief  Read the two center sensors.
*/
uint8_t Reflectance_Center(uint32_t time);


/**
 * <b>Calculate the weighted average for each bit</b>:<br>
 * Position varies from -332 (left) to +332 (right), with units of 0.1mm.<br>
<table>
<caption id="QTR_distance">8 element arrays</caption>
<tr><th>  <th>bit7<th>bit6<th>bit5<th>bit4<th>bit3<th>bit2<th>bit1<th>bit0
<tr><td>Weight<td>-332<td>-237<td>-142<td> -47<td>  47<td> 142<td> 237<td> 332
<tr><td>Mask  <td>0x01<td>0x02<td>0x04<td>0x08<td>0x10<td>0x20<td>0x40<td>0x80
</table>
 * count = 0<br>
 * sum = 0<br>
 * for i from 0 to 7 <br>
 * if (data&Mask[i]) then count++ and sum = sum+Weight[i]<br>
 * calculate <b>position</b> = sum/count
 * @param  data is 8-bit result from line sensor
 * @return position in 0.1mm relative to center of line
 * @brief  Perform sensor integration.
 * @note returns 333 if data is zero (off the line)
 * */
int32_t Reflectance_Position(uint8_t data);

/**
 * <b>Begin the process of reading the eight sensors</b>:<br>
  1) Turn on the 8 IR LEDs<br>
  2) Pulse the 8 sensors high for 10 us<br>
  3) Make the sensor pins input<br>
 * @param  none
 * @return none
 * @note Assumes Reflectance_Init() has been called
 * @brief  Beging reading the eight sensors.
 */
void Reflectance_Start(void);


/**
 * <b>Finish reading the eight sensors</b>:<br>
  5) Read sensors (white is 0, black is 1)<br>
  6) Turn off the 8 IR LEDs<br>
 * @param  none
 * @return 8-bit result
 * @note Assumes Reflectance_Init() has been called
 * @note Assumes Reflectance_Start() was called 1 ms ago
 * @brief  Read the eight sensors.
 */
uint8_t Reflectance_End(void);

/**
 * <b>Return last reading</b>
 * @param  none
 * @return 8-bit result
 * @note  Assumes: Reflectance_Init() has been called
 * @note  Assumes: Reflectance_Start() was called
 * @note  Assumes: Reflectance_End() was called 1 ms after Start
 * @brief  Get last reading of the eight sensors.
 */
uint8_t Reflectance_Get(void);

#endif /* REFLECTANCE_H_ */
