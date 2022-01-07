#include <radar.h>

#define MESSAGE_HEAD 0x55
int data[14] = {0};
int i = 0;
int Msg;

radar RADAR;

void setup()
{
  Serial1.begin(9600);
  Serial.begin(9600);
  Serial.println("Readly");
}

void Bodysign_judgment(int ad1, int ad2, int ad3, int ad4, int ad5){
  float s;
  s = RADAR.Bodysign_val(ad1, ad2, ad3, ad4, ad5);
  if(s > 1.5 && s < 35){
    Serial.println("SOMEBODY STOP");
  }
  else if(s < 1.5){
    Serial.println("NOBODY");
  }
  else if(s > 35){
    Serial.println("SOMEBODY MOVE");
  }
}

void loop()
{
  Msg = Serial1.read();
  if(Msg == MESSAGE_HEAD){
    for(i = 0; i<14; i++){
      data[i] = Msg;                         
      Msg = Serial1.read();
      if (Msg == MESSAGE_HEAD){
//        Bodysign_judgment(data[5], data[6], data[7], data[8], data[9]);   //体征数据解码
        RADAR.Situation_judgment(data[5], data[6], data[7], data[8], data[9]);   //雷达数据解码
        Serial.println("------------------------");
        continue;
      }
      delay(25);
     }
   }
}
