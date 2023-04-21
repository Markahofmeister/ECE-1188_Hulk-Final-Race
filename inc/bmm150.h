/**
* Copyright (c) 2020 Bosch Sensortec GmbH. All rights reserved.
*
* BSD-3-Clause
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* 3. Neither the name of the copyright holder nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* @file bmm150.h
* @date 10/01/2020
* @version  1.0.3
*
*
<table>
<caption id="BMM150">BMI160/BMM150 on BP-BASSENSORSMKII connected to MSP432</caption>
<tr><th>Signal <th> BMI160       <th> MSP432
<tr><td>SDO    <td>BMI160 pin 1  <td> 3.3V (I2C address = 0x69)
<tr><td>ASDx   <td>BMI160 pin 2  <td> P6.4 I2C data SDA_S
<tr><td>ASCx   <td>BMI160 pin 3  <td> P6.5 I2C clock SCL_S
<tr><td>INT1   <td>BMI160 pin 4  <td> P4.1 input from the interrupt pin
<tr><td>VDDIO  <td>BMI160 pin 5  <td> 3.3V
<tr><td>GNDIO  <td>BMI160 pin 6  <td> ground
<tr><td>GND    <td>BMI160 pin 7  <td> ground
<tr><td>VDD    <td>BMI160 pin 8  <td> 3.3V
<tr><td>INT2   <td>BMI160 pin 9  <td> P5.0/ERB cannot be used on RSLK due to conflict with right shaft encoder
<tr><td>OCSB   <td>BMI160 pin 10 <td> not connected (unused SPI for slave)
<tr><td>OSDO   <td>BMI160 pin 11 <td> not connected (unused SPI for slave)
<tr><td>CSB    <td>BMI160 pin 12 <td> 3.3V (protocol select = I2C)
<tr><td>SCX    <td>BMI160 pin 13 <td> I2C clock to BMM150 Three-Axis Geomagnetic Sensor slave (I2C address = 0x13)
<tr><td>SDX    <td>BMI160 pin 14 <td> I2C data to BMM150 Three-Axis Geomagnetic Sensor slave (I2C address = 0x13)
</table>
*/

/*! @file bmm150.h */

/*!
 * @defgroup BP-BASSENSORSMKII
 * @{*/

#ifndef BMM150_H__
#define BMM150_H__

/*! CPP guard */
#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************/
/* header files */

#include "bmm150_defs.h"

/********************************************************************/
/* (extern) variable declarations */
/********************************************************************/
/* function prototype declarations */

/*!
 *  @brief This API is the entry point, Call this API before using other APIs.
 *  This API reads the chip-id of the sensor which is the first step to
 *  verify the sensor and also it configures the read mechanism of SPI and
 *  I2C interface.
 *
 *  @param[in,out] dev : Structure instance of bmm150_dev
 *  @note : Refer user guide for detailed info.
 *
 *  @return Result of API execution status
 *  @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_init(struct bmm150_dev *dev);

/*!
 * @brief This API writes the given data to the register address
 * of the sensor.
 *
 * @param[in] reg_addr : Register address from where the data to be written.
 * @param[in] reg_data : Pointer to data buffer which is to be written
 *                       in the reg_addr of sensor.
 * @param[in] len      : No of bytes of data to write..
 * @param[in] dev      : Structure instance of bmm150_dev.
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_set_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, const struct bmm150_dev *dev);

/*!
 * @brief This API reads the data from the given register address of sensor.
 *
 * @param[in] reg_addr  : Register address from where the data to be read
 * @param[out] reg_data : Pointer to data buffer to store the read data.
 * @param[in] len       : No of bytes of data to be read.
 * @param[in] dev       : Structure instance of bmm150_dev.
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint8_t len, const struct bmm150_dev *dev);

/*!
 * @brief This API is used to perform soft-reset of the sensor
 * where all the registers are reset to their default values except 0x4B.
 *
 * @param[in] dev       : Structure instance of bmm150_dev.
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_soft_reset(const struct bmm150_dev *dev);

/*!
 * @brief This API is used to set the power mode of the sensor.
 *
 * @param[in] dev       : Structure instance of bmm150_dev.
 *
 * dev->settings.pwr_mode  |  Power mode
 * ------------------------|-----------------------
 *   0x00                  |   BMM150_NORMAL_MODE
 *   0x01                  |   BMM150_FORCED_MODE
 *   0x03                  |   BMM150_SLEEP_MODE
 *   0x04                  |   BMM150_SUSPEND_MODE
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_set_op_mode(struct bmm150_dev *dev);

/*!
 * @brief This API is used to get the power mode of the sensor.
 *
 * @param[out] op_mode  : power mode of the sensor.
 * @param[in] dev       : Structure instance of bmm150_dev.
 *
 *   op_mode     |  Power mode
 *  -------------|-----------------------
 *   0x00        |   BMM150_NORMAL_MODE
 *   0x01        |   BMM150_FORCED_MODE
 *   0x03        |   BMM150_SLEEP_MODE
 *   0x04        |   BMM150_SUSPEND_MODE
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_get_op_mode(uint8_t *op_mode, const struct bmm150_dev *dev);

/*!
 * @brief This API is used to set the preset mode of the sensor.
 *
 * @param[in] dev       : Structure instance of bmm150_dev.
 *
 *   dev->settings.preset_mode |  Preset mode
 *  ---------------------------|----------------------------------
 *   0x01                      |   BMM150_PRESETMODE_LOWPOWER
 *   0x02                      |   BMM150_PRESETMODE_REGULAR
 *   0x03                      |   BMM150_PRESETMODE_HIGHACCURACY
 *   0x04                      |   BMM150_PRESETMODE_ENHANCED
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_set_presetmode(struct bmm150_dev *dev);

/*!
 * @brief This API sets the sensor settings based on the desired_settings
 * and the dev structure configuration
 *
 * @param[in] desired_settings  : Selection macro for selecting the setting.
 * @param[in] dev               : Structure instance of bmm150_dev.
 *
 * @note Assign the sensor setting macros (multiple macros can be
 * set by doing a bitwise-OR operation) to the desired_settings parameter
 * of this API to perform the corresponding setting.
 *
 * @note threshold interrupt for each axes are set by using bitwise AND
 * operation of the following macros
 *  - BMM150_THRESHOLD_X
 *  - BMM150_THRESHOLD_Y
 *  - BMM150_THRESHOLD_Z
 *
 *   desired_settings  |  Selected sensor setting macros
 *  -------------------|--------------------------------
 *   0x0001            |  BMM150_DATA_RATE_SEL
 *   0x0002            |  BMM150_CONTROL_MEASURE_SEL
 *   0x0004            |  BMM150_XY_REP_SEL
 *   0x0008            |  BMM150_Z_REP_SEL
 *   0x0010            |  BMM150_DRDY_PIN_EN_SEL
 *   0x0020            |  BMM150_INT_PIN_EN_SEL
 *   0x0040            |  BMM150_DRDY_POLARITY_SEL
 *   0x0080            |  BMM150_INT_LATCH_SEL
 *   0x0100            |  BMM150_INT_POLARITY_SEL
 *   0x0200            |  BMM150_DATA_OVERRUN_INT_SEL
 *   0x0400            |  BMM150_OVERFLOW_INT_SEL
 *   0x0800            |  BMM150_HIGH_THRESHOLD_INT_SEL
 *   0x1000            |  BMM150_LOW_THRESHOLD_INT_SEL
 *   0x2000            |  BMM150_LOW_THRESHOLD_SETTING_SEL
 *   0x4000            |  BMM150_HIGH_THRESHOLD_SETTING_SEL
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_set_sensor_settings(uint16_t desired_settings, const struct bmm150_dev *dev);

/*!
 * @brief This API gets all the sensor settings and updates the dev structure
 *
 * @param[in] dev       : Structure instance of bmm150_dev.
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_get_sensor_settings(struct bmm150_dev *dev);

/*!
 * @brief This API reads the magnetometer data from registers 0x42 to 0x49
 * and updates the dev structure with compensated mag data in micro-tesla
 *
 * @param[in,out] dev     :   Structure instance of bmm150_dev.
 *
 * @note The output mag data can be obtained either in int16_t or float format
 *       using this API.
 * @note Enable the macro "BMM150_USE_FLOATING_POINT" in the bmm150_defs.h
 *       file and call this API to get the mag data in float,
 *       disable this macro to get the mag data in int16_t format
 *
 *  Mag data output(micro-tesla)   |  Mag data in dev structure(int16_t/float)
 * --------------------------------|------------------------------------------
 *   X-axis data                   | dev->data.x
 *   Y-axis data                   | dev->data.y
 *   Z-axis data                   | dev->data.z
 *
 * @return Result of API execution status
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_read_mag_data(struct bmm150_dev *dev);

/*!
 * @brief This API is used to perform the complete self test
 * (both normal and advanced) for the BMM150 sensor
 *
 * @param[in] self_test_mode    : The type of self test to be performed
 * @param[in] dev               : Structure instance of bmm150_dev.
 *
 *   self_test_mode    |  Self test enabled
 * --------------------|--------------------------
 *      0              | BMM150_NORMAL_SELF_TEST
 *      1              | BMM150_ADVANCED_SELF_TEST
 *
 * @note The return value of this API gives us the result of self test.
 *
 * @note Performing advanced self test does soft reset of the sensor, User can
 * set the desired settings after performing the advanced self test.
 *
 * @return Result of API execution status and self test result.
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 *
 *   Return value      | Result of self test
 * --------------------|---------------------------------
 *      0              | BMM150_OK
 *      1              | BMM150_W_NORMAL_SELF_TEST_YZ_FAIL
 *      2              | BMM150_W_NORMAL_SELF_TEST_XZ_FAIL
 *      3              | BMM150_W_NORMAL_SELF_TEST_Z_FAIL
 *      4              | BMM150_W_NORMAL_SELF_TEST_XY_FAIL
 *      5              | BMM150_W_NORMAL_SELF_TEST_Y_FAIL
 *      6              | BMM150_W_NORMAL_SELF_TEST_X_FAIL
 *      7              | BMM150_W_NORMAL_SELF_TEST_XYZ_FAIL
 *      8              | BMM150_W_ADV_SELF_TEST_FAIL
 *
 */
int8_t bmm150_perform_self_test(uint8_t self_test_mode, struct bmm150_dev *dev);

/*!
 * @brief This API obtains the status flags of all interrupt
 * which is used to check for the assertion of interrupts
 *
 * @param[in,out] dev       : Structure instance of bmm150_dev.
 *
 * @note The status flags of all the interrupts are stored in the
 * dev->int_status.
 *
 * @note The value of dev->int_status is performed a bitwise AND operation
 * with predefined interrupt status macros to find the interrupt status
 * which is either set or reset.
 *
 * Ex.
 * if (dev->int_status & BMM150_DATA_READY_INT)
 * {
 *  Occurrence of data ready interrupt
 * } else {
 *  No interrupt occurred
 * }
 *
 * @return Result of API execution status and self test result.
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_get_interrupt_status(struct bmm150_dev *dev);

/*!
 * @brief This API is used to compensate the raw mag data
 *
 * @param[in] aux_data      : Raw mag data obtained from BMI160 registers
 * @param[in,out] dev       : Structure instance of bmm150_dev.
 *
 * @return Result of API execution status and self test result.
 * @retval zero -> Success / +ve value -> Warning / -ve value -> Error
 */
int8_t bmm150_aux_mag_data(uint8_t *aux_data, struct bmm150_dev *dev);

#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif /* BMM150_H__ */
/** @}*/
