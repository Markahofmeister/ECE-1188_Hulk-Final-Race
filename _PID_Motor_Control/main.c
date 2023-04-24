#include <stdint.h>
#include <stdio.h>
#include "msp.h"
#include "../inc/Clock.h"
#include "../inc/CortexM.h"
#include "../inc/PWM.h"
#include "../inc/UART0.h"
//#include "../inc/Motor.h"
#include "../inc/Bump.h"
#include "../inc/TimerA1.h"
#include "../inc/Tachometer.h"
#include "PID_Motor.h"

void main(void){
    Clock_Init48MHz();// set system clock to 48 MHz

    UART0_Initprintf(); // BAUD Rate = 115,200 bps
    printf("\n\rLab 17 speed controller\n\r");

    Bump_Init();
    Tachometer_Init();
    EnableInterrupts();
    PID_Motor_Init(12, 6, 3);
    PID_Motor_Target(250, 250);

    volatile uint16_t LRPM;
    volatile uint16_t RRPM;

    while(Bump_Read() == 0){
        PID_Motor_Forward(&LRPM, &RRPM);
        Clock_Delay1ms(10); // delay ~0.1 sec at 48 MHz
    }
}
