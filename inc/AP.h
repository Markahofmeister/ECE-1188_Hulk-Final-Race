/**
 * @file      AP.h
 * @brief     Bluetooth Low Energy Application Processor
 * @details   Abstraction layer for implementing BLE
 * @remark    see GPIO.c file for hardware connections
 * @remark    The CC2650 is running with SimpleNP 2.2 (Powersave, MRDY, SRDY signals)
 * @remark    Tested with CC2650 BoosterPack with these two object codes:
 *   cc2650bp_simple_np_uart_pm_xsbl_mooc_custom.hex, and
 *   simple_np_cc2650bp_uart_pm_sbl.hex
 * @remark    Tested with CC2650 LaunchPad with these two object codes:
 *   cc2650lp_simple_np_uart_pm_xsbl_mooc_custom.hex, and
 *   simple_np_cc2650lp_uart_pm_sbl.hex
 * @remark    It doesn't matter if bootloadmode is enabled (sbl) or not enabled (xsbl)
 * @remark    Transmit and receive interrupts are implemented in UART1.c on UCA2.
 * @version   TI-RSLK MAX v1.1
 * @author    Daniel Valvano and Jonathan Valvano
 * @copyright Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu,
 * @warning   AS-IS
 * @note      For more information see  http://users.ece.utexas.edu/~valvano/
 * @date      June 28, 2019
<table>
<caption id="ap_terms">Terminology</caption>
<tr><th>Legend    <th>TI part number
<tr><td>CC2650BP  <td>BOOSTXL-CC2650MA
<tr><td>CC2650LP  <td>LAUNCHXL-CC2650
<tr><td>MSP432    <td>MSP-EXP432P401R
<tr><td>TM4C123   <td>EK-TM4C123GXL
<tr><td>MKII      <td>BOOSTXL-EDUMKII
</table>
 * @note      Options 1,2,3 allow the use of the MKII I/O boosterpack together with the CC2650.
 * Option 4 can be used with TI-RSLK MAX robot

 * @remark   <b>Option 1</b>: the CC2650BP is tethered to the MSP432 using 7 wires.
 * No reprogramming CC2650BP is needed.<br>
 * This configuration has MSP432 plugged into the MKII, and a 7-wire cable to the CC2650BP.<br>
 * For option 1, do NOT include the #define DEFAULT 1.
<table>
<caption id="ap_option1">Option 1</caption>
<tr><th>signal <th>TM4C123/MSP432                   <th>CC2650BP       <th>comment
<tr><td>3V3    <td>J1.1  3.3V                       <td>J1.1  3.3V     <td>Power from MSP432 to CC2650BP
<tr><td>GND    <td>J2.20 ground                     <td>J2.20 ground   <td>Connect ground together
<tr><td>NRESET <td>J4.35 TM4C123 PC6, MSP432 P6.7   <td>J4.35          <td>Reset from MSP432 to CC2650BP
<tr><td>TX     <td>J1.3  TM4C123 PB0, MSP432 P3.2   <td>J1.3  DIO0_TXD <td>UART from CC2650BP to MSP432
<tr><td>RX     <td>J1.4  TM4C123 PB1, MSP432 P3.3   <td>J1.4  DIO1_RXD <td>UART from MSP432 to CC2650BP
<tr><td>MRDY   <td>J2.14 TM4C123 PB6, MSP432 P1.7   <td>J1.2  IOID_7   <td>Master ready from MSP432 to CC2650BP
<tr><td>SRDY   <td>J2.12 TM4C123 PA3, MSP432 P5.2   <td>J2.19 IOID_8   <td>Slave ready from CC2650BP to MSP432
</table>
<br>
 * @remark    <b>Option 2</b>: The CC2650BP is reprogrammed with a custom version of the SNP.
 * You reprogram the CC2650BP with simple_np_cc2650bp_uart_pm_xsbl_mooc_custom.hex.<br>
 * This configuration has a three board stack: CC2650BP+MSP432+MKII<br>
 * In other words these three boards are connected together: BOOSTXL-CC2650MA, MSP-EXP432P401R, and BOOSTXL-EDUMKII.<br>
 * For option 2, do NOT include the #define DEFAULT 1.
<table>
<caption id="ap_option2">Option 2</caption>
<tr><th>signal <th>TM4C123/MSP432                   <th>CC2650BP       <th>comment
<tr><td>3V3    <td>J1.1  3.3V                       <td>J1.1  3.3V     <td>Power from MSP432 to CC2650BP
<tr><td>GND    <td>J2.20 ground                     <td>J2.20 ground   <td>Connect ground together
<tr><td>NRESET <td>J4.35 TM4C123 PC6, MSP432 P6.7   <td>J4.35          <td>Reset from MSP432 to CC2650BP
<tr><td>TX     <td>J1.3  TM4C123 PB0, MSP432 P3.2   <td>J1.3  DIO0_TXD <td>UART from CC2650BP to MSP432
<tr><td>RX     <td>J1.4  TM4C123 PB1, MSP432 P3.3   <td>J1.4  DIO1_RXD <td>UART from MSP432 to CC2650BP
<tr><td>MRDY   <td>J2.14 TM4C123 PB6, MSP432 P1.7   <td>J1.14 DIO8     <td>Master ready from MSP432 to CC2650BP
<tr><td>SRDY   <td>J2.12 TM4C123 PA3, MSP432 P5.2   <td>J2.12 DIO14    <td>Slave ready from CC2650BP to MSP432
</table>
<br>
 * @remark    <b>Option 3</b>: The CC2650LP is reprogrammed with a custom version of the SNP.
 * You reprogram the CC2650LP with simple_np_cc2650lp_uart_pm_xsbl_mooc_custom.hex.<br>
 * This configuration has a three board stack: CC2650LP+MSP432+MKII<br>
 * In other words these three boards are connected together: LAUNCHXL-CC2650, MSP-EXP432P401R, and BOOSTXL-EDUMKII.<br>
 * Remove the Rx and Tx jumpers on CC2650LP.<br>
 * It is optional, but we suggest you also remove the two LED jumpers on Red, Green LEDs on the CC2650 LaunchPad.
 * We also recommend you place insulting tape over the set of 11 jumpers in middle, before stacking.<br>
 * For option 3, do NOT include the #define DEFAULT 1.
<table>
<caption id="ap_option3">Option 3</caption>
<tr><th>signal <th>TM4C123/MSP432                   <th>CC2650LP       <th>comment
<tr><td>3V3    <td>J1.1  3.3V                       <td>J1.1  3.3V     <td>Power from MSP432 to CC2650LP
<tr><td>GND    <td>J2.20 ground                     <td>J2.20 ground   <td>Connect ground together
<tr><td>NRESET <td>J4.35 TM4C123 PC6, MSP432 P6.7   <td>J4.35          <td>Reset from MSP432 to CC2650LP
<tr><td>TX     <td>J1.3  TM4C123 PB0, MSP432 P3.2   <td>J1.3  DIO3_TXD <td>UART from CC2650LP to MSP432
<tr><td>RX     <td>J1.4  TM4C123 PB1, MSP432 P3.3   <td>J1.4  DIO2_RXD <td>UART from MSP432 to CC2650LP
<tr><td>MRDY   <td>J2.14 TM4C123 PB6, MSP432 P1.7   <td>J1.14 DIO12    <td>Master ready from MSP432 to CC2650LP
<tr><td>SRDY   <td>J2.12 TM4C123 PA3, MSP432 P5.2   <td>J2.12 DIO14    <td>Slave ready from CC2650LP to MSP432
</table>
<br>
 * @remark    <b>Option 4</b>: Use this setup with CC2650BP without a MKII.
 * This runs with the default version of SNP that ships on the BOOSTXL-CC2650MA.<br>
 * This configuration has a two board stack: CC2650BP+MSP432<br>
 * In other words these two boards are connected together: BOOSTXL-CC2650MA and MSP-EXP432P401R.<br>
 * This version will work with the TI-RSLK MAX robot
 * For option 4, DO include the #define DEFAULT 1.
<table>
<caption id="ap_option4">Option 4</caption>
<tr><th>signal <th>TM4C123/MSP432                   <th>CC2650BP       <th>comment
<tr><td>3V3    <td>J1.1  3.3V                       <td>J1.1  3.3V     <td>Power from MSP432 to CC2650BP
<tr><td>GND    <td>J2.20 ground                     <td>J2.20 ground   <td>Connect ground together
<tr><td>NRESET <td>J4.35 TM4C123 PC6, MSP432 P6.7   <td>J4.35          <td>Reset from MSP432 to CC2650BP
<tr><td>TX     <td>J1.3  TM4C123 PB0, MSP432 P3.2   <td>J1.3  DIO0_TXD <td>UART from CC2650BP to MSP432
<tr><td>RX     <td>J1.4  TM4C123 PB1, MSP432 P3.3   <td>J1.4  DIO1_RXD <td>UART from MSP432 to CC2650BP
<tr><td>MRDY   <td>J1.2  TM4C123 PB5, MSP432 P6.0   <td>J1.2  IOID_7   <td>Master ready from MSP432 to CC2650BP
<tr><td>SRDY   <td>J2.19 TM4C123 PB2, MSP432 P2.5   <td>J2.19 IOID_8   <td>Slave ready from CC2650BP to MSP432
</table>
<br>

 ******************************************************************************/

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2017, Jonathan Valvano, All rights reserved.

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

// CC2650 booster or CC2650 LaunchPad, CC2650 needs to be running SimpleNP 2.2 (POWERSAVE)
// UART serial from CC2650 to TM4C123/MSP432
// UART serial from TM4C123/MSP432 to CC2650
// MRDY, an output from TM4C123/MSP432 and an input to CC2650
// SRDY, an input to TM4C123/MSP432 and an output from CC2650
// Reset, an output from TM4C123/MSP432 to hardware reset to CC2650
// MRDY is a GPIO pin whose logic level indicates the state of the master processor.
// MRDY is an active low (low true) signal.
// An MRDY event indicates that the master processor is ready to send or receive data.
// SRDY is a GPIO pin whose logic level indicates the state of the slave processor.
// SRDY is an active low (low true) signal.
// An SRDY event indicates that the slave processor is ready to send or receive data.
// see GPIO.c file for hardware connections 

/*!
 * @defgroup BLE
 * @brief
 * @{*/

// NPI symbols
/**
 * Start of frame character
 */
#define SOF  254

/**
 * return parameters for fail
 */
#define APFAIL 0
/**
 * return parameters for success
 */
#define APOK   1


/**
 * Initialize serial link and GPIO to Bluetooth module.
 * See GPIO.h file for hardware connections.
 * Resets the Bluetooth module and initialize connection.
 * @param  none
 * @return APOK on success, APFAIL on timeout
 * @brief  Initialize serial link and GPIO to Bluetooth module
 */
int AP_Init(void);


/**
 * Resets the Bluetooth module.
 * With MRDY high, clear RESET low for 10 ms.
 * @param  none
 * @return none
 * @brief Hardware reset of the Bluetooth module
 */
void AP_Reset(void);

/**
 * Send a message to the Bluetooth module
 * Automatically calculates/sends FCS at end.<br>
 * 1) Send NPI package (it will calculate fcs)<br>
 * 2) Wait for entire message to be sent
 * @note FCS is the 8-bit EOR of all bytes except SOF and FCS itself.
 * @param  pt pointer to NPI encoded array (the message to send)
 * @return APOK on success, APFAIL on timeout
 * @brief sends a message to the Bluetooth module
 */
int AP_SendMessage(uint8_t *pt);


/**
 * This function is for debugging. It sends RecvBuf from SNP to UART0
 * @note FCS is the 8-bit EOR of all bytes except SOF and FCS itself.
 * @param  response 0 is OK, 1 was error
 * @return none
 * @brief RecvBuf from SNP to UART0
 */
void AP_EchoReceived(int response);

/**
 * This function is for debugging. It sends a message to UART0
 * @param  sendMsg is SNP message to display out UART
 * @return none
 * @brief Sends message to UART0
 */
void AP_EchoSendMessage(uint8_t *sendMsg);


/**
 * Receive a message from the Bluetooth module
 * 1) Receive NPI package<br>
 * 2) Wait for entire message to be received
 * @param  pt pointer to empty buffer into which data is returned
 * @param  max maximum size (discard data beyond this limit)
 * @return APOK on success, APFAIL on timeout or fcs error
 * @brief Receive a message from the Bluetooth module
 */
int AP_RecvMessage(uint8_t *pt, uint32_t max);

/**
 * Check to see if Bluetooth module wishes to send packet
 * @param  none
 * @return 0 for no communication needed and nonzero for communication ready
 * @brief Check Bluetooth status
 */
uint32_t AP_RecvStatus(void);


/**
 * send a message to the Bluetooth module
 * and receive a response from the Bluetooth module<br>
 * 1) send outgoing message<br>
 * 2) wait at least 10ms to receive NPI package<br>
 * 3) Wait for entire message to be received
 * @param  msgPt points to a string message to send
 * @param  responsePt points to empty buffer into which data is returned
 * @param  max maximum size (discard data beyond this limit)
 * @return APOK if ok, APFAIL on error (timeout or fcs error)
 * @brief Send and receive Bluetooth message
 */
int AP_SendMessageResponse(uint8_t *msgPt, uint8_t *responsePt,uint32_t max);

/**
 * Simple delay function which delays about n milliseconds.
 * @param n number of msec to wait
 * @return none
 * @brief Delay n milliseconds
 */
void AP_Delay1ms(uint32_t n);

/**
 * Return notification CCCD from the communication interface
 * this does not perform BLU communication, it reads current CCCD value of notify characteristic
 * @param i index into which notify characteristic to return
 * @return 16-bit CCCD value of the notify characteristic
 * @brief Get notification CCCD
 */
uint16_t AP_GetNotifyCCCD(uint32_t i);

/**
 * Debugging dump of last received message to virtual serial port to PC
 * @param response (number returned by last AP call)
 * @return none
 * @brief Debugging dump
 */
void AP_OutMessage(int response);

/**
 * Debugging dump of a data value to virtual serial port to PC.
 * Data shown as 1 to 8 hexadecimal characters
 * @param label string with name of parameter to dump
 * @param value numerical value of parameter to dump
 * @return none
 * @brief Debugging dump
 */
void AP_OutValue(char *label,uint32_t value);
 
/**
 * Add a service
 * @param uuid is 0xFFF0, 0xFFF1, ...
 * @return APOK if successful, APFAIL if SNP failure
 * @brief Add Service
 */
int AP_AddService(uint16_t uuid);

/**
 * Register a service
 * @param none
 * @return APOK if successful, APFAIL if SNP failure
 * @brief Register Service
 */
int AP_RegisterService(void);

/**
 * Add a read, write, or read/write characteristic
 * for notify properties, call AP_AddNotifyCharacteristic
 * @param uuid is 0xFFF0, 0xFFF1, ...
 * @param thesize is the number of bytes in the user data 1,2,4, or 8
 * @param pt is a pointer to the user data, stored little endian
 * @param permission is GATT Permission, 0=none,1=read,2=write, 3=Read+write
 * @param properties is GATT Properties, 2=read,8=write,0x0A=read+write
 * @param name is a null-terminated string, maximum length of name is 20 bytes
 * @param ReadFunc is a function pointer, called before it responses with data from internal structure
 * @param WriteFunc is a function pointer, called after it accepts data into internal structure
 * @return APOK if successful, APFAIL if name is empty, more than 8 characteristics, or if SNP failure
 * @brief Add Characteristic
 */
int AP_AddCharacteristic(uint16_t uuid, uint16_t thesize, void *pt, uint8_t permission,
  uint8_t properties, char name[], void(*ReadFunc)(void), void(*WriteFunc)(void));

/**
 * Add a notify characteristic
 * for read, write, or read/write characteristic, call AP_AddCharacteristic
 * @param uuid is 0xFFF0, 0xFFF1, ...
 * @param thesize is the number of bytes in the user data 1,2,4, or 8
 * @param pt is a pointer to the user data, stored little endian
 * @param name is a null-terminated string, maximum length of name is 20 bytes
 * @param CCCDfunc is a function pointer, called after it accepts , changing CCCDvalue
 * @return APOK if successful, APFAIL if name is empty, more than 4 notify characteristics, or if SNP failure
 * @brief Add Notify Characteristic
 */
int AP_AddNotifyCharacteristic(uint16_t uuid, uint16_t thesize,  void *pt, 
  char name[], void(*CCCDfunc)(void));
  
/**
 * Send a notification (will skip if CCCD is 0)
 * @param i index into notify characteristic to send
 * @return APOK if successful, APFAIL if notification not configured, or if SNP failure
 * @brief Send Notification
 */
int AP_SendNotification(uint32_t i);

/**
 * Start advertisement
 * @param none
 * @return APOK if successful, APFAIL if notification not configured, or if SNP failure
 * @brief start advertising
 */
int AP_StartAdvertisement(void);

/**
 * Start advertisement for Jacki
 * @param none
 * @return APOK if successful, APFAIL if notification not configured, or if SNP failure
 * @brief start advertising
 */
int AP_StartAdvertisementJacki(void);

/**
 * Get status of connection
 * @param none
 * @return status 0xAABBCCDD<br>
 * AA is GAPRole Status<br>
 * BB is Advertising Status<br>
 * CC is ATT Status<br>
 * DD is ATT method in progress
 * @brief Get status of BLE
 */
uint32_t AP_GetStatus(void);

/**
 * Get version of the SNP application running on the CC2650
 * @param none
 * @return version
 * @brief Get version number of SNP
 */
uint32_t AP_GetVersion(void);

/**
 * handle incoming SNP frames
 * @param none
 * @return none
 * @note must run periodically
 * @brief handles receive communication
 */
void AP_BackgroundProcess(void);

/**
 * returns the size of an NPI message
 * @param pt pointer to NPI message
 * @return size of the message
// extracted little Endian from byte 1 and byte 2
 * @brief Get size of message
 */
uint32_t AP_GetSize(uint8_t *pt);

/**
 * \enum GattDescription
 * These are possible predefined UUIDs for various BLE characteristics
 * @note source https://docs.mbed.com/docs/ble-api/en/master/api/classGattCharacteristic.html
 * @brief enumerated Gatt descriptions
 */
enum GattDescription   {
  UUID_BATTERY_LEVEL_STATE_CHAR = 0x2A1B, UUID_BATTERY_POWER_STATE_CHAR = 0x2A1A, UUID_REMOVABLE_CHAR = 0x2A3A, UUID_SERVICE_REQUIRED_CHAR = 0x2A3B,
  UUID_ALERT_CATEGORY_ID_CHAR = 0x2A43, UUID_ALERT_CATEGORY_ID_BIT_MASK_CHAR = 0x2A42, UUID_ALERT_LEVEL_CHAR = 0x2A06, UUID_ALERT_NOTIFICATION_CONTROL_POINT_CHAR = 0x2A44,
  UUID_ALERT_STATUS_CHAR = 0x2A3F, UUID_BATTERY_LEVEL_CHAR = 0x2A19, UUID_BLOOD_PRESSURE_FEATURE_CHAR = 0x2A49, UUID_BLOOD_PRESSURE_MEASUREMENT_CHAR = 0x2A35,
  UUID_BODY_SENSOR_LOCATION_CHAR = 0x2A38, UUID_BOOT_KEYBOARD_INPUT_REPORT_CHAR = 0x2A22, UUID_BOOT_KEYBOARD_OUTPUT_REPORT_CHAR = 0x2A32, UUID_BOOT_MOUSE_INPUT_REPORT_CHAR = 0x2A33,
  UUID_CURRENT_TIME_CHAR = 0x2A2B, UUID_DATE_TIME_CHAR = 0x2A08, UUID_DAY_DATE_TIME_CHAR = 0x2A0A, UUID_DAY_OF_WEEK_CHAR = 0x2A09,
  UUID_DST_OFFSET_CHAR = 0x2A0D, UUID_EXACT_TIME_256_CHAR = 0x2A0C, UUID_FIRMWARE_REVISION_STRING_CHAR = 0x2A26, UUID_GLUCOSE_FEATURE_CHAR = 0x2A51,
  UUID_GLUCOSE_MEASUREMENT_CHAR = 0x2A18, UUID_GLUCOSE_MEASUREMENT_CONTEXT_CHAR = 0x2A34, UUID_HARDWARE_REVISION_STRING_CHAR = 0x2A27, UUID_HEART_RATE_CONTROL_POINT_CHAR = 0x2A39,
  UUID_HEART_RATE_MEASUREMENT_CHAR = 0x2A37, UUID_HID_CONTROL_POINT_CHAR = 0x2A4C, UUID_HID_INFORMATION_CHAR = 0x2A4A, UUID_HUMIDITY_CHAR = 0x2A6F,
  UUID_IEEE_REGULATORY_CERTIFICATION_DATA_LIST_CHAR = 0x2A2A, UUID_INTERMEDIATE_CUFF_PRESSURE_CHAR = 0x2A36, UUID_INTERMEDIATE_TEMPERATURE_CHAR = 0x2A1E, UUID_LOCAL_TIME_INFORMATION_CHAR = 0x2A0F,
  UUID_MANUFACTURER_NAME_STRING_CHAR = 0x2A29, UUID_MEASUREMENT_INTERVAL_CHAR = 0x2A21, UUID_MODEL_NUMBER_STRING_CHAR = 0x2A24, UUID_UNREAD_ALERT_CHAR = 0x2A45,
  UUID_NEW_ALERT_CHAR = 0x2A46, UUID_PNP_ID_CHAR = 0x2A50, UUID_PRESSURE_CHAR = 0x2A6D, UUID_PROTOCOL_MODE_CHAR = 0x2A4E,
  UUID_RECORD_ACCESS_CONTROL_POINT_CHAR = 0x2A52, UUID_REFERENCE_TIME_INFORMATION_CHAR = 0x2A14, UUID_REPORT_CHAR = 0x2A4D, UUID_REPORT_MAP_CHAR = 0x2A4B,
  UUID_RINGER_CONTROL_POINT_CHAR = 0x2A40, UUID_RINGER_SETTING_CHAR = 0x2A41, UUID_SCAN_INTERVAL_WINDOW_CHAR = 0x2A4F, UUID_SCAN_REFRESH_CHAR = 0x2A31,
  UUID_SERIAL_NUMBER_STRING_CHAR = 0x2A25, UUID_SOFTWARE_REVISION_STRING_CHAR = 0x2A28, UUID_SUPPORTED_NEW_ALERT_CATEGORY_CHAR = 0x2A47, UUID_SUPPORTED_UNREAD_ALERT_CATEGORY_CHAR = 0x2A48,
  UUID_SYSTEM_ID_CHAR = 0x2A23, UUID_TEMPERATURE_CHAR = 0x2A6E, UUID_TEMPERATURE_MEASUREMENT_CHAR = 0x2A1C, UUID_TEMPERATURE_TYPE_CHAR = 0x2A1D,
  UUID_TIME_ACCURACY_CHAR = 0x2A12, UUID_TIME_SOURCE_CHAR = 0x2A13, UUID_TIME_UPDATE_CONTROL_POINT_CHAR = 0x2A16, UUID_TIME_UPDATE_STATE_CHAR = 0x2A17,
  UUID_TIME_WITH_DST_CHAR = 0x2A11, UUID_TIME_ZONE_CHAR = 0x2A0E, UUID_TX_POWER_LEVEL_CHAR = 0x2A07, UUID_CSC_FEATURE_CHAR = 0x2A5C,
  UUID_CSC_MEASUREMENT_CHAR = 0x2A5B, UUID_RSC_FEATURE_CHAR = 0x2A54, UUID_RSC_MEASUREMENT_CHAR = 0x2A53
};
/**
 * \enum GattFormat
 * These are possible predefined formats for various BLE characteristics
 * @brief enumerated Gatt formats
 */
enum  GattFormat  {
  BLE_GATT_FORMAT_RFU = 0x00, BLE_GATT_FORMAT_BOOLEAN = 0x01, BLE_GATT_FORMAT_2BIT = 0x02, BLE_GATT_FORMAT_NIBBLE = 0x03,
  BLE_GATT_FORMAT_UINT8 = 0x04, BLE_GATT_FORMAT_UINT12 = 0x05, BLE_GATT_FORMAT_UINT16 = 0x06, BLE_GATT_FORMAT_UINT24 = 0x07,
  BLE_GATT_FORMAT_UINT32 = 0x08, BLE_GATT_FORMAT_UINT48 = 0x09, BLE_GATT_FORMAT_UINT64 = 0x0A, BLE_GATT_FORMAT_UINT128 = 0x0B,
  BLE_GATT_FORMAT_SINT8 = 0x0C, BLE_GATT_FORMAT_SINT12 = 0x0D, BLE_GATT_FORMAT_SINT16 = 0x0E, BLE_GATT_FORMAT_SINT24 = 0x0F,
  BLE_GATT_FORMAT_SINT32 = 0x10, BLE_GATT_FORMAT_SINT48 = 0x11, BLE_GATT_FORMAT_SINT64 = 0x12, BLE_GATT_FORMAT_SINT128 = 0x13,
  BLE_GATT_FORMAT_FLOAT32 = 0x14, BLE_GATT_FORMAT_FLOAT64 = 0x15, BLE_GATT_FORMAT_SFLOAT = 0x16, BLE_GATT_FORMAT_FLOAT = 0x17,
  BLE_GATT_FORMAT_DUINT16 = 0x18, BLE_GATT_FORMAT_UTF8S = 0x19, BLE_GATT_FORMAT_UTF16S = 0x1A, BLE_GATT_FORMAT_STRUCT = 0x1B
};

/**
 * \enum GattUnits
 * These are possible predefined units for various BLE characteristics
 * @brief enumerated Gatt units
 */
enum  GattUnits  {
  BLE_GATT_UNIT_NONE = 0x2700, BLE_GATT_UNIT_LENGTH_METRE = 0x2701, BLE_GATT_UNIT_MASS_KILOGRAM = 0x2702, BLE_GATT_UNIT_TIME_SECOND = 0x2703,
  BLE_GATT_UNIT_ELECTRIC_CURRENT_AMPERE = 0x2704, BLE_GATT_UNIT_THERMODYNAMIC_TEMPERATURE_KELVIN = 0x2705, BLE_GATT_UNIT_AMOUNT_OF_SUBSTANCE_MOLE = 0x2706, BLE_GATT_UNIT_LUMINOUS_INTENSITY_CANDELA = 0x2707,
  BLE_GATT_UNIT_AREA_SQUARE_METRES = 0x2710, BLE_GATT_UNIT_VOLUME_CUBIC_METRES = 0x2711, BLE_GATT_UNIT_VELOCITY_METRES_PER_SECOND = 0x2712, BLE_GATT_UNIT_ACCELERATION_METRES_PER_SECOND_SQUARED = 0x2713,
  BLE_GATT_UNIT_WAVENUMBER_RECIPROCAL_METRE = 0x2714, BLE_GATT_UNIT_DENSITY_KILOGRAM_PER_CUBIC_METRE = 0x2715, BLE_GATT_UNIT_SURFACE_DENSITY_KILOGRAM_PER_SQUARE_METRE = 0x2716, BLE_GATT_UNIT_SPECIFIC_VOLUME_CUBIC_METRE_PER_KILOGRAM = 0x2717,
  BLE_GATT_UNIT_CURRENT_DENSITY_AMPERE_PER_SQUARE_METRE = 0x2718, BLE_GATT_UNIT_MAGNETIC_FIELD_STRENGTH_AMPERE_PER_METRE = 0x2719, BLE_GATT_UNIT_AMOUNT_CONCENTRATION_MOLE_PER_CUBIC_METRE = 0x271A, BLE_GATT_UNIT_MASS_CONCENTRATION_KILOGRAM_PER_CUBIC_METRE = 0x271B,
  BLE_GATT_UNIT_LUMINANCE_CANDELA_PER_SQUARE_METRE = 0x271C, BLE_GATT_UNIT_REFRACTIVE_INDEX = 0x271D, BLE_GATT_UNIT_RELATIVE_PERMEABILITY = 0x271E, BLE_GATT_UNIT_PLANE_ANGLE_RADIAN = 0x2720,
  BLE_GATT_UNIT_SOLID_ANGLE_STERADIAN = 0x2721, BLE_GATT_UNIT_FREQUENCY_HERTZ = 0x2722, BLE_GATT_UNIT_FORCE_NEWTON = 0x2723, BLE_GATT_UNIT_PRESSURE_PASCAL = 0x2724,
  BLE_GATT_UNIT_ENERGY_JOULE = 0x2725, BLE_GATT_UNIT_POWER_WATT = 0x2726, BLE_GATT_UNIT_ELECTRIC_CHARGE_COULOMB = 0x2727, BLE_GATT_UNIT_ELECTRIC_POTENTIAL_DIFFERENCE_VOLT = 0x2728,
  BLE_GATT_UNIT_CAPACITANCE_FARAD = 0x2729, BLE_GATT_UNIT_ELECTRIC_RESISTANCE_OHM = 0x272A, BLE_GATT_UNIT_ELECTRIC_CONDUCTANCE_SIEMENS = 0x272B, BLE_GATT_UNIT_MAGNETIC_FLEX_WEBER = 0x272C,
  BLE_GATT_UNIT_MAGNETIC_FLEX_DENSITY_TESLA = 0x272D, BLE_GATT_UNIT_INDUCTANCE_HENRY = 0x272E, BLE_GATT_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_CELSIUS = 0x272F, BLE_GATT_UNIT_LUMINOUS_FLUX_LUMEN = 0x2730,
  BLE_GATT_UNIT_ILLUMINANCE_LUX = 0x2731, BLE_GATT_UNIT_ACTIVITY_REFERRED_TO_A_RADIONUCLIDE_BECQUEREL = 0x2732, BLE_GATT_UNIT_ABSORBED_DOSE_GRAY = 0x2733, BLE_GATT_UNIT_DOSE_EQUIVALENT_SIEVERT = 0x2734,
  BLE_GATT_UNIT_CATALYTIC_ACTIVITY_KATAL = 0x2735, BLE_GATT_UNIT_DYNAMIC_VISCOSITY_PASCAL_SECOND = 0x2740, BLE_GATT_UNIT_MOMENT_OF_FORCE_NEWTON_METRE = 0x2741, BLE_GATT_UNIT_SURFACE_TENSION_NEWTON_PER_METRE = 0x2742,
  BLE_GATT_UNIT_ANGULAR_VELOCITY_RADIAN_PER_SECOND = 0x2743, BLE_GATT_UNIT_ANGULAR_ACCELERATION_RADIAN_PER_SECOND_SQUARED = 0x2744, BLE_GATT_UNIT_HEAT_FLUX_DENSITY_WATT_PER_SQUARE_METRE = 0x2745, BLE_GATT_UNIT_HEAT_CAPACITY_JOULE_PER_KELVIN = 0x2746,
  BLE_GATT_UNIT_SPECIFIC_HEAT_CAPACITY_JOULE_PER_KILOGRAM_KELVIN = 0x2747, BLE_GATT_UNIT_SPECIFIC_ENERGY_JOULE_PER_KILOGRAM = 0x2748, BLE_GATT_UNIT_THERMAL_CONDUCTIVITY_WATT_PER_METRE_KELVIN = 0x2749, BLE_GATT_UNIT_ENERGY_DENSITY_JOULE_PER_CUBIC_METRE = 0x274A,
  BLE_GATT_UNIT_ELECTRIC_FIELD_STRENGTH_VOLT_PER_METRE = 0x274B, BLE_GATT_UNIT_ELECTRIC_CHARGE_DENSITY_COULOMB_PER_CUBIC_METRE = 0x274C, BLE_GATT_UNIT_SURFACE_CHARGE_DENSITY_COULOMB_PER_SQUARE_METRE = 0x274D, BLE_GATT_UNIT_ELECTRIC_FLUX_DENSITY_COULOMB_PER_SQUARE_METRE = 0x274E,
  BLE_GATT_UNIT_PERMITTIVITY_FARAD_PER_METRE = 0x274F, BLE_GATT_UNIT_PERMEABILITY_HENRY_PER_METRE = 0x2750, BLE_GATT_UNIT_MOLAR_ENERGY_JOULE_PER_MOLE = 0x2751, BLE_GATT_UNIT_MOLAR_ENTROPY_JOULE_PER_MOLE_KELVIN = 0x2752,
  BLE_GATT_UNIT_EXPOSURE_COULOMB_PER_KILOGRAM = 0x2753, BLE_GATT_UNIT_ABSORBED_DOSE_RATE_GRAY_PER_SECOND = 0x2754, BLE_GATT_UNIT_RADIANT_INTENSITY_WATT_PER_STERADIAN = 0x2755, BLE_GATT_UNIT_RADIANCE_WATT_PER_SQUARE_METRE_STERADIAN = 0x2756,
  BLE_GATT_UNIT_CATALYTIC_ACTIVITY_CONCENTRATION_KATAL_PER_CUBIC_METRE = 0x2757, BLE_GATT_UNIT_TIME_MINUTE = 0x2760, BLE_GATT_UNIT_TIME_HOUR = 0x2761, BLE_GATT_UNIT_TIME_DAY = 0x2762,
  BLE_GATT_UNIT_PLANE_ANGLE_DEGREE = 0x2763, BLE_GATT_UNIT_PLANE_ANGLE_MINUTE = 0x2764, BLE_GATT_UNIT_PLANE_ANGLE_SECOND = 0x2765, BLE_GATT_UNIT_AREA_HECTARE = 0x2766,
  BLE_GATT_UNIT_VOLUME_LITRE = 0x2767, BLE_GATT_UNIT_MASS_TONNE = 0x2768, BLE_GATT_UNIT_PRESSURE_BAR = 0x2780, BLE_GATT_UNIT_PRESSURE_MILLIMETRE_OF_MERCURY = 0x2781,
  BLE_GATT_UNIT_LENGTH_ANGSTROM = 0x2782, BLE_GATT_UNIT_LENGTH_NAUTICAL_MILE = 0x2783, BLE_GATT_UNIT_AREA_BARN = 0x2784, BLE_GATT_UNIT_VELOCITY_KNOT = 0x2785,
  BLE_GATT_UNIT_LOGARITHMIC_RADIO_QUANTITY_NEPER = 0x2786, BLE_GATT_UNIT_LOGARITHMIC_RADIO_QUANTITY_BEL = 0x2787, BLE_GATT_UNIT_LENGTH_YARD = 0x27A0, BLE_GATT_UNIT_LENGTH_PARSEC = 0x27A1,
  BLE_GATT_UNIT_LENGTH_INCH = 0x27A2, BLE_GATT_UNIT_LENGTH_FOOT = 0x27A3, BLE_GATT_UNIT_LENGTH_MILE = 0x27A4, BLE_GATT_UNIT_PRESSURE_POUND_FORCE_PER_SQUARE_INCH = 0x27A5,
  BLE_GATT_UNIT_VELOCITY_KILOMETRE_PER_HOUR = 0x27A6, BLE_GATT_UNIT_VELOCITY_MILE_PER_HOUR = 0x27A7, BLE_GATT_UNIT_ANGULAR_VELOCITY_REVOLUTION_PER_MINUTE = 0x27A8, BLE_GATT_UNIT_ENERGY_GRAM_CALORIE = 0x27A9,
  BLE_GATT_UNIT_ENERGY_KILOGRAM_CALORIE = 0x27AA, BLE_GATT_UNIT_ENERGY_KILOWATT_HOUR = 0x27AB, BLE_GATT_UNIT_THERMODYNAMIC_TEMPERATURE_DEGREE_FAHRENHEIT = 0x27AC, BLE_GATT_UNIT_PERCENTAGE = 0x27AD,
  BLE_GATT_UNIT_PER_MILLE = 0x27AE, BLE_GATT_UNIT_PERIOD_BEATS_PER_MINUTE = 0x27AF, BLE_GATT_UNIT_ELECTRIC_CHARGE_AMPERE_HOURS = 0x27B0, BLE_GATT_UNIT_MASS_DENSITY_MILLIGRAM_PER_DECILITRE = 0x27B1,
  BLE_GATT_UNIT_MASS_DENSITY_MILLIMOLE_PER_LITRE = 0x27B2, BLE_GATT_UNIT_TIME_YEAR = 0x27B3, BLE_GATT_UNIT_TIME_MONTH = 0x27B4, BLE_GATT_UNIT_CONCENTRATION_COUNT_PER_CUBIC_METRE = 0x27B5,
  BLE_GATT_UNIT_IRRADIANCE_WATT_PER_SQUARE_METRE = 0x27B6
};
#define  BLE_GATT_CPF_FORMAT_RFU      0x00  ///< BLE format reserved for future use
#define  BLE_GATT_CPF_FORMAT_BOOLEAN  0x01  ///< BLE format boolean (1=true,0=false)
#define  BLE_GATT_CPF_FORMAT_2BIT     0x02  ///< BLE format 2 bits
#define  BLE_GATT_CPF_FORMAT_NIBBLE   0x03  ///< BLE format 4 bits
#define  BLE_GATT_CPF_FORMAT_UINT8    0x04  ///< BLE format unsigned 8 bits
#define  BLE_GATT_CPF_FORMAT_UINT12   0x05  ///< BLE format unsigned 12 bits
#define  BLE_GATT_CPF_FORMAT_UINT16   0x06  ///< BLE format unsigned 16 bits
#define  BLE_GATT_CPF_FORMAT_UINT24   0x07  ///< BLE format unsigned 24 bits
#define  BLE_GATT_CPF_FORMAT_UINT32   0x08  ///< BLE format unsigned 32 bits
#define  BLE_GATT_CPF_FORMAT_UINT48   0x09  ///< BLE format unsigned 48 bits
#define  BLE_GATT_CPF_FORMAT_UINT64   0x0A  ///< BLE format unsigned 64 bits
#define  BLE_GATT_CPF_FORMAT_UINT128  0x0B  ///< BLE format unsigned 128 bits
#define  BLE_GATT_CPF_FORMAT_SINT8    0x0C  ///< BLE format signed 8 bits
#define  BLE_GATT_CPF_FORMAT_SINT12   0x0D  ///< BLE format signed 12 bits
#define  BLE_GATT_CPF_FORMAT_SINT16   0x0E  ///< BLE format signed 16 bits
#define  BLE_GATT_CPF_FORMAT_SINT24   0x0F  ///< BLE format signed 24 bits
#define  BLE_GATT_CPF_FORMAT_SINT32   0x10  ///< BLE format signed 32 bits
#define  BLE_GATT_CPF_FORMAT_SINT48   0x11  ///< BLE format signed 48 bits
#define  BLE_GATT_CPF_FORMAT_SINT64   0x12  ///< BLE format signed 64 bits
#define  BLE_GATT_CPF_FORMAT_SINT128  0x13  ///< BLE format signed 128 bits
#define  BLE_GATT_CPF_FORMAT_FLOAT32  0x14  ///< BLE format IEEE-754 32-bit floating point
#define  BLE_GATT_CPF_FORMAT_FLOAT64  0x15  ///< BLE format IEEE-754 64-bit floating point
#define  BLE_GATT_CPF_FORMAT_SFLOAT   0x16  ///< BLE format IEEE-11073 16-bit SFLOAT
#define  BLE_GATT_CPF_FORMAT_FLOAT    0x17  ///< BLE format IEEE-11073 32-bit FLOAT
#define  BLE_GATT_CPF_FORMAT_DUINT16  0x18  ///< BLE format IEEE-20601 format
#define  BLE_GATT_CPF_FORMAT_UTF8S    0x19  ///< BLE format UTF-8 string
#define  BLE_GATT_CPF_FORMAT_UTF16S   0x1A  ///< BLE format UTF-16 string
#define  BLE_GATT_CPF_FORMAT_STRUCT   0x1B  ///< BLE format Opaque structure
