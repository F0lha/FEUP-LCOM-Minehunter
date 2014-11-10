#include "test4.h"
#include "mouse.h"
#include "i8254.h"
#include "timer.h"
#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>


typedef enum {INIT, DRAW, COMP} state_t;
typedef enum {LDOWN, LUP, MOVE} ev_type_t;

typedef struct event_t{unsigned xPos;unsigned yPos;ev_type_t type;} event_t;

unsigned x_total;
unsigned y_total;
unsigned tolerancia,distancia;


int test_packet(unsigned short cnt) {

	unsigned long mouse_byte;
	int ipc_status, loops = 0,contador = 0;
	message msg;
	unsigned char packets[3];
	short irq_set = BIT(mouse_subscribe_int());
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
					mouse_byte = mouse_int_handler();
					if (contador == 0) {
						if (!first_byte(mouse_byte)) {
							contador = 0;
							continue;
						}
					}
					packets[contador] = mouse_byte;
					if (contador == 2) {
						contador = 0;
						print_packet(packets);
						loops++;
						continue;
					}
					contador++;
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
											packets[0], packets[1], packets[2],(packets[0] & BIT(0)),(packets[0] & BIT(2)) >> 2,(packets[0] & BIT(1)) >> 1,
											(packets[0] & BIT(6)) >> 6, (packets[0] & BIT(7)) >> 7,
											(char)packets[1], (char)packets[2]);
}

int first_byte(unsigned long mouse_byte) {
	return (BIT(3) & mouse_byte & BIT(3));
}

int test_async(unsigned short idle_time) {

	unsigned long mouse_byte;
	int ipc_status, loops = 0,contador = 0;
	int global_counter = 0;
	message msg;
	unsigned char packets[3];
	short irq_set_mouse = BIT(mouse_subscribe_int());
	short irq_set_timer = BIT(timer_subscribe_int());
	enable_packets();
	while (loops != idle_time) {
		/* Get a request message. */
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {

			/* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					global_counter = 0;
					loops = 0;
					mouse_byte = mouse_int_handler();
					if (contador == 0) {
						if (!first_byte(mouse_byte)) {
							contador = 0;
							continue;
						}
					}
					packets[contador] = mouse_byte;
					if (contador == 2) {
						contador = 0;
						print_packet(packets);
						continue;
					}
					contador++;
				}
				else if (msg.NOTIFY_ARG & irq_set_timer){
					global_counter++;
					if (global_counter == 60) {
						loops++;
						global_counter = 0;
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
	printf("Fim do Tempo\n");
	return (mouse_unsubscribe_int()&&timer_unsubscribe_int());

}

int test_config(void) {
	unsigned char packets[3];
	int irq_set = BIT(get_mouse_status());
	int n = 0,ipc_status;
	unsigned long mouse_status_byte;
	message msg;

	while (n<3) {
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
						mouse_status_byte = mouse_int_handler();
						packets[n]=mouse_status_byte;
						n++;

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

	if((packets[0]&BIT(6) >> 6))
	printf("Remote Mode\n");
	else printf("Stream Mode\n");
	if ((packets[0] &BIT(5) >> 5))
	printf ("Enable Mode \n");
	else printf("Data reporting enabled \n");
	if ((packets[0] &BIT(4) >> 4))
	printf ("Scaling is 2:1 \n");
	else printf ("Scaling is 1:1 \n");
	if ((packets[0] & BIT(2) >> 2))
	printf ("Left button is pressed \n");
	else printf ("Left button is released \n");
	if ((packets[0] & BIT(1) >> 1))
	printf ("Middle button  is pressed \n");
	else printf ("Middle button is released \n");
	if ((packets[0] & BIT(0) >> 0))
	printf ("Right button is pressed \n");
	else printf("Right button is released \n");
	if ((packets[1] & (BIT (0) | BIT (1)) == 0))
	printf ("Resolution = 1");
	else if ((packets[1] & (BIT (0) | BIT (1)) == 1))
	printf ("Resolution = 2");
	else if ((packets[1] & (BIT (0) | BIT (1)) == 2))
	printf ("Resolution = 4");
	else if ((packets[1] & (BIT (0) | BIT (1)) == 3))
	printf ("Resolution = 8");
	printf("Sample rate = %d\n",packets[2]);
	return mouse_unsubscribe_int();
}

int test_gesture(short length, unsigned short tolerance) {


	unsigned long mouse_byte;
	int ipc_status, loops = 0,contador = 0, tolerancia = tolerance,distancia = length;
	message msg;
	x_total = 0;
	y_total = 0;
	int stop = 1,andar = 0;
	unsigned char packets[3];
	struct event_t evt;
	short irq_set = BIT(mouse_subscribe_int());
	enable_packets();
	while (stop) {
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
					mouse_byte = mouse_int_handler();
					if (contador == 0) {
						if (!first_byte(mouse_byte)) {
							contador = 0;
							continue;
						}
					}
					packets[contador] = mouse_byte;
					if (contador == 2) {
						contador = 0;
						print_packet(packets);

						if (packets[2] == 0 && packets[1] == 0 ) {
							if (packets[0] & BIT(0)) {
								x_total = 0;
								y_total = 0;
								andar = 1;
							}
							else{
								andar = 0;
							}
						}
						else if (andar == 1)
						{
							int novo_total_x = x_total + packets[1];
							int novo_total_y = y_total + packets[2];
							if(abs(x_total) > abs(novo_total_x)){
								x_total = 0;
								y_total = 0;
								continue;
							}
							else if (abs(novo_total_y) > tolerancia) {
								x_total = 0;
								y_total = 0;
								continue;
							}
							else{
							x_total = novo_total_x;
							y_total = novo_total_y;
							}
							if((abs(x_total) > length) &&(abs(y_total) < tolerance)){
								stop = 0;
								continue;
							}
						}



						/*evt.xPos = packets[1];
						evt.yPos = packets[2];
						if (packets[2] == 0 && packets[1] == 0 ) {
							if (packets[0] & BIT(0)) {
								evt.type = LDOWN;
							}
							else{
								evt.type = LUP;
							}
						}
						else{
							evt.type =  MOVE;
						}
						check_hor_line(&evt);
						if(abs(x_total) > length && abs(y_total) < tolerance){
							stop = 0;
							continue;
						}*/
						continue;
					}
					contador++;
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


void check_hor_line(event_t* evt) {
	static state_t st = INIT; // initial state; keep state
	switch (st) {
	case INIT:
		printf("INICIO");
		x_total = 0;
		y_total = 0;
		if( evt->type == LDOWN ){
			st = DRAW;
		}
		break;
	case DRAW:
		if( evt->type == MOVE ) {
			printf("cenas\n");
			int novo_total_x = x_total + evt->xPos;
			int novo_total_y = y_total + evt->yPos;
			if(abs(x_total) > abs(novo_total_x))
				st = INIT;
			else if (abs(novo_total_y) > tolerancia) {
				st = INIT;
			}
			x_total = novo_total_x;
			y_total = novo_total_y;
			if((abs(x_total) > distancia) &&(abs(y_total) < tolerancia) )
				st = COMP;
		} else if( evt->type == LUP )
			st = INIT;
		break;
	default:
		break;
	}
}











