#ifndef __MODTIMER_H
#define __MODTIMER_H


#define BIT(n) (0x01<<(n))
#define HOOK_ID				1
#define TIMER0_IRQ	        0

int global_counter;

int timer_subscribe_int(void );
int timer_unsubscribe_int();
void timer_int_handler();











#endif
