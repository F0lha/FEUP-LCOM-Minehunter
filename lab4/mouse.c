#include "mouse.h"
#include "i8254.h"

int mouse_subscribe_int(void) {
	int value = MOUSE_HOOK_ID;
	sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &value);
	sys_irqenable(&value);
	return BIT(KBD_HOOK_ID);
}

int mouse_unsubscribe_int() {
	int value = MOUSE_HOOK_ID;
	return (sys_irqrmpolicy(&value)||sys_irqdisable(&value));
}
