#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "timer.h"



int global_counter,hook_id;

int timer_set_square(unsigned long timer, unsigned long freq) {

	unsigned long div;
	unsigned char arg = 0;

	printf("%lu \n",freq);

	div = TIMER_FREQ/freq;
	unsigned long freq_lsb = div;
	unsigned long freq_msb = (div >> 8);


	if (timer == 0) arg |= TIMER_SEL0;
	else if (timer == 1) arg |= TIMER_SEL1;
	else if (timer == 2) arg |= TIMER_SEL2;
	else return 1;

	arg |= TIMER_LSB_MSB;
	arg |= TIMER_SQR_WAVE;
	arg |= TIMER_BIN;

	printf("%lu   %lu\n",freq_lsb,freq_msb);

	sys_outb(TIMER_CTRL, arg);
	sys_outb(TIMER_0 + timer, freq_lsb);
	sys_outb(TIMER_0 + timer, freq_msb);

	return 0;

}

int timer_subscribe_int(void ) {



	sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id);
	sys_irqenable(&hook_id);

	return ;
}

int timer_unsubscribe_int() {

	return sys_irqrmpolicy(&hook_id);
}

void timer_int_handler() {
global_counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {



	unsigned long arg = TIMER_RB_CMD | TIMER_RB_SEL(timer)|TIMER_RB_COUNT_;
	unsigned long t = TIMER_0 + timer;
	unsigned long var;

		sys_outb(TIMER_CTRL, arg);
		sys_inb(t, &var);
		*st = var;
		return 0;

}


int timer_display_conf(unsigned char conf) {
	
	int bcd,prog_mode,type_access,null_counter,output;
	int temp_conf;
	temp_conf=conf;
	temp_conf &= 1;
	bcd=temp_conf;
	temp_conf = conf;
	temp_conf >> 1;
	temp_conf &= 7;
	prog_mode=temp_conf;
	temp_conf = conf;
	temp_conf >> 4;
	temp_conf &= 3;
	type_access=temp_conf;
	temp_conf = conf;
	temp_conf >> 6;
	temp_conf &= 1;
	null_counter=temp_conf;
	temp_conf = conf;
	temp_conf >> 7;
	temp_conf &= 1;
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
	else printf("cenas %d\n", prog_mode);

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
	
	if(freq==0){
			return 1;
		}
	else{

	int res = timer_set_square(0,freq);
	return res;
}
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {

	if (timer>2){
		return 1;
	}

	char test;
	timer_get_conf(timer,&test);
	timer_display_conf(test);
	return 0;
}
