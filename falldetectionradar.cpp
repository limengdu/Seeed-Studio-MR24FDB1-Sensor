#include "Arduino.h"
#include "falldetectionradar.h"

#ifdef __AVR__
    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(2, 3); // RX, TX
    #define Serial1 SSerial
#endif

void FallDetectionRadar::SerialInit(){
  Serial1.begin(9600);
}

// Receive data and process
void FallDetectionRadar::recvRadarBytes(){
  static boolean recvInProgress = false;
  static byte ndx = 0;
  byte startMarker = MESSAGE_HEAD;            //Header frame
  byte rb; // Each frame received
  while (Serial1.available() > 0 && newData == false) {
      rb = Serial1.read();
      if (recvInProgress == true) {           //Received header frame
          if (dataLen > ndx) {                //Length in range
            Msg[ndx] = rb;
            if(ndx == 0)dataLen = Msg[0];     //Get length frame
            ndx++;
          }
          else {                              //Ending the information acquisition of a set of data
              recvInProgress = false;
              ndx = 0;
              newData = true;
          }
      }
      else if (rb == startMarker) {           //Waiting for the first frame to arrive
          recvInProgress = true;
      }
  }
}

// Unpacking of physical parameters
void FallDetectionRadar::Bodysign_judgment(byte inf[], float Move_min, float Move_max){
  typedef union
  {
    unsigned char Byte[4];
    float Float;
  }Float_Byte;
  if(inf[3] == ACTIVE_REPORT){
    if(inf[5] == BODYSIGN){
        Float_Byte fb;
        fb.Byte[0] = inf[6];
        fb.Byte[1] = inf[7];
        fb.Byte[2] = inf[8];
        fb.Byte[3] = inf[9];
        ShowData(inf);
        Serial.println(fb.Float);
        if(fb.Float >= Move_min && fb.Float < Move_max){
          Serial.println("SOMEBODY STOP");
        }
        else if(fb.Float < Move_min){
          Serial.println("NOBODY");
        }
        else if(fb.Float >= Move_max){
          Serial.println("SOMEBODY MOVE");
        }
        Serial.println("----------------------------");
    }
  }
}

// Judgment of occupied and unoccupied, approach and distance
void FallDetectionRadar::Situation_judgment(byte inf[]){
  switch(inf[3]){
    case ACTIVE_REPORT:
      switch(inf[4]){
        case REPORT_RADAR:
          switch(inf[5]){
            case ENVIRONMENT:
              ShowData(inf);
              switch(inf[6]){
                case NOBODY:
                  Serial.println("Radar detects no one.");
                  Serial.println("----------------------------");
                  break;
                case SOMEBODY_BE:
                  switch(inf[7]){
                    case SOMEBODY_MOVE:
                      Serial.println("Radar detects somebody is moving.");
                      Serial.println("----------------------------");
                      break;
                    case SOMEBODY_STOP:
                      Serial.println("Radar detects somebody is stopping.");
                      Serial.println("----------------------------");
                      break;
                  }
                  break;
              }
              break;
            case HEARTBEAT:
              ShowData(inf);
              switch(inf[6]){
                case NOBODY:
                  Serial.println("Radar detects no one.");
                  Serial.println("----------------------------");
                  break;
                case SOMEBODY_BE:
                  switch(inf[7]){
                    case SOMEBODY_MOVE:
                      Serial.println("Radar detects somebody is moving.");
                      Serial.println("----------------------------");
                      break;
                    case SOMEBODY_STOP:
                      Serial.println("Radar detects somebody is stopping.");
                      Serial.println("----------------------------");
                      break;
                  }
                  break;
              }
              break;
            case CLOSE_AWAY:
              ShowData(inf);
              switch(inf[6]){
                case CA_BE:
                  switch(inf[7]){
                    case CA_BE:
                      switch(inf[8]){
                        case CA_BE:
                          Serial.println("Radar detects no move.");
                          Serial.println("----------------------------");
                          break;
                        case CA_CLOSE:
                          Serial.println("Radar detects somebody close.");
                          Serial.println("----------------------------");
                          break;
                        case CA_AWAY:
                          Serial.println("Radar detects somebody away.");
                          Serial.println("----------------------------");
                          break;
                      }
                      break;
                  }
                  break;
              }
              break;
            case ABNOEMAL:
              ShowData(inf);
              Serial.println("An exception appears and the reset is complete.");
              Serial.println("----------------------------");
              break;
          }
          break;
        case REPORT_OTHER:
          switch(inf[5]){
            case ENVIRONMENT || HEARTBEAT:
              ShowData(inf);
              switch(inf[6]){
                case NOBODY:
                  Serial.println("Radar detects no one.");
                  Serial.println("----------------------------");
                  break;
                case SOMEBODY_BE:
                  switch(inf[7]){
                    case SOMEBODY_MOVE:
                      Serial.println("Radar detects somebody is moving.");
                      Serial.println("----------------------------");
                      break;
                    case SOMEBODY_STOP:
                      Serial.println("Radar detects somebody is stopping.");
                      Serial.println("----------------------------");
                      break;
                  }
                  break;
              }
              break;
            case CLOSE_AWAY:
              ShowData(inf);
              switch(inf[6]){
                case CA_BE:
                  switch(inf[7]){
                    case CA_BE:
                      switch(inf[8]){
                        case CA_BE:
                          Serial.println("Radar detects no move.");
                          Serial.println("----------------------------");
                          break;
                        case CA_CLOSE:
                          Serial.println("Radar detects somebody close.");
                          Serial.println("----------------------------");
                          break;
                        case CA_AWAY:
                          Serial.println("Radar detects somebody away.");
                          Serial.println("----------------------------");
                          break;
                      }
                      break;
                  }
                  break;
              }
              break;
            case ABNOEMAL:
              ShowData(inf);
              Serial.println("An exception appears and the reset is complete.");
              Serial.println("----------------------------");
              break;
          }
          break;
      }
      break;
  }
}

//Fall information output
void FallDetectionRadar::Fall_inf(byte inf[]){
  switch(inf[3]){
    case REPORT_FALL:
      switch(inf[4]){
        case ALARM:
          ShowData(inf);
          switch(inf[5]){
            case ALARM_FALL:
              switch (inf[6]){
                case SUSPECTED_FALL:
                  Serial.println("A suspected fall was detected.");
                  break;
                case REAL_FALL:
                  Serial.println("A real fall was detected.");
                  break;
                case NO_FALL:
                  Serial.println("A no fall was detected.");
                  break;
                }
              break;
            case ALARM_RESIDUAL:
              switch (inf[6]){
                case NONE:
                  Serial.println("No warning message at this time.");
                  break;
                case FIRST:
                  Serial.println("First warning prompt.");
                  break;
                case SENCOND:
                  Serial.println("Second warning prompt.");
                  break;
                case THIRD:
                  Serial.println("Third warning prompt.");
                  break;
                case FORTH:
                  Serial.println("Fourth warning prompt.");
                  break;
                }
              break;
            }
          Serial.println("----------------------------");
          break;
      }
      break;
  }
}

//Radar transmits data frames for display via serial port
void FallDetectionRadar::ShowData(byte inf[]){
  for (byte n = 0; n < dataLen+1; n++) {
    Serial.print(inf[n], HEX);
    Serial.print(' ');
  }
    Serial.println();
}

const unsigned char cuc_CRCHi[256]= {
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
  0x00, 0xC1, 0x81, 0x40
};

const unsigned char  cuc_CRCLo[256]= {
  0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
  0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
  0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
  0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
  0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
  0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
  0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
  0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
  0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
  0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
  0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
  0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
  0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
  0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
  0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
  0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
  0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
  0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
  0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
  0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
  0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
  0x41, 0x81, 0x80, 0x40
};

unsigned short int FallDetectionRadar::us_CalculateCrc16(unsigned char *lpuc_Frame, unsigned short int lus_Len){
  unsigned char luc_CRCHi = 0xFF;
  unsigned char luc_CRCLo = 0xFF;
  int li_Index=0;
  while(lus_Len--){
    li_Index = luc_CRCLo ^ *( lpuc_Frame++);
    luc_CRCLo = (unsigned char)( luc_CRCHi ^ cuc_CRCHi[li_Index]);
    luc_CRCHi = cuc_CRCLo[li_Index];
  }
  return (unsigned short int )(luc_CRCLo << 8 | luc_CRCHi);
}

