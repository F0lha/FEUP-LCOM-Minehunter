#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>
#include "ModTeclado.h"


#define PAR_ERR 	BIT(7)
#define TO_ERR 		BIT(6)
#define IBF 		BIT(1)
#define OBF 		BIT(0)
#define RESEND 		0xFE
#define ERROR 		0xFC



unsigned long data;
int value;

int kbd_subscribe_int(void) { //
	value = KBD_HOOK_ID;
	sys_irqsetpolicy(IRQ1, IRQ_REENABLE | IRQ_EXCLUSIVE, &value);
	sys_irqenable(&value);
	return BIT(KBD_HOOK_ID);
}

int kbd_unsubscribe_int() {

	unsigned long stat;
	return (sys_irqrmpolicy(&value)||sys_irqdisable(&value));
}

void kbd_int_handler(){ //function that handles the interrupts in C
	sys_inb(IN_BUF,&scan_code);
}
