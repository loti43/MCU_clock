#include "reg51.h"
#include "key.h"

// void main()
// {
//     int key_value ;
//     P0 = 0x11;
// //    P2 = 0xff;
//
//     key_init();
//     while(1)
//     {
//         // get_key( &key_value);
//         get_key(&key_value);
//         if(key_value == (SELECT | KEY_DOWN)) P0 = 0xf0 ;
//         if(key_value == (SELECT | KEY_LONG)) P0 = 0x0f ;
//         if(key_value == (SELECT | KEY_CONTINUE)) { P0 = 0xaa;}
//         if(key_value == (SELECT | KEY_UP)) P0 = 0xff ;
//     }
// }
