#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "timer.h"





int timer_set_square(unsigned long timer, unsigned long freq) {



	return 1;
}

int timer_subscribe_int(void ) {

	return 1;
}

int timer_unsubscribe_int() {

	return 1;
}

void timer_int_handler() {

}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	if (timer != 0 | timer != 1 | timer != 2) return 1;
	else {

	unsigned long arg = TIMER_RB_CMD | TIMER_RB_SEL(timer)|TIMER_RB_COUNT_;
	unsigned long t = TIMER_0 + timer;
	unsigned long var;

		sys_outb(TIMER_CTRL, arg);
		sys_inb(t, &var);
		*st = var;
		return 0;
	}
}


int timer_display_conf(unsigned char conf) {
	
	int bcd,prog_mode,type_access,null_counter,output;
	int temp_conf;
	temp_conf=conf;
	temp_conf &= 0x01;
	bcd=temp_conf;
	temp_conf = conf;
	temp_conf >> 1;
	temp_conf &= 0x111;
	prog_mode=temp_conf;
	temp_conf = conf;
	temp_conf >> 4;
	temp_conf &= 0x11;
	type_access=temp_conf;
	temp_conf = conf;
	temp_conf >> 6;
	temp_conf &= 0x01;
	null_counter=temp_conf;
	temp_conf = conf;
	temp_conf >> 7;
	temp_conf &= 0x01;
	output=temp_conf;

	if(bcd == 1){
		printf("bcd\n");
	}
	else{
		printf("binary\n");
	}

	if(null_counter == 1){
			printf("null_counter = 1\n");
		}
		else{
			printf("null_counter = 0\n");
		}
	if(prog_mode == 0)
		printf("INTERRUPT ON TERMINAL COUNT\n");
	else if(prog_mode == 1)
		printf("HARDWARE RETRIGGERABLE ONE-SHOT\n");
	else if(prog_mode == 2)
		printf("RATE GENERATOR\n");
	else if(prog_mode == 3)
		printf("SQUARE WAVE MODE\n");
	else if(prog_mode == 4)
		printf("SOFTWARE TRIGGERED STROBE\n");
	else if(prog_mode == 5)
		printf("HARDWARE TRIGGERED STROBE (RETRIGGERABLE)\n");

	if(output == 0)
		printf("OUTPUT = 0\n");
	else printf("OUTPUT = 1\n");

	if(type_access == 0)
		printf("LSB\n");
	else if(type_access == 1)
		printf("MSB\n");
	else printf("LSB FOLLOWED BY MSB\n");

	return 0;
}

int timer_test_square(unsigned long freq) {
	
	unsigned long arg = TIMER_SEL0|TIMER_SQR_WAVE|TIMER_LSB|TIMER_BIN;
	sys_outb(TIMER_CTRL, arg);


	int new_freq = freq;
	sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &new_freq);
	sys_irqenable(&new_freq);
	return 0;
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {



	unsigned char test;
	timer_get_conf(timer,&test);
	timer_display_conf(test);
	return 0;
}
