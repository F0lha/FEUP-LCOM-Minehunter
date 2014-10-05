#include <minix/syslib.h>
#include <minix/drivers.h>

int main(){

}

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

	unsigned long arg = TIMER_RB_CMD | TIMER_RB_SEL(timer)
	unsigned long t = TIMER_0 + timer;
	unsigned long var;
	else {
		sys_outb(TIMER_CTRL, arg);
		sys_inb(t, &var);
		*st = char(var);
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
	prog_mode=temp_conf;
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


	return 1;
}

int timer_test_square(unsigned long freq) {
	
	return 1;
}

int timer_test_int(unsigned long time) {
	
	return 1;
}

int timer_test_config(unsigned long timer) {
	
	return 1;
}
