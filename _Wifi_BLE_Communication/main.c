#include <stdlib.h>
#include <string.h>
#include "driverlib.h"
#include "simplelink.h"
#include "sl_common.h"
#include "MQTTClient.h"
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

static void Init_Dist();
static void sendUpdates();

struct{
    _u8 Recvbuff[MAX_SEND_RCV_SIZE];
    _u8 SendBuff[MAX_SEND_BUF_SIZE];

    _u8 HostName[SMALL_BUF];
    _u8 CityName[SMALL_BUF];

    _u32 DestinationIP;
    _i16 SockID;
}g_AppData;


int main(int argc, char** argv)

{
    Motor_Init();
    Bump_Init();
    Tachometer_Init();
    EnableInterrupts();
    CLI_Configure();



    _i32 retVal = -1;
    retVal = initializeAppVariables();
    ASSERT_ON_ERROR(retVal);
    stopWDT();
    initClk();



    Init_Dist();

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


    while(1){
        sendUpdates();
        Delay(10);
        uint32_t k_i = Ki;
        uint32_t k_p = Kp;
        uint32_t k_d = Kd;
    }
}

static void Init_Dist() {
    SysTick->LOAD = 0x00FFFFFF;           // maximum reload value
    SysTick->CTRL = 0x00000005;           // enable SysTick with no interrupts
    I2CB1_Init(30); // baud rate = 12MHz/30=400kHz
    OPT3101_Init();
    OPT3101_Setup();
    OPT3101_CalibrateInternalCrosstalk();
    OPT3101_StartMeasurementChannel(channel);
    StartTime = SysTick->VAL;
}

static void sendUpdates() {
    rc = MQTTYield(&hMQTTClient, 10);
    if (rc != 0) {
        CLI_Write(" MQTT failed to yield \n\r");
        LOOP_FOREVER();
    }

    /*
     * Tachometry
     */
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
    else bump_str = ((char*)"Bump!!!");

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


    sendMessage(leftDist,"MayaNet_LeftDist");
    sendMessage(centerDist,"MayaNet_CenterDist");
    sendMessage(rightDist,"MayaNet_RightDist");
    sendMessage(bump_str,"MayaNet_Bump");
    sendMessage(leftRPM,"MayaNet_LeftRPM");
    sendMessage(rightRPM,"MayaNet_RightRPM");
}
