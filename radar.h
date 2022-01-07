#ifndef _RADAR_H__
#define _RADAR_H__

//导入Arduino核心头文件
#include"Arduino.h"  

class radar
{  //public中声明的函数和变量可以被外部程序所访问，而private中声明的函数和变量，只能从这个类的内部访问。
    private:
        
    public:
        int Bodysign_val(int ad1, int ad2, int ad3, int ad4, int ad5);
        void Situation_judgment(int ad1, int ad2, int ad3, int ad4, int ad5);
        void Fall_judgment(int ad1, int ad2, int ad3, int ad4);
        char CRC(char ad1, char ad2, char ad3, char ad4, char ad5, char ad6, char ad7);
};

#endif
