#include "reg51.h"

#define KEY_LONG_PERIOD 50
#define KEY_CONTINUE_PERIOD 10

#define KEY_DOWN 0X02
#define KEY_LONG 0x04
#define KEY_CONTINUE 0x08
#define KEY_UP 0x01

#define KEY_STATE_INIT 0
#define KEY_STATE_WOBBLE 1
#define KEY_STATE_PRESS 2
#define KEY_STATE_LONG 3
#define KEY_STATE_CONTINUE 4
#define KEY_STATE_RELEASE 5

sbit key1 = P2^4;
sbit key2 = P2^5;
sbit key3 = P2^6;
sbit key4 = P2^7;

static int key_state ;
static int temp ;
static int key_time_count;
static int last_key ;


void key_init(void);
int scan_key(void);
void get_key(int *);

void key_init()
{
    P2 |= 0xf0;// 使用P2 口高四位做按键
    key1 = 1;
    key2 = 1;
    key3 = 1;
    key4 = 1;
    last_key = 0xf0;
    key_state = KEY_STATE_INIT;
}
int scan_key()
{
    if(key1 == 0) return 0xe0;
    if(key2 == 0) return 0xd0;
    if(key3 == 0) return 0xb0;
    if(key4 == 0) return 0x70;
    return 0xf0;
}

void get_key(int *value)
{

    temp = scan_key();

    switch (key_state)
    {
        case KEY_STATE_INIT :
        {
            if(temp != 0xf0)
            {
                key_state = KEY_STATE_WOBBLE;
            }
        }
        break;

        case KEY_STATE_WOBBLE:
        {
            key_state = KEY_STATE_PRESS;
        }
        break;

        case KEY_STATE_PRESS :
        {
            if(temp != 0xf0)
            {
                last_key = temp;
                temp |= KEY_DOWN;
                key_state = KEY_STATE_LONG;
            }
            else
            {
                key_state = KEY_STATE_INIT;
            }
        }
        break;

        case KEY_STATE_LONG:
        {
            if(temp != 0xf0)
            {
                if(++key_time_count > KEY_LONG_PERIOD)
                {
                    key_time_count = 0;
                    temp |= KEY_LONG;
                    key_state = KEY_STATE_CONTINUE;
                }
            }
            else
            {
                key_state = KEY_STATE_RELEASE;
            }
        }
        break;

        case KEY_STATE_CONTINUE:
        {
            if(temp != 0xf0)
            {
                if(++key_time_count > KEY_CONTINUE_PERIOD)
                {
                    key_time_count = 0;
                    temp |= KEY_CONTINUE;
                }
            }
            else
            {
                key_state = KEY_STATE_RELEASE;
            }
        }
        break;

        case KEY_STATE_RELEASE:
        {
            last_key |= KEY_UP;
            temp = last_key;
            key_state = KEY_STATE_INIT;
        }
        break;

        default: break;

    }
    *value = temp;

}


// void main()
// {
//     int key_value ;
//     P0 = 0x11;
//     P2 = 0xff;
//
//     key_init();
//     while(1)
//     {
//         // get_key( &key_value);
//         get_key(&key_value);
//         if(key_value == (0xe0 | KEY_DOWN)) P0 = 0xf0 ;
//         if(key_value == (0xe0 | KEY_LONG)) P0 = 0x0f ;
//         if(key_value == (0xe0 | KEY_CONTINUE)) { P0 = 0xaa;}
//         if(key_value == (0xe0 | KEY_UP)) P0 = 0x00 ;
//     }
// }
