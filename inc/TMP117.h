/**
 * @file      TMP117.h
 * @brief     Provide low-level functions that interface TMP117 to the TI-RSLK MAX robot.
 * @details   TMP117 is connected to Ports 4 and 6<br>
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      2/29/2020
 *
<table>
<caption id="TMP117">TMP117 on BP-BASSENSORSMKII connected to MSP432</caption>
<tr><th>Signal    <th> TMP117 <th>MSP432
<tr><td>GND       <td> ground             <td> ground
<tr><td>TMP_V+    <td> TMP117 V+    pin 5 <td> P4.7 power to sensor
<tr><td>SDA       <td> TMP117 SDA   pin 6 <td> P6.4 I2C data SDA
<tr><td>SCL       <td> TMP117 SCL   pin 1 <td> P6.5 I2C clock SCL
<tr><td>TMP_ALERT <td> TMP117 ALERT pin 3 <td> P4.4 open drain alert
</table>

 ******************************************************************************/
#include <stdint.h>
/*!
 * @defgroup BP-BASSENSORSMKII
 * @brief
 * @{*/

/**
 * Initialize TMP117 temperature sensor.
 * Runs in continuous mode, so user calls
 * TMP117_ReadTemperature to get most recent measurement
 * @param none
 * @return none
 * @brief Initialize TMP117
 */
void TMP117_Init(void);

/**
 * Initialize and activate TMP117 alarm interrupts to run user task when new data ready.
 * This function calls TMP117_Init, uses ONE-SHOT mode.
 * @param task is a pointer to a user function called when data ready
 * @return none
 * @note  Units of temperature are  0.0078125 C, range is -256 to +255 C
 * @brief Initialize TMP117 with interrupts
 */
void TMP117_Arm(void (*pt)(int16_t t));

/**
 * Read last measurement from TMP117 sensor.
 * This function assume you have previously called TMP117_Init.
 * @param none
 * @return temperature
 * @note  Units of temperature are 0.0078125 C, range is -256 to +255 C
 * @brief return last TMP117 measurement
 */
int16_t TMP117_ReadTemperature(void);
