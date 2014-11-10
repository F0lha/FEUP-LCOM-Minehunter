#include "test4.h"
#include "mouse.h"
#include "i8254.h"
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>

unsigned long mouse_byte;

int mouse_subscribe_int(void) {
	int value = MOUSE_HOOK_ID;
	sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &value);
	sys_irqenable(&value);
	return MOUSE_HOOK_ID;
}

int mouse_unsubscribe_int() {
	int value = MOUSE_HOOK_ID;
	return (sys_irqrmpolicy(&value) || sys_irqdisable(&value));
}

int enable_packets() {
	unsigned long ret;
	/*if (sys_outb(STATUS_PORT, ENABLE_MOUSE) != OK) { // manda a8 para 64
		return 1;
	}*/
	if (sys_outb(STATUS_PORT, KBC_CMD_MOUSE) != OK) {// manda d4 para 64
		return 1;
	}
	RESEND_TRY:
	if (sys_outb(OUT_BUF, ENABLE_PACKETS) != OK) {//manda f4 para 60
			return 1;
		}
	if (sys_inb(OUT_BUF, &ret) != OK) {
		return 1;
	}
	if (ret == ERROR) {
		enable_packets();
	}
	else if(ret == RESEND)
		goto RESEND_TRY;
}

int check_kbd_ibf(){
	unsigned long ret;
	sys_inb(STAT_REG,&ret);
	ret &= IBF;
	return (int) ret;
}


int mouse_int_handler() {


	unsigned long mouse_byte;
	int testing = 4;
	while(testing > 0){
		if(check_kbd_ibf())
		{
			printf("ta cheio \n");
			 tickdelay(micros_to_ticks(DELAY_US));
			continue;
		}
		testing--;
	}

	if (sys_inb(OUT_BUF, &mouse_byte) != OK) {
		return 1;
	} else
		return mouse_byte;
}

int receive_kbd() {
	unsigned long stat,data;
	while (1) {
		sys_inb(STAT_REG, &stat); /* assuming it returns OK */
		/* loop while 8042 output buffer is empty*/
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK*/
			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

int send_kbd(short cmd) {
	unsigned long stat;
	while (1) {
		sys_inb(STAT_REG, &stat);
		if (stat != OK)
			continue;
		if ((stat & IBF) == 0) {
			sys_outb(OUT_BUF, cmd); /* no args command */
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

int get_mouse_status(){

	int irq_set = mouse_subscribe_int();
	unsigned long ret;
	if (sys_outb(STATUS_PORT, KBC_CMD_MOUSE) != OK) {// manda d4 para 64
		return 1;
	}
	if (sys_outb(OUT_BUF, DSM) != OK) {// manda F5 para 60
		return 1;
	}
	if(sys_inb(OUT_BUF, &ret) != OK){
		return 1;
	}
	if(ret!=ACK)
		get_mouse_status();
	if (sys_outb(STATUS_PORT, KBC_CMD_MOUSE) != OK) {// manda d4 para 64
			return 1;
		}
	if (sys_outb(OUT_BUF, STATUS_REQUEST) != OK) {
		return 1;
	}
	if(sys_inb(OUT_BUF, &ret) != OK){
		return 1;
	}
	if(ret!=ACK)
		get_mouse_status();
	return irq_set;
}
