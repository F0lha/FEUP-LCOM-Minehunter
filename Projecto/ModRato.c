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
	sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &value);
	sys_irqenable(&value);
	return BIT(MOUSE_HOOK_ID);
}

int mouse_unsubscribe_int() {
	int value = MOUSE_HOOK_ID;
	return (sys_irqrmpolicy(&value) || sys_irqdisable(&value));
}

int enable_packets() {
	unsigned long ret;
	/*if (sys_outb(STATUS_PORT, ENABLE_MOUSE) != OK) { // manda a8 para 64
		return 1;
	}*/
	if (sys_outb(STATUS_PORT, KBC_CMD_MOUSE) != OK) {// manda d4 para 64
		return 1;
	}
	RESEND_TRY:
	if (sys_outb(OUT_BUF, ENABLE_PACKETS) != OK) {//manda f4 para 60
			return 1;
		}
	if (sys_inb(OUT_BUF, &ret) != OK) {
		return 1;
	}
	if (ret == ERROR) {
		enable_packets();
	}
	else if(ret == RESEND)
		goto RESEND_TRY;
}

int check_kbd_ibf(){
	unsigned long ret;
	sys_inb(STAT_REG,&ret);
	ret &= IBF;
	return (int) ret;
}


int mouse_int_handler() {


	long mouse_byte;
	int testing = 4;
	while(testing > 0){
		if(check_kbd_ibf())
		{
			printf("ta cheio \n");
			 tickdelay(micros_to_ticks(DELAY_US));
			continue;
		}
		testing--;
	}

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
	rato = malloc(sizeof(Rato));

	rato->x = 0;

	rato->y = 0;

	rato->leftButtonDown= 0;

	rato->middleButtonDown= 0;

	rato->rightButtonDown= 0;

	rato->leftButtonReleased = 0;

	rato->middleButtonReleased = 0;

	rato->rightButtonReleased = 0;

	return rato;
}

void drawRato(){
	int i,j;
	trocarRato_buffer();
	for(i = 0;i < 5;i++)
		for(j = 0; j < 5;j++){
			vg_set_pixel_bufferRato(rato->x+i,rato->y+j,4);
		}
}

void updateMouse(){
	if(!(rato->packets[0] & BIT(4)))
	{
		if((rato->x + (signed char)rato->packets[1] )< 0)
			rato->x = 0;
		else if(rato->x + (signed char)rato->packets[1] > 1024 - 5)
			rato->x = 1019;
		else rato->x +=  (signed char)rato->packets[1];
	}
	else{
		char temp =((-1 << 8) | rato->packets[1]);
		if((rato->x + temp ) < 0)
			rato->x = 0;
		else if( rato->x + temp > 1024 - 5)
			rato->x = 1019;
		else rato->x +=  temp;
	}
	if(!(rato->packets[1] & BIT(5)))
	{
		if((rato->y - (signed char)rato->packets[2] )< 0)
			rato->y = 0;
		else if( rato->y - (signed char)rato->packets[2] > 768 - 5)
			rato->y = 763;
		else rato->y -= (signed char)rato->packets[2];
	}
	else{
		char temp = ((-1 << 8) | rato->packets[2]);
		if((rato->y - temp ) < 0)
			rato->y = 0;
		else if( rato->y - temp > 768 - 5)
			rato->y = 763;
		else rato->y -= (signed char)temp;
	}
	printf("delta x : %d\ndelta y : %d\n",(signed char)rato->packets[1],rato->packets[2]);
}

int first_byte(unsigned long mouse_byte) {
	return (BIT(3) & mouse_byte & BIT(3));
}
