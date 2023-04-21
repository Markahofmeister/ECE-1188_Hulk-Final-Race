// HDC2080.c
// Runs on MSP432, RSLK
// Provide low-level functions that interface HDC2080 humidity/temperature sensor to the robot.
// derived from HDC2080.cpp HDC2080.h by Brandon Fisher downloaded from tinkeringtech
// edited for MSP432 by Valvano and Valvano 3/2/2020

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2020
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2020, Jonathan Valvano, All rights reserved.

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

#include <stdint.h>
#include <msp.h>
#include "../inc/HDC2080.h"
#include "../inc/I2CB1.h"
#include "../inc/Clock.h"

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
// HDC2080 hardware from BP-BASSENSORSMKII Sensor Booster Pack
// GND     ground        pin 2
// HDC_V+  HDC2080 VDD   pin 5 <- P4.3 output 0 to power the sensor
// SDA     HDC2080 SDA   pin 1 <> P6.4 I2C data SDA
// SCL     HDC2080 SCL   pin 6 <- P6.5 I2C clock SCL
// HDC_INT HDC2080 INT   pin 4 -> P6.1 interrupt
// ADDR    HDC2080 ADDR  pin 3 = ground to set address 0x40
//

#define HDC2080_ADDRESS 0x40
//Define Register Map
#define TEMP_LOW 0x00
#define TEMP_HIGH 0x01
#define HUMID_LOW 0x02
#define HUMID_HIGH 0x03
#define INTERRUPT_DRDY 0x04
#define TEMP_MAX 0x05
#define HUMID_MAX 0x06
#define INTERRUPT_CONFIG 0x07
#define TEMP_OFFSET_ADJUST 0x08
#define HUM_OFFSET_ADJUST 0x09
#define TEMP_THR_L 0x0A
#define TEMP_THR_H 0x0B
#define HUMID_THR_L 0x0C
#define HUMID_THR_H 0x0D
#define CONFIG 0x0E
#define MEASUREMENT_CONFIG 0x0F
#define MID_L 0xFC
#define MID_H 0xFD
#define DEVICE_ID_L 0xFE
#define DEVICE_ID_H 0xFF


uint8_t static readRegister(uint8_t reg){
  I2CB1_Send(HDC2080_ADDRESS, &reg, 1);
  uint8_t data;
  data = I2CB1_Recv1(HDC2080_ADDRESS);
  return data;
}

void static writeRegister(uint8_t reg, uint8_t data){
  uint8_t buffer[] = {
     reg,
     data
    };
  I2CB1_Send(HDC2080_ADDRESS, buffer, sizeof(buffer));
}


/* Upper two bits of the MEASUREMENT_CONFIG register controls
   the temperature resolution*/
void HDC2080_SetTempRes(int resolution){
  uint8_t configContents;
  configContents = readRegister(MEASUREMENT_CONFIG);

  switch(resolution){
    case FOURTEEN_BIT:
      configContents = (configContents & 0x3F);
      break;

    case ELEVEN_BIT:
      configContents = (configContents & 0x7F);
      configContents = (configContents | 0x40);
      break;

    case NINE_BIT:
      configContents = (configContents & 0xBF);
      configContents = (configContents | 0x80);
      break;

    default:
      configContents = (configContents & 0x3F);
  }

  writeRegister(MEASUREMENT_CONFIG, configContents);

}
/*  Bits 5 and 6 of the MEASUREMENT_CONFIG register controls
    the humidity resolution*/
void HDC2080_SetHumidRes(int resolution){
  uint8_t configContents;
  configContents = readRegister(MEASUREMENT_CONFIG);

  switch(resolution)    {
    case FOURTEEN_BIT:
      configContents = (configContents & 0xCF);
      break;

    case ELEVEN_BIT:
      configContents = (configContents & 0xDF);
      configContents = (configContents | 0x10);
      break;

    case NINE_BIT:
      configContents = (configContents & 0xEF);
      configContents = (configContents | 0x20);
      break;

    default:
      configContents = (configContents & 0xCF);
  }

  writeRegister(MEASUREMENT_CONFIG, configContents);
}
void HDC2080_SetMeasurementMode(int mode){
  uint8_t configContents;
  configContents = readRegister(MEASUREMENT_CONFIG);

  switch(mode){
    case TEMP_AND_HUMID:
      configContents = (configContents & 0xF9);
      break;

    case TEMP_ONLY:
      configContents = (configContents & 0xFC);
      configContents = (configContents | 0x02);
      break;

    case HUMID_ONLY:
      configContents = (configContents & 0xFD);
      configContents = (configContents | 0x04);
      break;

    default:
      configContents = (configContents & 0xF9);
  }
  writeRegister(MEASUREMENT_CONFIG, configContents);
}

/*  Bit 0 of the MEASUREMENT_CONFIG register can be used
    to trigger measurements  */
void HDC2080_TriggerMeasurement(void){
  uint8_t configContents;
  configContents = readRegister(MEASUREMENT_CONFIG);

  configContents = (configContents | 0x01);
  writeRegister(MEASUREMENT_CONFIG, configContents);
}

/*  Bit 7 of the CONFIG register can be used to trigger a
    soft reset  */
void HDC2080_Reset(void){
  uint8_t configContents;
  configContents = readRegister(CONFIG);

  configContents = (configContents | 0x80);
  writeRegister(CONFIG, configContents);
  Clock_Delay1ms(1); // needed??
}

/*  Bit 2 of the CONFIG register can be used to enable/disable
    the interrupt pin  */
void HDC2080_EnableInterrupt(void){
  uint8_t configContents;
  configContents = readRegister(CONFIG);

  configContents = (configContents | 0x04);
  writeRegister(CONFIG, configContents);
}

/*  Bit 2 of the CONFIG register can be used to enable/disable
    the interrupt pin  */
void HDC2080_DisableInterrupt(void){
  uint8_t configContents;
  configContents = readRegister(CONFIG);
  configContents = (configContents & 0xFB);
  writeRegister(CONFIG, configContents);
}


/*  Bits 6-4  of the CONFIG register controls the measurement
    rate  */
void HDC2080_SetRate(int rate){
  uint8_t configContents;
  configContents = readRegister(CONFIG);
  switch(rate){
    case MANUAL:
      configContents = (configContents & 0x8F);
      break;

    case TWO_MINS:
      configContents = (configContents & 0x9F);
      configContents = (configContents | 0x10);
    break;

    case ONE_MINS:
      configContents = (configContents & 0xAF);
      configContents = (configContents | 0x20);
      break;

    case TEN_SECONDS:
      configContents = (configContents & 0xBF);
      configContents = (configContents | 0x30);
      break;

    case FIVE_SECONDS:
      configContents = (configContents & 0xCF);
      configContents = (configContents | 0x40);
      break;

    case ONE_HZ:
      configContents = (configContents & 0xDF);
      configContents = (configContents | 0x50);
      break;

    case TWO_HZ:
      configContents = (configContents & 0xEF);
      configContents = (configContents | 0x60);
      break;

    case FIVE_HZ:
      configContents = (configContents | 0x70);
      break;

    default:
      configContents = (configContents & 0x8F);
    }

    writeRegister(CONFIG, configContents);
}

/*  Bit 1 of the CONFIG register can be used to control the
    the interrupt pins polarity */
void HDC2080_SetInterruptPolarity(int polarity){
  uint8_t configContents;
  configContents = readRegister(CONFIG);

  switch(polarity) {
    case ACTIVE_LOW:
      configContents = (configContents & 0xFD);
      break;

    case ACTIVE_HIGH:
      configContents = (configContents | 0x02);
      break;

   default:
      configContents = (configContents & 0xFD);
  }
  writeRegister(CONFIG, configContents);
}

/*  Bit 0 of the CONFIG register can be used to control the
    the interrupt pin's mode */
void HDC2080_SetInterruptMode(int mode){
  uint8_t configContents;
  configContents = readRegister(CONFIG);
  switch(mode){
    case LEVEL_MODE:
      configContents = (configContents & 0xFE);
      break;

    case COMPARATOR_MODE:
      configContents = (configContents | 0x01);
      break;

    default:
      configContents = (configContents & 0xFE);
  }
  writeRegister(CONFIG, configContents);
}


uint8_t HDC2080_ReadInterruptStatus(void){
  uint8_t regContents;
  regContents = readRegister(INTERRUPT_DRDY);
  return regContents;
}

//  Clears the maximum temperature register
void HDC2080_ClearMaxTemp(void){
    writeRegister(TEMP_MAX, 0x00);
}

//  Clears the maximum humidity register
void HDC2080_ClearMaxHumidity(void){
  writeRegister(HUMID_MAX, 0x00);
}

//  Reads the maximum temperature register
int32_t HDC2080_ReadMaxTemp(void){
  uint8_t regContents;
  regContents = readRegister(TEMP_MAX);
  return (1650*regContents)/256 - 400;
}

//  Reads the maximum humidity register
uint32_t HDC2080_ReadMaxHumidity(void){
  uint8_t regContents;
  regContents = readRegister(HUMID_MAX);
  return (1000*regContents)/256;
}


// Enables the interrupt pin for comfort zone operation
void HDC2080_EnableThresholdInterrupt(void){
  uint8_t regContents;
  regContents = readRegister(INTERRUPT_CONFIG);
  regContents = (regContents | 0x78);
  writeRegister(INTERRUPT_CONFIG, regContents);
}

// Disables the interrupt pin for comfort zone operation
void HDC2080_DisableThresholdInterrupt(void){
  uint8_t regContents;
  regContents = readRegister(INTERRUPT_CONFIG);
  regContents = (regContents & 0x87);
  writeRegister(INTERRUPT_CONFIG, regContents);
}

// enables the interrupt pin for DRDY operation
void HDC2080_EnableDRDYInterrupt(void){
  uint8_t regContents;
  regContents = readRegister(INTERRUPT_CONFIG);
  regContents = (regContents | 0x80);
  writeRegister(INTERRUPT_CONFIG, regContents);
}

// disables the interrupt pin for DRDY operation
void HDC2080_DisableDRDYInterrupt(void){
  uint8_t regContents;
  regContents = readRegister(INTERRUPT_CONFIG);
  regContents = (regContents & 0x7F);
  writeRegister(INTERRUPT_CONFIG, regContents);
}


void HDC2080_Init(void){
  // Drive P4.3 low to power the HDC2080.
  P4->SEL0 &= ~0x08;
  P4->SEL1 &= ~0x08;  // 1) configure P4.3 as GPIO
  P4->DIR |= 0x08;
  P4->OUT &= ~0x08;
  P6->DIR &= ~0x02;  // P6.1 is HDC_INT
  Clock_Delay1ms(1); // needed??
  HDC2080_Reset();

  // Configure Measurements
  HDC2080_SetMeasurementMode(TEMP_AND_HUMID);  // Set measurements to temperature and humidity
  HDC2080_SetRate(ONE_HZ);                     // Set measurement frequency to 1 Hz
  HDC2080_SetTempRes(FOURTEEN_BIT);
  HDC2080_SetHumidRes(FOURTEEN_BIT);

  //begin measuring
  HDC2080_TriggerMeasurement();
}
// units 0.1 C
int32_t HDC2080_ReadTemperature(void){
  uint8_t msb,lsb;
  uint32_t temp;
  lsb = readRegister(TEMP_LOW);
  msb = readRegister(TEMP_HIGH);
  temp = (msb << 8) | lsb;
  return (temp * 1650) / 65536 - 400;
}
// units 0.1% (ranges from 0 to 1000, 123 means 12.3%
uint32_t HDC2080_ReadHumidity(void){
  uint8_t msb,lsb;
  uint32_t humidity;
  lsb = readRegister(HUMID_LOW);
  msb = readRegister(HUMID_HIGH);
  humidity = (msb << 8) | lsb;
  return (1000*humidity)/(65536);
}

void HDC2080_EnableHeater(void){
  uint8_t configContents; //Stores current contents of config register
  configContents = readRegister(CONFIG);
  //set bit 3 to 1 to enable heater
  configContents = (configContents | 0x08);
  writeRegister(CONFIG, configContents);
}

void HDC2080_DisableHeater(void){
  uint8_t configContents; //Stores current contents of config register
  configContents = readRegister(CONFIG);
  //set bit 3 to 0 to disable heater (all other bits 1)
  configContents = (configContents & 0xF7);
  writeRegister(CONFIG, configContents);
}

