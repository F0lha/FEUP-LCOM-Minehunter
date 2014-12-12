#include "ModRato.h"

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

int mouse_int_handler() {


	unsigned long mouse_byte;
	int testing = 4;
	while(testing > 0){
		if(check_kbd_inbf())
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
