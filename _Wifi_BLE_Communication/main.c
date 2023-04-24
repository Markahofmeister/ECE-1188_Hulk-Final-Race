#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "FFT.h"
#include "LPF.h"
#include "driverlib.h"
#include "simplelink.h"
#include "sl_common.h"
#include "odometry.h"
#include "MQTTClient.h"
#include "PID_Motor.h"
#include "../inc/Bump.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/CortexM.h"
#include "../inc/SysTickInts.h"
#include "../inc/Reflectance.h"
#include "../inc/BumpInt.h"
#include "../inc/Motor.h"
#include "../inc/TimerA1.h"
#include "../inc/I2CB1.h"
#include "../inc/UART0.h"
#include "../inc/opt3101.h"
#include "../inc/Tachometer.h"
#include "../inc/TA3InputCapture.h"
#include "/Users/mhegde/Downloads/Pitt/Spring 2023/1188/ECE-1188_Hulk-Final-Race/_Wifi_BLE_Communication/Wifi.h"

#define BUFF_SIZE 32
#define SSID_NAME       "1PittBitt"       /* Access point name to connect to. */
#define SEC_TYPE        SL_SEC_TYPE_WPA_WPA2     /* Security type of the Access point */
#define PASSKEY         "b3n3dum!"   /* Password in case of secure AP */
#define PASSKEY_LEN     pal_Strlen(PASSKEY)  /* Password length in case of secure AP */
#define SL_STOP_TIMEOUT        0xFF
#define SMALL_BUF           32
#define MAX_SEND_BUF_SIZE   512
#define MAX_SEND_RCV_SIZE   1024
#define min(X,Y) ((X) < (Y) ? (X) : (Y))

Client hMQTTClient;     // MQTT Client
uint32_t channel = 1;
int nVal = 0;
int rc = 0;
char* bump_str;
char input = 'a';
extern int32_t MyX,MyY;               // position in 0.0001cm
extern int32_t MyTheta;               // direction units 2*pi/16384 radians (-pi to +pi)

static void Init_Dist();
static void sendUpdates();
static void poll_start();
static void check_stop();

struct{
    _u8 Recvbuff[MAX_SEND_RCV_SIZE];
    _u8 SendBuff[MAX_SEND_BUF_SIZE];

    _u8 HostName[SMALL_BUF];
    _u8 CityName[SMALL_BUF];

    _u32 DestinationIP;
    _i16 SockID;
}g_AppData;

int32_t Right(int32_t right){
  return  (right*(59*right + 7305) + 2348974)/32768;
}
// left is raw sensor data from left sensor
// return calibrated distance from center of Robot to left wall
int32_t Left(int32_t left){
  return (1247*left)/2048 + 22;
}

// assumes track is 500mm
int32_t Mode=1; // 0 stop, 1 run
float Error;
uint32_t UR, UL;  // PWM duty 0 to 14,998
#define TOOCLOSE 400        //was 200
#define DESIRED 450         //was 250
int32_t SetPoint = 400; // mm       //was 250
int32_t LeftDistance,CenterDistance,RightDistance; // mm
#define TOOFAR 400 // was 400. Don't think they actually use this.

#define PWMNOMINAL 6500 // was 2500
#define SWING 2000 //was 1000
#define PWMMIN (PWMNOMINAL-SWING)
#define PWMMAX (PWMNOMINAL+SWING)


void Controller(void){ // runs at 100 Hz
  if(Mode){
    if((LeftDistance>DESIRED)&&(RightDistance>DESIRED)){
      SetPoint = (LeftDistance+RightDistance)/2;
    }else{
      SetPoint = DESIRED;
    }
    if(LeftDistance < RightDistance ){
      Error = LeftDistance-SetPoint;
    }else {
      Error = SetPoint-RightDistance;
    }
 //   UR = UR + Ki*Error;      // adjust right motor
    UR = PWMNOMINAL+Kp*Error; // proportional control
    UL = PWMNOMINAL-Kp*Error; // proportional control
    if(UR < (PWMNOMINAL-SWING)) UR = PWMNOMINAL-SWING; // 3,000 to 7,000
    if(UR > (PWMNOMINAL+SWING)) UR = PWMNOMINAL+SWING;
    if(UL < (PWMNOMINAL-SWING)) UL = PWMNOMINAL-SWING; // 3,000 to 7,000
    if(UL > (PWMNOMINAL+SWING)) UL = PWMNOMINAL+SWING;
    Motor_Forward(UL,UR);

  }
}

void Controller_Right(void){ // runs at 100 Hz
  if(Mode){
    if((RightDistance>DESIRED)){
      SetPoint = (RightDistance)/2;
    }else{
      SetPoint = DESIRED;
    }

    Error = SetPoint-RightDistance;
    UR = PWMNOMINAL+Kp*Error; // proportional control
    UR = UR + Ki*Error;      // adjust right motor
    UL = PWMNOMINAL-Kp*Error; // proportional control
    if(UR < (PWMNOMINAL-SWING)) UR = PWMNOMINAL-SWING; // 3,000 to 7,000
    if(UR > (PWMNOMINAL+SWING)) UR = PWMNOMINAL+SWING;
    if(UL < (PWMNOMINAL-SWING)) UL = PWMNOMINAL-SWING; // 3,000 to 7,000
    if(UL > (PWMNOMINAL+SWING)) UL = PWMNOMINAL+SWING;

    //turns left if the center measurement and right measurement is small enough that we will hit the wall if we don't turn
    if((RightDistance<250) && (CenterDistance <250)){
        UL = 0;
        UR = PWMNOMINAL;
    }

    Motor_Forward(UL,UR);

  }
}
void Controller_Left(void){ // runs at 100 Hz
  if(Mode){
    if((LeftDistance>DESIRED)){
      SetPoint = (LeftDistance)/2;
    }else{
      SetPoint = DESIRED;
    }

    Error = SetPoint-LeftDistance;
    UL = PWMNOMINAL+Kp*Error; // proportional control
    UL = UL + Ki*Error;      // adjust right motor
    UR = PWMNOMINAL-Kp*Error; // proportional control

    if(UR < (PWMNOMINAL-SWING)) UR = PWMNOMINAL-SWING; // 3,000 to 7,000
    if(UR > (PWMNOMINAL+SWING)) UR = PWMNOMINAL+SWING;
    if(UL < (PWMNOMINAL-SWING)) UL = PWMNOMINAL-SWING; // 3,000 to 7,000
    if(UL > (PWMNOMINAL+SWING)) UL = PWMNOMINAL+SWING;

    //turns left if the center measurement and left measurement is small enough that we will hit the wall if we don't turn
    if((LeftDistance<250) && (CenterDistance <250)){
        UR = 0;
        UL = PWMNOMINAL;
    }

    Motor_Forward(UL,UR);

  }
}

int main(int argc, char** argv)

{
    int i_cont = 0;

    Motor_Init();
//    PID_Motor_Init(12, 6, 3);
//    PID_Motor_Target(250, 250);
    Bump_Init();
    Tachometer_Init();
    CLI_Configure();
    DisableInterrupts();
    Clock_Init48MHz();
    LaunchPad_Init();
    Motor_Stop();
//    PID_Motor_Stop();
    Odometry_Init(0,0,NORTH); // facing North

    Mode = 1;
    Init_Dist();

    UR = UL = PWMNOMINAL;     //initial power

    EnableInterrupts();

    /* Wifi Setup Stuff Start */
    _i32 retVal = -1;
    retVal = initializeAppVariables();
    ASSERT_ON_ERROR(retVal);
    stopWDT();
    initClk();
    retVal = configureSimpleLinkToDefaultState();
    if(retVal < 0)
    {
        if (DEVICE_NOT_IN_STATION_MODE == retVal) CLI_Write(" Failed to configure the device in its default state \n\r");
        LOOP_FOREVER();
    }
    CLI_Write(" Device is configured in default state \n\r");
    retVal = sl_Start(0, 0, 0);
    if ((retVal < 0) || (ROLE_STA != retVal) )
    {
        CLI_Write(" Failed to start the device \n\r");
        LOOP_FOREVER();
    }
    CLI_Write(" Device started as STATION \n\r");
    retVal = establishConnectionWithAP();
    if(retVal < 0)
    {
        CLI_Write(" Failed to establish connection w/ an AP \n\r");
        LOOP_FOREVER();
    }
    CLI_Write(" Connection established w/ AP and IP is acquired \n\r");
    sl_NetCfgGet(SL_MAC_ADDRESS_GET,NULL,&macAddressLen,(unsigned char *)macAddressVal);
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", macAddressVal[0], macAddressVal[1], macAddressVal[2], macAddressVal[3], macAddressVal[4], macAddressVal[5]);
    generateUniqueID();
    unsigned char buf[100];
    unsigned char readbuf[100];
    NewNetwork(&n);
    rc = ConnectNetwork(&n, MQTT_BROKER_SERVER, 1883);
    if (rc != 0) {
        CLI_Write(" Failed to connect to MQTT broker \n\r");
        LOOP_FOREVER();
    }
    CLI_Write(" Connected to MQTT broker \n\r");
    MQTTClient(&hMQTTClient, &n, 1000, buf, 100, readbuf, 100);
    MQTTPacket_connectData cdata = MQTTPacket_connectData_initializer;
    cdata.MQTTVersion = 3;
    cdata.clientID.cstring = uniqueID;
    rc = MQTTConnect(&hMQTTClient, &cdata);
    if (rc != 0) {
        CLI_Write(" Failed to start MQTT client \n\r");
        LOOP_FOREVER();
    }
    CLI_Write(" Started MQTT client successfully \n\r");
    rc = MQTTSubscribe(&hMQTTClient, SUBSCRIBE_TOPIC, QOS0, messageArrived);
    if (rc != 0) {
        CLI_Write(" Failed to subscribe to start/stop topic \n\r");
        LOOP_FOREVER();
    }
    CLI_Write(" Subscribed to start/stop topic \n\r");
    rc = MQTTSubscribe(&hMQTTClient, SUBSCRIBE_TOPIC_Ki, QOS0, messageArrivedKi);
    if (rc != 0) {
        CLI_Write(" Failed to subscribe to Ki topic \n\r");
        LOOP_FOREVER();
    }
    CLI_Write(" Subscribed to Ki topic \n\r");
    rc = MQTTSubscribe(&hMQTTClient, SUBSCRIBE_TOPIC_Kp, QOS0, messageArrivedKp);
    if (rc != 0) {
        CLI_Write(" Failed to subscribe to Kp topic \n\r");
        LOOP_FOREVER();
    }
    CLI_Write(" Subscribed to Kp topic \n\r");
    rc = MQTTSubscribe(&hMQTTClient, SUBSCRIBE_TOPIC_Kd, QOS0, messageArrivedKd);
    if (rc != 0) {
        CLI_Write(" Failed to subscribe to Kd topic \n\r");
        LOOP_FOREVER();
    }
    CLI_Write(" Subscribed to Kd topic \n\r");
    rc = MQTTSubscribe(&hMQTTClient, uniqueID, QOS0, messageArrived);
    if (rc != 0) {
        CLI_Write(" Failed to subscribe to uniqueID topic \n\r");
        LOOP_FOREVER();
    }
    CLI_Write(" Subscribed to uniqueID topic \n\r");
    /* Wifi Setup Stuff End */

    poll_start();

    while(1){
        check_stop();

        if(TxChannel <= 2){ // 0,1,2 means new data
          if(TxChannel==0){
            if(Amplitudes[0] > 1000){
              LeftDistance = FilteredDistances[0] = Left(LPF_Calc(Distances[0]));
            }else{
              LeftDistance = FilteredDistances[0] = 500;
            }
          }else if(TxChannel==1){
            if(Amplitudes[1] > 1000){
              CenterDistance = FilteredDistances[1] = LPF_Calc2(Distances[1]);
            }else{
              CenterDistance = FilteredDistances[1] = 500;
            }
          }else {
            if(Amplitudes[2] > 1000){
              RightDistance = FilteredDistances[2] = Right(LPF_Calc3(Distances[2]));
            }else{
              RightDistance = FilteredDistances[2] = 500;
            }
          }
          TxChannel = 3; // 3 means no data
          channel = (channel+1)%3;
          OPT3101_StartMeasurementChannel(channel);
          i_cont = i_cont + 1;
        }
        Controller_Right();
        //Controller();
        if(i_cont >= 100){
            i_cont = 0;
        }

        WaitForInterrupt();

        UpdatePosition();
        sendUpdates();
        Delay(10);
    }
}

static void Init_Dist() {
//    SysTick->LOAD = 0x00FFFFFF;           // maximum reload value
//    SysTick->CTRL = 0x00000005;           // enable SysTick with no interrupts
    I2CB1_Init(30); // baud rate = 12MHz/30=400kHz
    OPT3101_Init();
    OPT3101_Setup();
    OPT3101_CalibrateInternalCrosstalk();
    TxChannel = 3;
    OPT3101_StartMeasurementChannel(channel);
//    StartTime = SysTick->VAL;
    LPF_Init(100,8);          // Setup FIR Filter
    LPF_Init2(100,8);
    LPF_Init3(100,8);
}

static void sendUpdates() {
    rc = MQTTYield(&hMQTTClient, 10);
    if (rc != 0) {
        CLI_Write(" MQTT failed to yield \n\r");
        LOOP_FOREVER();
    }

    Tachometer_Get(&LeftTach[i], &LeftDir, &LeftSteps, &RightTach[i], &RightDir, &RightSteps);
    i = i + 1;
    if(i >= TACHBUFF)
    {
        i = 0;
        ActualL = 2000000/avg(LeftTach, TACHBUFF);
        ActualR = 2000000/avg(RightTach, TACHBUFF);
     }
     Average_RPM_L[nVal] = ActualL;
     Average_RPM_R[nVal] = ActualR;
     nVal++;
     nVal = nVal % TACHBUFF;
     sprintf(leftRPM, "%d", ActualL);
     sprintf(rightRPM, "%d", ActualR);

    uint8_t bumps = Bump_Read();
    if (bumps == 0xED ) bump_str = ((char*)"No Bump");
    else {
        bump_str = ((char*)"Bump!!!");
        sendMessage(bump_str,"MayaNet_Bump");
        Motor_Stop();
        Delay(100);
    }

    if(pollDistanceSensor())
    {
      TimeToConvert = ((StartTime-SysTick->VAL)&0x00FFFFFF)/48000; // msec
      if(TxChannel <= 2)
      {
          sprintf(leftDist, "%d",Distances[0]);
          sprintf(centerDist, "%d", Distances[1]);
          sprintf(rightDist,"%d", Distances[2]);
      }
      channel = (channel+1)%3;
      OPT3101_StartMeasurementChannel(channel);
      StartTime = SysTick->VAL;
    }
    sprintf(cur_kp,"%d", Kp);
    sprintf(cur_ki,"%d", Ki);
    sprintf(cur_kd,"%d", Kd);
//    sprintf(set_point,"%d", SetPoint);

    Odometry_Get(&MyX,&MyY,&MyTheta);
    sprintf(x_dist,"%d", MyX);
    sprintf(y_dist,"%d", MyY);
    sprintf(theta_val,"%d", MyTheta);

    sendMessage(leftDist,"MayaNet_LeftDist");
    sendMessage(centerDist,"MayaNet_CenterDist");
    sendMessage(rightDist,"MayaNet_RightDist");
    sendMessage(bump_str,"MayaNet_Bump");
    sendMessage(leftRPM,"MayaNet_LeftRPM");
    sendMessage(rightRPM,"MayaNet_RightRPM");
    sendMessage(cur_kp,"MayaNet_PubKp");
    sendMessage(cur_ki,"MayaNet_PubKi");
    sendMessage(cur_kd,"MayaNet_PubKd");
    sendMessage(x_dist,"MayaNet_PubX");
    sendMessage(y_dist,"MayaNet_PubY");
    sendMessage(theta_val,"MayaNet_PubTheta");
//    sendMessage(set_point,"MayaNet_PubSP");
}

static void poll_start() {
    while(input != 'f') {
        input = UART0_InChar();
    }
    return;
}
static void check_stop() {
    input = UART0_InChar();
    if (input == 's') {
        while(1){}
    }
    return;
}
