#include <minix/syslib.h>
#include <minix/drivers.h>
#include "keyboard.h"
#include "test5.h"
#include "timer.h"
#include "i8254.h"
#include "pixmap.h"
#include "sprite.h"



#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8


void *test_init(unsigned short mode, unsigned short delay) {
	/* To be completed */
	long video_mem = vg_init(mode);
	int counter = 0;
	int ipc_status, loops = 0;
	message msg;
	int irq_set = BIT(timer_subscribe_int());

	while (loops != delay) {
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

					if (counter == 60) {
						loops++;
						counter = 0;
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

	timer_unsubscribe_int();

	vg_exit();
	printf ("%d \n",video_mem);
}


int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	/* To be completed */

	unsigned int *ptr = vg_init(0x105);
	/*
	if(x+size>H_RES||y+size>V_RES) return -1;
	ptr += H_RES*2*size;
	ptr += 2*size;
	ptr -= 1;
	int i,diferenca=H_RES-(x+size);
	for(i=0;i<=x;i++){
		*ptr = color;
		ptr += 2;
	}
	ptr -= 2;
	for(i=0;i<(y-2);i++){
		ptr+=diferenca*2;
		ptr+=size*2;
		*ptr = color;
		ptr += x*2;
		*ptr = color;
	}
	ptr+=diferenca*2 + size*2;
	for(i=0;i<=x;i++){
		*ptr = color;
		ptr += 2;
	}
*/
	vg_fill(x,y, size ,size , color);
	//////////

	int breaker = 1;
	int two_bytes = 0;
	int ipc_status, r;
	message msg;

	int irq_set = kbd_subscribe_int();

	while(breaker) {

		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					kbd_int_handler();
					if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
					else if(scan_code>>7){
						if(two_bytes) ;
						else ;
						if (scan_code==BREAK_CODE_ESC)
							breaker = 0;
					}
					else
					{
						if (two_bytes){
							two_bytes=0;
						}
						else ;
					}
				}
				break;
			default:
				break; // no other notifications expected: do nothing
			}
		}
	}
	kbd_unsubscribe_int();

	//////////////
	vg_exit();
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, 
		unsigned short xf, unsigned short yf, unsigned long color) {

	//plotLine(x0,y0, x1,y1)

	unsigned int *ptr = vg_init(0x105);

	vg_set_line(xi, yi, xf, yf, color);

		//////////


		//////////

	  	int breaker = 1;
	  	int two_bytes = 0;
	  	int ipc_status, r;
	  	message msg;

	  	int irq_set = kbd_subscribe_int();

	  	while(breaker) {

	  		/* Get a request message. */
	  		if ( (r=driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
	  			printf("driver_receive failed with: %d", r);
	  			continue;
	  		}
	  		if (is_ipc_notify(ipc_status)) { // received notification
	  			switch (_ENDPOINT_P(msg.m_source)) {
	  			case HARDWARE: /* hardware interrupt notification */
	  				if (msg.NOTIFY_ARG & irq_set) {
	  					kbd_int_handler();
	  					if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
	  					else if(scan_code>>7){
	  						if(two_bytes) ;
	  						else ;
	  						if (scan_code==BREAK_CODE_ESC)
	  							breaker = 0;
	  					}
	  					else
	  					{
	  						if (two_bytes){
	  							two_bytes=0;
	  						}
	  						else ;
	  					}
	  				}
	  				break;
	  			default:
	  				break; // no other notifications expected: do nothing
	  			}
	  		}
	  	}
	  	kbd_unsubscribe_int();

	  	//////////////
	  	vg_exit();
	  	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	unsigned int *ptr = vg_init(0x105);
	int two_bytes = 0;
	int ipc_status, r;
	message msg;
	int breaker = 1;

	int irq_set = kbd_subscribe_int();


	// copy it to graphics memory

	draw_sprite(create_sprite(xpm, xi,yi));


	while(breaker) {

		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					kbd_int_handler();
					if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
					else if(scan_code>>7){
						if(two_bytes) ;
						else ;
						if (scan_code==BREAK_CODE_ESC)
							breaker = 0;
					}
					else
					{
						if (two_bytes){
							two_bytes=0;
						}
						else ;
					}
				}
				break;
			default:
				break; // no other notifications expected: do nothing
			}
		}
	}
	kbd_unsubscribe_int();

	//////////////
	vg_exit();


}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
		unsigned short hor, short delta, unsigned short time) {

	unsigned int *ptr = vg_init(0x105);
	int two_bytes = 0,loops = 0,global_counter = 0;
	int ipc_status, r;
	message msg;
	int breaker = 1;
	float pixels_per_sec = (float)delta / (float)time*60, x =0,y = 0,x_se = 0,y_se = 0, andado = 0;
	int irq_set_key = kbd_subscribe_int();
	int irq_set_timer =timer_subscribe_int();


	// copy it to graphics memory
	Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
	sp = create_sprite(xpm, xi,yi);
	draw_sprite(sp);


	while(breaker) {

		/* Get a request message. */
		if ( (r=driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set_timer) {
					global_counter++;
					if(hor)
					{
						x += pixels_per_sec;
						x_se += pixels_per_sec;
						if(x_se>1)
						{
							x_se -= 1;

							erase_sprite(sp);
							sp->x += x;
							andado += x;
							draw_sprite(sp);
							x=0;
						}
						/*
						if((sp->x + pixels_per_sec + sp->width) >= H_RES)
							breaker = 0;
							*/
					}
					else{
						y += pixels_per_sec;
						y_se += pixels_per_sec;
						if(y_se>1)
						{
							y_se -= 1;
							erase_sprite(sp);
							sp->y += y;
							andado +=y;
							draw_sprite(sp);
							y=0;
						}
						/*
						if((sp->y + pixels_per_sec + sp->height) >= V_RES)
							breaker = 0;
							*/
					}
					if (global_counter == 60) {
						loops++;
						global_counter = 0;
					}
					if(loops == time || andado > delta){
						breaker = 0;
					}
				}
				else if (msg.NOTIFY_ARG & irq_set_key) {
					kbd_int_handler();
					if(scan_code==TWO_BYTE_CODE){two_bytes = 1;}
					else if(scan_code>>7){
						if(two_bytes) ;
						else ;
						if (scan_code==BREAK_CODE_ESC)
							breaker = 0;
					}
					else
					{
						if (two_bytes){
							two_bytes=0;
						}
						else ;
					}
				}
				break;
			default:
				break; // no other notifications expected: do nothing
			}
		}
	}
	kbd_unsubscribe_int();
	timer_unsubscribe_int();

	//////////////
	vg_exit();
	printf("%d",loops);



}					


