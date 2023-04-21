/**
 * @file      HDC2080.h
 * @brief     Provide low-level functions that interface HDC2080 humidity/temperature sensor to the robot.
 *            This is derived from HDC2080.cpp HDC2080.h by Brandon Fisher on tinkeringtech.
 * @details   HDC2080 is connected to Ports 4 and 6<br>
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      3/2/2020
 *
<table>
<caption id="HDC2080">HDC2080 on BP-BASSENSORSMKII connected to MSP432</caption>
<tr><th>Signal  <th> HDC2080             <th> MSP432
<tr><td>GND     <td> ground        pin 2 <td> ground
<tr><td>HDC_V+  <td> Q2 powers VDD pin 5 <td> P4.3, output 0 to power the sensor
<tr><td>SDA     <td> HDC2080 SDA   pin 1 <td> P6.4 I2C data SDA
<tr><td>SCL     <td> HDC2080 SCL   pin 6 <td> P6.5 I2C clock SCL
<tr><td>HDC_INT <td> HDC2080 DRDY/INT pin 4 <td> P6.1 alert
</table>

 ******************************************************************************/
/* J1.6 P4.3
RSLK BMP2 (bumper switch 2), switch to ground and software internal pullup
BP-BASSENSORSMKII Sensor HDC_V+
Conflict: cannot use both HDC2080 Temperature/Humidity Sensor and bump switch BMP2
Resolution:
a) To use the HDC2080 Temperature and Humidity Sensor and not BMP2: Make P4.3 a GPIO output low to power the Sensor (BMP2 has no functionality if the switch is pressed or released). The software will not have the functionality of being able to turn off power to HDC2080, because if the software makes P4.3 GPIO output high and the user activates BMP2, then P4.3 output high will be shorted to ground. Do not make P4.3 a GPIO output high and then press BMP2.
b) To use BMP2 and not the HDC2080 Temperature and Humidity Sensor: Make P4.3 a GPIO input from BMP2 (pressing BMP2 temporarily powers HDC2080 Temperature and Humidity Sensor).

J3.23 P6.1
RSLK DISTC (analog input from center distance sensor)
BP-BASSENSORSMKII Sensor HDC_INT
Conflict: you cannot use both HDC_INT on the HDC2080 and RSLK DISTC
this is disconnected by default unless the user attached an analog signal here, which will probably need to be disconnected to use the interrupt pin HDC_INT on the HDC2080 Temperature and Humidity Sensor (disabled by default)

 */
#include <stdint.h>
/*!
 * @defgroup BP-BASSENSORSMKII
 * @brief
 * @{*/

//  Constants for setting measurement resolution
#define FOURTEEN_BIT 0
#define ELEVEN_BIT 1
#define NINE_BIT  2

//  Constants for setting sensor mode
#define TEMP_AND_HUMID 0
#define TEMP_ONLY      1
#define HUMID_ONLY     2
#define ACTIVE_LOW     0
#define ACTIVE_HIGH    1
#define LEVEL_MODE      0
#define COMPARATOR_MODE 1

//  Constants for setting sample rate
#define MANUAL          0
#define TWO_MINS        1
#define ONE_MINS        2
#define TEN_SECONDS     3
#define FIVE_SECONDS    4
#define ONE_HZ          5
#define TWO_HZ          6
#define FIVE_HZ         7

/**
 * Initialize HDC2080 temperature sensor.
 * Runs in continuous mode, so user calls
 * HDC2080_ReadHumidity and HDC2080_ReadTemperature to get most recent measurements
 * @param none
 * @return none
 * @brief Initialize HDC2080
 */
void HDC2080_Init(void);

/**
 * Read last measurement from HDC2080 sensor.
 * This function assume you have previously called HDC2080_Init.
 * @param none
 * @return light
 * @note lux = 0.01*(2^Exponent)*Result
 * @brief return last OPT3001 measurement
 */
/* Upper two bits of the MEASUREMENT_CONFIG register controls
   the temperature resolution*/
void HDC2080_SetTempRes(int resolution);

/**
 * Initialize HDC2080 humidity resolution.
 * @param resolution Bits 5 and 6 of the MEASUREMENT_CONFIG register controls the humidity resolution
 * @return none
 * @brief Humidity resolution HDC2080
 */
void HDC2080_SetHumidRes(int resolution);

/**
 * Initialize HDC2080 measurement mode.
 * @param mode  TEMP_AND_HUMID, TEMP_ONLY, HUMID_ONLY
 * @return none
 * @brief set mode HDC2080
 */
void HDC2080_SetMeasurementMode(int mode);

/**
 * Trigger a HDC2080 measurement .
 * Bit 0 of the MEASUREMENT_CONFIG register can be used to trigger measurement
 * @param none
 * @return none
 * @brief trigger HDC2080
 */
void HDC2080_TriggerMeasurement(void);


/**
 * Reset the HDC2080 .
 * Bit 7 of the CONFIG register can be used to trigger a soft reset
 * @param none
 * @return none
 * @brief reset HDC2080
 */
void HDC2080_Reset(void);


/**
 * Enable interrupts on the HDC2080 .
 * Bit 2 of the CONFIG register can be used to enable/disable the interrupt pin
 * @param none
 * @return none
 * @brief enable interrupts on the HDC2080
 */
void HDC2080_EnableInterrupt(void);


/**
 * Disable interrupts on the HDC2080 .
 * Bit 2 of the CONFIG register can be used to enable/disable the interrupt pin
 * @param none
 * @return none
 * @brief disable interrupts on the HDC2080
 */
void HDC2080_DisableInterrupt(void);



/**
 * Set rate of HDC2080 measurements.
 * Bits 6-4  of the CONFIG register controls the measurement rate
 * @param rate  MANUAL, TWO_MINS, ONE_MINS, TEN_SECONDS, FIVE_SECONDS, ONE_HZ, TWO_HZ, FIVE_HZ
 * @return none
 * @brief set rate HDC2080
 */
void HDC2080_SetRate(int rate);


/**
 * Set interrupt polarity of HDC2080.
 * Bit 1 of the CONFIG register can be used to control the the interrupt pins polarity
 * @param polarity  ACTIVE_LOW ACTIVE_HIGH
 * @return none
 * @brief set polarity HDC2080
 */
void HDC2080_SetInterruptPolarity(int polarity);


/**
 * Set interrupt mode of HDC2080.
 * Bit 0 of the CONFIG register can be used to control  the interrupt pin's mode
 * @param mode  LEVEL_MODE, COMPARATOR_MODE
 * @return none
 * @brief set interrupt mode HDC2080
 */
void HDC2080_SetInterruptMode(int mode);


/**
 * Read interrupt status of HDC2080.
 *     7            6       5          4          3     2   1   0
 * DRDY_STATUS TH_STATUS TL_STATUS HH_STATUS HL_STATUS RES RES RES
 * @param none
 * @return status which is contents of register 4
 * @brief read status HDC2080
 */
uint8_t HDC2080_ReadInterruptStatus(void);


/**
 * Clears the maximum temperature register of HDC2080.
 * @param none
 * @return none
 * @brief clear max temperature
 */
void HDC2080_ClearMaxTemp(void);

/**
 * Clears the maximum humidity register of HDC2080.
 * @param none
 * @return none
 * @brief clear max humidity
 */
void HDC2080_ClearMaxHumidity(void);


/**
 * Reads the maximum temperature register of HDC2080.
 * @param none
 * @return max temp
 * @brief read max temperature
 */
int32_t HDC2080_ReadMaxTemp(void);


/**
 * Reads the maximum humidity register of HDC2080.
 * @param none
 * @return max humidity
 * @brief read humidity temperature
 */
uint32_t HDC2080_ReadMaxHumidity(void);


/**
 * Enables the interrupt pin for comfort zone operation of HDC2080.
 * @param none
 * @return none
 * @brief Enables the interrupt pin
 */
void HDC2080_EnableThresholdInterrupt(void);

/**
 * Disables the interrupt pin for comfort zone operation of HDC2080.
 * @param none
 * @return none
 * @brief Disables the interrupt pin
 */
void HDC2080_DisableThresholdInterrupt(void);


/**
 * Enables the interrupt pin for DRDY operation of HDC2080.
 * @param none
 * @return none
 * @brief Enables the interrupt pin for DRDY operation
 */
void HDC2080_EnableDRDYInterrupt(void);

/**
 * Disables the interrupt pin for DRDY operation of HDC2080.
 * @param none
 * @return none
 * @brief Disables the interrupt pin for DRDY operation
 */
void HDC2080_DisableDRDYInterrupt(void);


/**
 * Reads the  temperature register of HDC2080.
 * @param none
 * @return  temp units 0.1 C
 * @brief read temperature
 */
int32_t HDC2080_ReadTemperature(void);

/**
 * Reads the  humidity register of HDC2080.
 * @param none
 * @return  humidity units 0.1% (ranges from 0 to 1000, 123 means 12.3%
 * @brief read humidity
 */
uint32_t HDC2080_ReadHumidity(void);

/**
 * Enables heater of HDC2080.
 * @param none
 * @return none
 * @brief Enables the heater
 */
void HDC2080_EnableHeater(void);

/**
 * Disables heater of HDC2080.
 * @param none
 * @return none
 * @brief Disables the heater
 */
void HDC2080_DisableHeater(void);
