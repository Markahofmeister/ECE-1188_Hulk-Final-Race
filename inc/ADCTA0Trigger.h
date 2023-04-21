/**
 * @file      ADCTA0Trigger.h
 * @brief     Initialize 14-bit ADC0
 * @details   ADC input, timer trigger interrupting system, 14-bit conversion<br>
 * With timer-triggered sampling, there is no jitter.
 * The ADC14 allows two possible references: 2.5V or 3.3V.
 * The internal 2.5V reference is lower noise, but limits the range to 0 to 2.5V.>
 * The other possibility is to use the analog voltage supply at 3.3V,
 * making the ADC range 0 to 3.3V. In this driver, the 2.5V range is selected.<br>
 * - sample just P4.7/A6 <br>
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      July 11, 2019
<table>
<caption id="ADCpin">ADC pin used for this example</caption>
<tr><th>Pin  <th>ADC channel
<tr><td>P4.7 <td>A6
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
 * @defgroup ADC14
 * @brief
 * @{*/

/**
 * Activate Timer A0 to periodically trigger ADC conversions
 * on P4.7 = A6. Run the user task when each conversion is
 * complete.
 * @param task is a pointer to a user function called when ADC result ready
 * @param period in units (1/(bus clock)/div), 16 bits
 * @return none
 * @brief Initialize timer-triggered ADC sampling
 */
void ADC0_InitTA0TriggerCh6(void(*task)(uint16_t result), uint16_t period);
