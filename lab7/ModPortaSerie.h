#ifndef __MODPORTASERIE_H
#define __MODPORTASERIE_H

int hook_id;

#define BIT(n) (0x01<<(n))

#define UART_FREQ 		115200

#define COM1_ADDR		0x3f8
#define COM1_IRQ		4
#define COM2_ADDR		0x2f8
#define COM2_IRQ		3

#define DLL 			0
#define DLM				1

#define LCR				3
#define IER				1
#define LSR				5
#define RB				0
#define TH				0



#define RR				0
#define THRE			5

void changeDLAB(unsigned short base_addr,int bit);



void setPorta(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate);


void set_poll(unsigned short base_addr);

#endif
