#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include "i8254.h"
#include "keyboard.h"



int kbd_subscribe_int(void) {
	int value = KBD_HOOK_ID;
	sys_irqsetpolicy(IRQ1, IRQ_REENABLE | IRQ_EXCLUSIVE, &value);
	sys_irqenable(&value);
	return BIT(KBD_HOOK_ID);
}

int kbd_unsubscribe_int() {
	int value = KBD_HOOK_ID;
	return (sys_irqrmpolicy(&value)||sys_irqdisable(&value));
}

void kbd_int_handler(){
	sys_inb(OUT_BUF,&scan_code);
}
