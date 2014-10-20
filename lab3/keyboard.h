#ifndef __KEYBOARD_H
#define __KEYBOARD_H


#define OUT_BUF 0x60
#define KBD_HOOK_ID   0x00
#define DELAY_US 20000
#define TWO_BYTE_CODE 0xE0
#define BREAK_CODE_ESC 0x81

unsigned long scan_code;



void kbd_int_handler();
int kbd_unsubscribe_int();
int kbd_subscribe_int(void);










#endif