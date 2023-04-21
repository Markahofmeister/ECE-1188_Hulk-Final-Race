// Reflectance.c
// Provide functions to take measurements using the kit's built-in
// QTRX reflectance sensor array.  Pololu part number 3672. This works by outputting to the
// sensor, waiting, then reading the digital value of each of the
// eight phototransistors.  The more reflective the target surface is,
// the faster the voltage decays.
// Mark Hofmeister
// 2/14/2022

// reflectance even LED illuminate connected to P5.3
// reflectance odd LED illuminate connected to P9.2

// reflectance sensor 1 connected to P7.0 (robot's right, robot off road to left)
// reflectance sensor 2 connected to P7.1
// reflectance sensor 3 connected to P7.2
// reflectance sensor 4 connected to P7.3 center
// reflectance sensor 5 connected to P7.4 center
// reflectance sensor 6 connected to P7.5
// reflectance sensor 7 connected to P7.6
// reflectance sensor 8 connected to P7.7 (robot's left, robot off road to right)

#include <stdint.h>
#include "msp432.h"
#include "..\inc\Clock.h"

// ------------Reflectance_Init------------
// Initialize the GPIO pins associated with the QTR-8RC
// reflectance sensor.  Infrared illumination LEDs are
// initially off.
// Input: none
// Output: none

void Reflectance_Init(void){

    //Initialize CTRL EVEN to output on P5.3
    // 0000 1000 = 0x08
    uint8_t ceMask = 0x08;
    P5->SEL0 &= ~ceMask;                  // set P5.3 to GPIO
    P5->SEL1 &= ~ceMask;
    P5->DIR  |=  ceMask;                  // set P5.3 to output
    P5->OUT  &= ~ceMask;                  // P5.3 should be off at first
    P5->REN  &= ~ceMask;                  // turn off PUR?

    //Initialize CTRL ODD to output on P9.2
    // 0000 0100 = 0x04
    uint8_t coMask = 0x04;
    P9->SEL0 &= ~coMask;                  // set P9.2 to GPIO
    P9->SEL1 &= ~coMask;
    P9->DIR  |=  coMask;                  // set P9.2 to output
    P9->OUT  &= ~coMask;                  // P9.2 should be off at first

    //Initialize bits 0-7 of QTRX module as inputs (initially) on P7.0-7.7
    // 1111 1111 = 0xFF
    uint8_t qtrxMask = 0xFF;
    P7->SEL0 = ~qtrxMask;                  // set P7.0-7.7 as GPIO
    P7->SEL1 = ~qtrxMask;
    P7->DIR  = ~qtrxMask;                 // set P7.0-7.7 as input
    //P7->REN  = qtrxMask;                //NO PULL UP/DOWN RESISTORS
    //P7->OUT  = ~qtrxMask;

}

// ------------Reflectance_Read------------
// Input: time to wait in usec
// Output: sensor readings
// Assumes: Reflectance_Init() has been called

uint8_t Reflectance_Read(uint32_t time){

    uint8_t result;                 //result to return

    uint8_t ceMask = 0x08;          //mask for even LED control
    uint8_t coMask = 0x04;          //mask for odd LED control
    uint8_t qtrxMask = 0xFF;        //mask for control of QTRX LEDs

    P5->OUT |= ceMask;              // turn on 4 even IR LEDs
    P9->OUT |= coMask;              // turn on 4 odd IR LEDs
    P7->DIR = qtrxMask;             // make P7.7-P7.0 out
    P7->OUT = qtrxMask;             // prime capacitor for measurement

    Clock_Delay1us(10);             // wait 10 us for cap to charge
    P7->DIR = ~qtrxMask;            // make P7.7-P7.0 in

    Clock_Delay1us(time);           // delay time in us
    result = P7->IN;                // read in result from IR LEDs

    P7->OUT = ~qtrxMask;            // turn off all 7 IR LEDs

    return result;

}

// Perform sensor integration
// Input: data is 8-bit result from line sensor
// Output: position in 0.1mm relative to center of line

int32_t Reflectance_Position(uint8_t data){

    uint16_t wi[] = {-33400,-23800,-14300,-4800,4800,14300,23800,33400};        //array of distances in microns

    uint16_t num = 0, den = 0;
    uint8_t i, dataMask, numBit;

    for(i = 0; i <= 7; i++) {                //create numerator and denominator values

        dataMask = (0x01 << i);             //isolate bit i to do nefarious things with.
        numBit = data & dataMask;
        numBit = numBit >> i;               //shift isolated bit down to zeroth bit place for multiplication

        num += numBit * wi[i];              //add term for sensor i to numerator

        den += numBit;

    }

    return num/den;

}

// ------------Reflectance_Center------------
// Read the two center sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 10 us
// Make the sensor pins input
// wait t us
// Read sensors
// Turn off the 8 IR LEDs
// Input: time to wait in usec
// Output: 0 (off road), 1 off to left, 2 off to right, 3 on road
// (Left,Right) Sensors
// 1,1          both sensors   on line
// 0,1          just right     off to left
// 1,0          left left      off to right
// 0,0          neither        lost
// Assumes: Reflectance_Init() has been called

uint8_t Reflectance_Center(uint32_t time){



  return 0; // replace this line

}


// ------------Reflectance_Start------------
// Begin the process of reading the eight sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 10 us
// Make the sensor pins input
// Input: none
// Output: none
// Assumes: Reflectance_Init() has been called
void Reflectance_Start(void){
    // write this as part of Lab 10
}


// ------------Reflectance_End------------
// Finish reading the eight sensors
// Read sensors
// Turn off the 8 IR LEDs
// Input: none
// Output: sensor readings
// Assumes: Reflectance_Init() has been called
// Assumes: Reflectance_Start() was called 1 ms ago
uint8_t Reflectance_End(void){
    // write this as part of Lab 10
 return 0; // replace this line
}

