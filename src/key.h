#ifndef _KEY_
#define _KEY_

#define KEY_DOWN 0X02
#define KEY_LONG 0x04
#define KEY_CONTINUE 0x08
#define KEY_UP 0x01

#define KEY1  0xe0
#define KEY2  0xd0
#define KEY3  0xb0
#define KEY4  0x70

#define SELECT  0xe0
#define PLUS    0xd0
#define MINUS   0xb0
#define CONFIRM 0x70



extern  void key_init(void);
extern  void get_key(int *);

#endif
