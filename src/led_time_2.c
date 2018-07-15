#include "reg51.h"
#include "stdio.h"
#include "key.h"
#include "serial_port.h"

#define NORMAL_RUN 0
#define SET_HOUR 1
#define SET_MIN 2
#define SET_SEC 3

sbit LSA=P1^7;   //38移位按键
sbit LSB=P1^6;
sbit LSC=P1^5;
// sfr AUXR = 0x8e;

char time_count;
int display_nums[11] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67,0x40}; // 0-9 and -
int t [8] ;
int mode;
// int the_last_key;
int t_count;
int reg_value;
char hour = 0;
char min = 0;
char sec = 0;

void time_init(void); // 计时器0 初始化
void time_cal(int *);  // 时间计算
void select_led(int);  // select index of leds 38译码器  使用 74hc138
void led_display(int,int); // 数码管显示
void prinf_(void);
void set_time(void);


void select_led(int i)
{
        switch(i)
        {
            case 0:
                LSA = 0; LSB = 0; LSC = 0; break;
            case 1:
                LSA = 1; LSB = 0; LSC = 0; break;
            case 2:
                LSA = 0; LSB = 1; LSC = 0; break;
            case 3:
                LSA = 1; LSB = 1; LSC = 0; break;
            case 4:
                LSA = 0; LSB = 0; LSC = 1; break;
            case 5:
                LSA = 1; LSB = 0; LSC = 1; break;
            case 6:
                LSA = 0; LSB = 1; LSC = 1; break;
            case 7:
                LSA = 1; LSB = 1; LSC = 1; break;
            default:
                break;
        }
}


void time_cal( int *a)
{
    char dpl [8] = {0,0,10,0,0,10};
    int i ;

    if(time_count == 60)
    {
        min += 1;
        time_count = 0 ;
    }
    sec = time_count;

    if(min == 60)
    {
        hour += 1;
        min = 0 ;
    }
    if(hour == 24)
    {
        hour = 0;
    }
    dpl[0] = hour / 10;
    dpl[1] = hour % 10;
    dpl[3] = min / 10;
    dpl[4] = min % 10;
    dpl[6] = sec / 10;
    dpl[7] = sec % 10;

    for(i=0;i<=7;i++)
    {
        a[i] = display_nums[dpl[i]];
    }

}


void time_init(void)		//1毫秒@11.0592MHz
{

	TMOD &= 0xf0;
	TMOD |= 0x01;
	TH0 = 0xfc;
	TL0 = 0x21;
	TF0 = 0;
	TR0 = 1;

}

void led_display( int display_mode,int index)
{
    P0 = 0x00;
    switch(display_mode)
    {
        case NORMAL_RUN:
        {
            select_led(index);
                P0 = t[index];
        }

        break;

        case SET_HOUR:
        {
            if(index == 0 || index == 1)
            {
                if(t_count > 400)
                {
                    select_led(index);
                    P0 = t[index];
                }

            }
            else
            {
                select_led(index);
                P0 = t[index];
            }

        }

        break;

        case SET_MIN:
        {
            if(index == 3 || index == 4)
            {
                if(t_count > 400)
                {
                    select_led(index);
                    P0 = t[index];
                }

            }
            else
            {
                select_led(index);
                P0 = t[index];
            }

        }

        break;


        case SET_SEC:
        {
            if(index == 6 || index == 7)
            {
                if(t_count > 400)
                {
                    select_led(index);
                    P0 = t[index];
                }

            }
            else
            {
                select_led(index);
                P0 = t[index];
            }

        }

        break;

    }



}

void set_time()
{
    switch(mode)
    {
        case NORMAL_RUN:
		{
            if((reg_value == (SELECT | KEY_DOWN))||(reg_value == (SELECT | KEY_LONG))){
               mode = SET_HOUR;
            }
            if((reg_value == (CONFIRM | KEY_DOWN))||(reg_value == (CONFIRM | KEY_LONG))){
                mode = NORMAL_RUN;
            }
            if(reg_value == (SELECT | KEY_DOWN)){
               mode = SET_HOUR;
            }
            if(reg_value == (CONFIRM | KEY_DOWN)){
                mode = NORMAL_RUN;
            }
        }
        break;

        case SET_HOUR:{
            if((reg_value == (SELECT | KEY_DOWN))||(reg_value == (SELECT | KEY_LONG))){
                mode = SET_MIN;
            }
            if((reg_value == (CONFIRM | KEY_DOWN))||(reg_value == (CONFIRM | KEY_LONG))){
                mode = NORMAL_RUN;
            }
            // if(reg_value == (SELECT | KEY_CONTINUE)){
            //     mode = SET_MIN;
            // }
            if(reg_value == (CONFIRM | KEY_CONTINUE)){
                mode = NORMAL_RUN;
            }
            if(reg_value == (PLUS | KEY_DOWN)){
                hour += 1;
            }
            if(reg_value == (PLUS | KEY_CONTINUE)){
                hour += 1;
            }
            if(reg_value == (MINUS | KEY_DOWN)){
                hour -= 1;
                if(hour < 0)
                hour += 24;
            }
            if(reg_value == (PLUS | KEY_CONTINUE)){
                hour -= 1;
                if(hour < 0)
                hour += 24;
            }
        }
        break;

        case SET_MIN:{
            if((reg_value == (SELECT | KEY_DOWN))||(reg_value == (SELECT | KEY_LONG))){
                mode = SET_SEC;
            }
            if((reg_value == (CONFIRM | KEY_DOWN))||(reg_value == (CONFIRM | KEY_LONG))){
                mode = NORMAL_RUN;
            }
            // if(reg_value == (SELECT | KEY_CONTINUE)){
            //     mode = SET_SEC;
            // }
            if(reg_value == (CONFIRM | KEY_CONTINUE)){
                mode = NORMAL_RUN;
            }
            if(reg_value == (PLUS | KEY_DOWN)){
                min += 1;
            }
            if(reg_value == (PLUS | KEY_CONTINUE)){
                min += 1;
            }
            if(reg_value == (MINUS | KEY_DOWN)){
                min -= 1;
                if(min < 0)
                min += 60;
            }
            if(reg_value == (PLUS | KEY_CONTINUE)){
                min -= 1;
                if(min < 0)
                min += 60;
            }
        }
        break;

        case SET_SEC:{
            if((reg_value == (SELECT | KEY_DOWN))||(reg_value == (SELECT | KEY_LONG))){
                mode = SET_HOUR;
            }
            if((reg_value == (CONFIRM | KEY_DOWN))||(reg_value == (CONFIRM | KEY_LONG))){
                mode = NORMAL_RUN;
            }
            // if(reg_value == (SELECT | KEY_CONTINUE)){
            //     mode = SET_HOUR;
            // }
            if(reg_value == (CONFIRM | KEY_CONTINUE)){
                mode = NORMAL_RUN;
            }
            if(reg_value == (PLUS | KEY_DOWN)){
                sec += 1;
            }
            if(reg_value == (PLUS | KEY_CONTINUE)){
                sec += 1;
            }
            if(reg_value == (MINUS | KEY_DOWN)){
                sec -= 1;
                if(sec < 0)
                sec += 60;
            }
            if(reg_value == (PLUS | KEY_CONTINUE)){
                sec -= 1;
                if(sec < 0)
                sec += 60;
            }
        }
        break;

    default: break;

    }
}

main()
{
    int i = 0;
    mode = NORMAL_RUN;
    time_count = 0;
    time_init();
    key_init();
    init_serial_port();
    P0 = 0x00;
    time_cal(t);

    while(1)
    {
        if(TF0){

            TH0 = 0xfc;	//设置定时初值
            TL0 = 0x21;	//设置定时初值  初略测试 996每秒误差4-5 ms 改992
            TF0 = 0;
            t_count++;

            if(t_count % 1 == 0){
                led_display(mode,i++);
                if(i>7) i = 0;
            }

            if(t_count % 10 ==0){
                get_key(&reg_value);
            }

            if(t_count % 20 ==0){
                set_time();
            }

            if(t_count % 50 == 0){
                time_cal(t);
            }

			if(t_count % 1000 == 0){
            	t_count = 0;
				time_count++;
			}

        }
    }
}
