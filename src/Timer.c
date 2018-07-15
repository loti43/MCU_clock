#include "reg51.h"

bit osTime1ms = 0;// os time 1ms
char i ;


void Timer0Init(void);

void Timer0Init()
{
    TMOD &= 0xf0;
    TMOD |= 0x01;
    TH0 = 0xd8;
    TL0 = 0xf0; // 10ms
    // EA = 1; ET0 = 1;//中断开关
    i = 0;
    TR0 = 1; //TCON
    while(1);
}

void time0() interrupt 1
{
    TH0 = 0xd8;
    TL0 = 0xf0; // 10ms
    i++;
    if(i==100)
    {
        P0 = ~P0; i = 0;
    }
}

// void main()
// {
//     P0 = 0xaa;
//     Timer0Init();
// }
