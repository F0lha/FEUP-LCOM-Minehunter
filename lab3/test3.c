#include <minix/syslib.h>
#include <minix/drivers.h>

#include "test3.h"
#include "keyboard.h"
#include "i8254.h"
#include "timer.h"




int kbd_test_scan(unsigned short ass) {

	int two_bytes = 0;
	int ipc_status, r;
	message msg;

	int irq_set = kbd_subscribe_int();

	while(scan_code!=BREAK_CODE_ESC) {

		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					if (ass==0){ //C code
						kbd_int_handler();
						if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
						else if(scan_code>>7){
							if(two_bytes) printf("BREAKCODE : 0xE0%x\n",scan_code & 0xff);
							else printf("BREAKCODE : 0x%x\n",scan_code & 0xff);
							if (scan_code==BREAK_CODE_ESC)
								break;
						}
						else
						{
							if (two_bytes){
								printf("MAKCODE : 0xE0%x\n",scan_code & 0xff);
								two_bytes=0;
							}
							else printf("MAKECODE : 0x%x\n",scan_code & 0xff);
						}
					}
					else { //Assembly code

					}
				}
				break;
			default:
				break; // no other notifications expected: do nothing
			}
		}
	}
	kbd_unsubscribe_int();
}


int kbd_test_leds(unsigned short n, unsigned short *leds) {
	int ipc_status;
	message msg;
	int i = 0;
	int led_0 = 0, led_1 = 0, led_2 = 0;
	int counter = 0;
	int irq_set = BIT(timer_subscribe_int());

	while (i < n)
	{
		/* Get a request message. */
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {

			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					counter++;
					if (counter >= 60) {
						counter = 0;
						printf("LED: %d\n",leds[i]);
						switch (leds[i]) {
						case 0:
							if (led_0 == 0){
								printf("Scroll Lock Led On\n");
								led_0 = BIT(0);
							}
							else
							{
								led_0 = 0;
								printf("Scroll Lock Led Off\n");
							}
							break;
						case 1:
							if (led_1 == 0)
							{
								led_1 = BIT(1);
								printf("Num Lock Led On\n");
							}
							else
							{
								led_1 = 0;
								printf("Num Lock Led Off\n");
							}
							break;
						case 2:
							if (led_2 == 0)
							{
								led_2 = BIT(2);
								printf("Caps Lock Led On\n");
							}
							else
							{
								led_2 = 0;
								printf("Caps Lock Led Off\n");
							}
							break;
						default:
							printf("Invalid Led %d\n", i);
							continue;
						}

						int leds = led_0 | led_1 | led_2;

						kbd_command_leds(leds);
						i++;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}




	}

}



int kbd_test_timed_scan(unsigned short n) {
	/* To be completed */
}
