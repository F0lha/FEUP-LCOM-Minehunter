#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>
#include "ModRato.h"
#include "ModGrafico.h"



unsigned long mouse_byte;

int mouse_subscribe_int(void) {
	int value = MOUSE_HOOK_ID;
	sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &value);
	sys_irqenable(&value);
	return BIT(MOUSE_HOOK_ID);
}

int mouse_unsubscribe_int() {
	int value = MOUSE_HOOK_ID;
	return (sys_irqrmpolicy(&value) || sys_irqdisable(&value));
}

int mouse_int_handler() {


	unsigned long mouse_byte;
	if (sys_inb(OUT_BUF, &mouse_byte) != OK) {
		return 1;
	} else
		return mouse_byte;
}

Rato* getRato(){
	if(!rato){
		rato = newRato();
	}
	return rato;
}

Rato* newRato(){
	Rato* rato_temp;

	rato_temp = malloc(sizeof(Rato));

	rato_temp->x = 0;

	rato_temp->y = 0;

	rato_temp->leftButtonDown= 0;

	rato_temp->middleButtonDown= 0;

	rato_temp->rightButtonDown= 0;

	rato_temp->leftButtonReleased = 0;

	rato_temp->middleButtonReleased = 0;

	rato_temp->rightButtonReleased = 0;

	rato = rato_temp;

	return rato;
}

void drawRato(){
	int i,j;
	for(i = 0;i < 5;i++)
		for(j = 0; j < 5;j++){
			vg_set_pixel_final_buffer(rato->x+i,rato->y+j,4);
		}
}

void updateMouse(){
	rato->x += rato->packets[1];
	rato->y += rato->packets[2];
}

int first_byte(unsigned long mouse_byte) {
	return (BIT(3) & mouse_byte & BIT(3));
}
