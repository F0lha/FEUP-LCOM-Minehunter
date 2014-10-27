#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include "i8254.h"
#include "keyboard.h"

#define PAR_ERR 	BIT(7)
#define TO_ERR 		BIT(6)
#define IBF 		BIT(1)
#define OBF 		BIT(0)
#define RESEND 		0xFE
#define ERROR 		0xFC

unsigned long data;

int kbd_subscribe_int(void) {
	int value = KBD_HOOK_ID;
	sys_irqsetpolicy(IRQ1, IRQ_REENABLE | IRQ_EXCLUSIVE, &value);
	sys_irqenable(&value);
	return BIT(KBD_HOOK_ID);
}

int kbd_unsubscribe_int() {
	int value = KBD_HOOK_ID;
	return (sys_irqrmpolicy(&value)||sys_irqdisable(&value));
}

void kbd_int_handler(){
	sys_inb(IN_BUF,&scan_code);
}

void kbd_command_leds(short leds){

	unsigned long cenas;
	unsigned passou1 =0,passou2 = 0;
	while(!passou1){
		sys_outb(OUT_BUF,0xED);
		sys_inb(IN_BUF,&cenas);
		if(cenas == RESEND)
			continue;
		else if(cenas == ERROR)
			kbd_command_leds(leds);
		else
			passou1 =1;
	}
	while(!passou2){
		sys_outb(OUT_BUF,leds);
		sys_inb(IN_BUF,&cenas);
		if(cenas == RESEND)
			continue;
		else if(cenas == ERROR)
			kbd_command_leds(leds);
		else
			passou2 =1;
	}


}

/*
int receive_kbd(){
	unsigned long stat;
	while( 1 ) {
			sys_inb(STAT_REG, &stat); /* assuming it returns OK */
			/* loop while 8042 output buffer is empty
			if( stat & OBF ) {
				sys_inb(OUT_BUF, &data); /* assuming it returns OK
				if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
					return data;
				else
					return -1;
			}
			tickdelay(micros_to_ticks(DELAY_US));
		}
}
*/

int send_kbd(short cmd){
	unsigned long stat;
	while( 1 ) {
		sys_inb(STAT_REG, &stat);
		if(stat != OK)
			continue;
		if( (stat & IBF) == 0 ) {
			sys_outb(KBC_CMD_REG,cmd); /* no args command */
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}
/*
  	send_kbd(0xED);
	cenas = receive_kbd();
	while(1){
		if(cenas==ERROR)
			kbd_command_leds(leds);
		else if (cenas == RESEND)
			send_kbd(0xED);
		else break;
		cenas = receive_kbd();
	}
	send_kbd(leds);
	while(1){
			if(cenas==ERROR)
				kbd_command_leds(leds);
			else if (cenas == RESEND)
				send_kbd(leds);
			else break;
			cenas = receive_kbd();
		}
		*/

