#include <Dist.h>

uint32_t Distances[3];
uint32_t FilteredDistances[3];
uint32_t Amplitudes[3];
uint32_t TxChannel;
uint32_t channel;
int i_dist;

bool pollDistanceSensor(void)
{
  if(OPT3101_CheckDistanceSensor())
  {
    TxChannel = OPT3101_GetMeasurement(Distances,Amplitudes);
    return true;
  }
  return false;
}

int32_t Left(int32_t left){
  return (1247*left)/2048 + 22;
}

int32_t Right(int32_t right){
  return  (right*(59*right + 7305) + 2348974)/32768;
}

void Dist_Init(void)
{
    I2CB1_Init(30); // baud rate = 12MHz/30=400kHz
    channel = 0;
    Clock_Init48MHz();

    OPT3101_Init();
    OPT3101_Setup();
    OPT3101_CalibrateInternalCrosstalk();
    OPT3101_ArmInterrupts(&TxChannel, Distances, Amplitudes);
    TxChannel = 3;
    OPT3101_StartMeasurementChannel(channel);

    LPF_Init(100,8);
    LPF_Init2(100,8);
    LPF_Init3(100,8);
    EnableInterrupts();
    i_dist = 0;
}

void getDist(uint32_t *distanceBuf)
{
    if(TxChannel <= 2){ // 0,1,2 means new data
      if(TxChannel==0){
        if(Distances[0] < 1000){
            distanceBuf[0] = Distances[0];
        }else{
            distanceBuf[0] = 1000;
        }
      }else if(TxChannel==1){
        if(Distances[1] < 1000){
            distanceBuf[1] = Distances[1];
        }else{
            distanceBuf[1] = 1000;
        }
      }else {
        if(Distances[2] < 1000){
            distanceBuf[2] = Distances[2];
        }else{
            distanceBuf[2] = 1000;
        }
      }
      TxChannel = 3; // 3 means no data
      channel = (channel+1)%3;
      OPT3101_StartMeasurementChannel(channel);
      i_dist = i_dist + 1;
    }
}
