

extern uint16_t ActualL;                        // actual rotations per minute measured by tachometer
extern uint16_t ActualR;                        // actual rotations per minute measured by tachometer

extern uint16_t LeftDuty;                // duty cycle of left wheel (0 to 14,998)
extern uint16_t RightDuty;               // duty cycle of right wheel (0 to 14,998)

#define TACHBUFF 10
extern uint16_t LeftTach[TACHBUFF];             // tachometer period of left wheel (number of 0.0833 usec cycles to rotate 1/360 of a wheel rotation)
extern uint16_t RightTach[TACHBUFF];            // tachometer period of right wheel (number of 0.0833 usec cycles to rotate 1/360 of a wheel rotation)
extern enum TachDirection LeftDir;              // direction of left rotation (FORWARD, STOPPED, REVERSE)
extern enum TachDirection RightDir;             // direction of right rotation (FORWARD, STOPPED, REVERSE)
extern int32_t LeftSteps;                       // number of tachometer steps of left wheel (units of 220/360 = 0.61 mm traveled)
extern int32_t RightSteps;                      // number of tachometer steps of right wheel (units of 220/360 = 0.61 mm traveled)

extern uint16_t Average_RPM_L[TACHBUFF];
extern uint16_t Average_RPM_R[TACHBUFF];

extern char macStr[18];        // Formatted MAC Address String
extern char uniqueID[9];       // Unique ID generated from TLV RAND NUM and MAC Address
extern char leftDist[5];
extern char centerDist[5];
extern char rightDist[5];
extern char rightRPM[5];
extern char leftRPM[5];

extern uint32_t Distances[3];
extern uint32_t FilteredDistances[3];
extern uint32_t Amplitudes[3];
extern uint32_t TxChannel;
extern uint32_t StartTime;
extern uint32_t TimeToConvert; // in msec
extern int i;

extern unsigned char macAddressVal[SL_MAC_ADDR_LEN];
extern unsigned char macAddressLen;
extern Network n;
extern Client hMQTTClient;     // MQTT Client
extern _u32  g_Status;

#define MQTT_BROKER_SERVER  "broker.hivemq.com"
#define SUBSCRIBE_TOPIC "MayaNet_Sub"
#define PUBLISH_TOPIC "MayaNet_Pub"
#define DEVICE_NOT_IN_STATION_MODE -0x7D0



///* Application specific status/error codes */
//typedef enum{
//    DEVICE_NOT_IN_STATION_MODE = -0x7D0,        /* Choosing this number to avoid overlap with host-driver's error codes */
//    HTTP_SEND_ERROR = DEVICE_NOT_IN_STATION_MODE - 1,
//    HTTP_RECV_ERROR = HTTP_SEND_ERROR - 1,
//    HTTP_INVALID_RESPONSE = HTTP_RECV_ERROR -1,
//    STATUS_CODE_MAX = -0xBB8
//}e_AppStatusCodes;


uint16_t avg(uint16_t *array, int length);

bool pollDistanceSensor(void);

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent);

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent);

void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse);

void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent);

void SimpleLinkSockEventHandler(SlSockEvent_t *pSock);

//static void generateUniqueID();
//
//static void messageArrived(MessageData* data);
//
//static void sendMessage(char* message,const char* topicName);

void PORT1_IRQHandler(void);

void TA1_0_IRQHandler(void);

//static _i32 configureSimpleLinkToDefaultState();
//
//static _i32 establishConnectionWithAP();
//
//static _i32 initializeAppVariables();
//
//static void displayBanner();
