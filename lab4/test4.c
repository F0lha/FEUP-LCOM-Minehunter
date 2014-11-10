#include "test4.h"
#include "mouse.h"
#include "i8254.h"
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>

extern unsigned long mouse_byte;

int test_packet(unsigned short cnt) {
	int ipc_status, loops = 0,contador = 0;
	message msg;
	unsigned char packets[3];
	int irq_set = BIT(mouse_subscribe_int());
	enable_packets();
	while (loops != cnt) {
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
					mouse_int_handler();
					if ((contador == 0) && (mouse_byte & 8 == 8)){
						packets[0] = mouse_byte;
						contador++;
					}
					else if(contador == 0){
						continue;
					}
					else if(contador == 2){
						contador = 0;
						packets[2] = mouse_byte;
					 	printf("B1=0x%X\tB2=0x%X\tB3=0x%X\tLB=%d\tMB=%d\tRB=%d\tXOV=%d\tYOV=%d\tX=%d\tY=%d\n\n",
																packets[0], packets[1], packets[2],(packets[0] & BIT(0)),(packets[0] & BIT(2)) >> 2,(packets[0] & BIT(1)) >> 1, (packets[0] & BIT(6)) >> 6, (packets[0] & BIT(7)) >> 7, packets[1], packets[2]);
						loops++;
					}
					else{
						packets[1] = mouse_byte;
						contador++;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}
		else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	return mouse_unsubscribe_int();
}

void print_packet(unsigned char * packets){
	printf("B1=0x%X\tB2=0x%X\tB3=0x%X\tLB=%d\tMB=%d\tRB=%d\tXOV=%d\tYOV=%d\tX=%d\tY=%d\n\n",
										packets[0], packets[1], packets[2],(packets[0] & BIT(0)),(packets[0] & BIT(2)) >> 2,(packets[0] & BIT(1)) >> 1, (packets[0] & BIT(6)) >> 6, (packets[0] & BIT(7)) >> 7, packets[1], packets[2]);
}

int test_async(unsigned short idle_time) {
	/* To be completed ... */
}

int test_config(void) {
	/* To be completed ... */
}

int test_gesture(short length, unsigned short tolerance) {
	/* To be completed ... */
}
