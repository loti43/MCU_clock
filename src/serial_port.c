#include "reg51.h"

#define FOSC 12000000L
#define BAUD 9600


void init_serial_port(void);
void send_data(char);

/*************************************************/

void send_data(char dat)
{
    while (!TI);                    //等待前一个数据发送完成
    TI = 0;                         //清除发送标志
    SBUF = dat;                     //发送当前数据
}


void init_serial_port()
{
    SCON = 0x40;
    REN = 1;
    // PCON = 0x00;
    TMOD &= 0x0f;
    TMOD |= 0x20;
    // TH1 = TL1 =256-(FOSC/12/32/BAUD);
    TH1 = TL1 = 253;
    TI = 1;
    TR1 = 1;
}
