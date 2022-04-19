#ifndef _RADAR_H__
#define _RADAR_H__

#define MESSAGE_HEAD 0x55       //Data frame header
#define ACTIVE_REPORT 0x04      //Proactive reporting

#define REPORT_RADAR 0x03       //Report radar information
#define REPORT_OTHER 0x05       //Report other information
#define REPORT_FALL 0x06        //Report a fall

#define HEARTBEAT 0x01          //Heartbeat Pack
#define ABNOEMAL 0x02           //Abnormal Reset
#define ENVIRONMENT 0x05        //Environment
#define BODYSIGN 0x06           //Physical parameters
#define CLOSE_AWAY 0x07         //Approach away

#define CA_BE 0x01              //Approach away head frame
#define CA_CLOSE 0x02           //Someone approaches
#define CA_AWAY 0x03            //Some people stay away
#define SOMEBODY_BE 0x01        //Motion state header frame
#define SOMEBODY_MOVE 0x01      //Somebody move
#define SOMEBODY_STOP 0x00      //Somebody stop
#define NOBODY 0x00             //No one here

#define ALARM 0x01
#define ALARM_FALL 0x01
#define ALARM_RESIDUAL 0x02

#define SUSPECTED_FALL 0x00
#define REAL_FALL 0x01
#define NO_FALL 0x02

#define NONE 0x00
#define FIRST 0x01
#define SENCOND 0x02
#define THIRD 0x03
#define FORTH 0x04

class FallDetectionRadar{
    private:
        
    public:
        const byte MsgLen = 12;
        byte dataLen = 12;
        byte Msg[12];
        boolean newData = false;
        void SerialInit();
        void recvRadarBytes();
        void Bodysign_judgment(byte inf[], float Move_min, float Move_max);
        void Situation_judgment(byte inf[]);
        void Fall_inf(byte inf[]);
        void ShowData(byte inf[]);
        unsigned short int us_CalculateCrc16(unsigned char *lpuc_Frame, unsigned short int lus_Len);
};

#endif
