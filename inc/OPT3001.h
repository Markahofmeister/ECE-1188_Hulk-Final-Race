/**
 * @file      OPT3001.h
 * @brief     Provide low-level functions that interface OPT3001 to the TI-RSLK MAX robot.
 * @details   OPT3001 is connected to Ports 4 and 6<br>
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      3/1/2020
 *
<table>
<caption id="OPT3001">OPT3001 on BP-BASSENSORSMKII connected to MSP432</caption>
<tr><th>Signal    <th> OPT3001             <th>MSP432
<tr><td>GND       <td> ground              <td> ground
<tr><td>TMP_V+    <td> OPT3001 V+    pin 5 <td> P4.7 power to sensor
<tr><td>SDA       <td> OPT3001 SDA   pin 6 <td> P6.4 I2C data SDA
<tr><td>SCL       <td> OPT3001 SCL   pin 1 <td> P6.5 I2C clock SCL
<tr><td>TMP_ALERT <td> OPT3001 ALERT pin 3 <td> P4.4 open drain alert
</table>

 ******************************************************************************/
/* J3.25 P4.2
RSLK BMP1 (bumper switch 1), switch to ground and software internal pullup
BP-BASSENSORSMKII Sensor OPT_INT
because the INT pin functionality on OPT3001 Ambient Light Sensor is open-drain, there is not an electrical conflict between the signals

J3.27 P4.5
RSLK BMP3 (bumper switch 3), switch to ground and software internal pullup
BP-BASSENSORSMKII Sensor OPT_V+
this one is different from the other power-down configurations because the OPT3001 is powered directly from P4.5, which works because of its very low quiescent current (3.7 µA)
a) To use the OPT3001 Ambient Light Sensor and not BMP3-5: Un-bolt and remove the left bump switch assembly (BMP3-5) and make P4.5 a GPIO output high to power the Sensor. Warning: Do not make P4.5 a GPIO output high and then press BMP3.
b) To use BMP3 and not the OPT3001 Ambient Light Sensor: Make P4.5 a GPIO input from BMP3 (pressing BMP3 temporarily un-powers OPT3001 Ambient Light Sensor)

 */
/*!
 * @defgroup BP-BASSENSORSMKII
 * @brief
 * @{*/

#include <stdint.h>

typedef union {
  uint16_t rawData;
  struct{
    uint16_t Result : 12;
    uint8_t Exponent : 4;
  };
} OPT3001_Result;

/**
 * Initialize OPT3001 temperature sensor.
 * Runs in continuous mode, so user calls
 * OPT3001_ReadLight to get most recent measurement
 * @param none
 * @return none
 * @brief Initialize OPT3001
 */
void OPT3001_Init(void);

/**
 * Read last measurement from OPT3001 sensor.
 * This function assume you have previously called OPT3001_Init.
 * @param none
 * @return light
 * @note lux = 0.01*(2^Exponent)*Result
 * @brief return last OPT3001 measurement
 */
OPT3001_Result OPT3001_ReadLight(void);
