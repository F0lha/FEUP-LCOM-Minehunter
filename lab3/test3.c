#include <minix/syslib.h>
#include <minix/drivers.h>

#include "test3.h"

#define OUT_BUF 0x60
unsigned int kbd_hook_id = 0;


int main (){
	sef_startup();

	return 0;

}


int kbd_subscribe_int(void) {
	int value = kbd_hook_id;
	sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
	sys_irqenable(&kbd_hook_id);
	return value;
}

int kbd_unsubscribe_int() {
	return (sys_irqrmpolicy(&kbd_hook_id)||sys_irqdisable(&kbd_hook_id));
}

void kbd_int_handler(){

}

int kbd_test_scan(unsigned short ass); {
	/* To be completed */
	int ipc_status, i = 1, r;
	message msg;

		kbd_subscribe_int();

		int irq_set = BIT(kbd_subscribe_int());

		while(i) {
			/* Get a request message. */
			if ( (r=driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
				printf("driver_receive failed with: %d", r);
				continue;
			}

			if (is_ipc_notify(ipc_status)) { // received notification
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: /* hardware interrupt notification */
					if (msg.NOTIFY_ARG & irq_set) { // subscribed interrupt

						/* process it */
						if (ass==0){ //C code
							// kbd_int_handler();
						}
						else { //Assembly code

						}
					}
				}
				break;
default:
	break; // no other notifications expected: do nothing
			}



	kbd_unsubscribe_int();
}


int kbd_test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}
int kbd_test_timed_scan(unsigned short n) {
    /* To be completed */
}
