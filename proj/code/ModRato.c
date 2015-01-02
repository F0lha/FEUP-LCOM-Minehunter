#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/com.h>
#include <assert.h>
#include <minix/sysutil.h>
#include "ModRato.h"
#include "ModGrafico.h"
#include "bitmap.h"



unsigned long mouse_byte;
int value;
int mouse_subscribe_int(void) {
	value = MOUSE_HOOK_ID;
	sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &value);
	sys_irqenable(&value);
	return BIT(MOUSE_HOOK_ID);
}

int mouse_unsubscribe_int() {
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

	rato->x = 500;

	rato->y = 500;

	rato->leftButtonDown= 0;

	rato->rightButtonDown= 0;

	rato->leftButtonReleased = 0;

	rato->rightButtonReleased = 0;

	return rato;
}

void drawRato(int cor){
	if(!rato)
		newRato();
	int i,j;
	trocarRato_buffer();
	Bitmap* rato_bmp;
	if(cor == 0)
		rato_bmp = loadBitmap(PATH"Cursor.bmp");
	else if(cor == 1)
		rato_bmp= loadBitmap(PATH"Cursor_1.bmp");
	else rato_bmp= loadBitmap(PATH"Cursor_2.bmp");
	drawBitmap(rato_bmp,rato->x,rato->y,ALIGN_LEFT,bufferRato);
	deleteBitmap(rato_bmp);
}

void updateMouse(){
	if(!(rato->packets[0] & BIT(4)))
	{
		if((rato->x + (signed char)rato->packets[1] ) < 0)
			rato->x = 0;
		else if( (rato->x + (signed char)rato->packets[1]) > 1024)
			rato->x = 1024;
		else rato->x +=  (signed char)rato->packets[1];
	}
	else{
		int16_t   temp=((0xFF00) | rato->packets[1]);
		if((rato->x + (signed char)temp ) < 0)
			rato->x = 0;
		else if(( rato->x + (signed char)temp) > 1024)
			rato->x = 1024;
		else rato->x +=  (signed char)temp;
	}
	if(!(rato->packets[1] & BIT(5)))
	{
		if((rato->y - (signed char)rato->packets[2]  ) < 0)
			rato->y = 0;
		else if(( rato->y - (signed char)rato->packets[2])  > 768)
			rato->y = 768;
		else rato->y -= (signed char)rato->packets[2] ;
	}
	else{
		int16_t   temp = ((0xFF00) | rato->packets[2]);
		if((rato->y - (signed char)temp  ) < 0)
			rato->y = 0;
		else if(( rato->y - (signed char)temp)  > 768)
			rato->y = 768;
		else rato->y -= (signed char)temp ;
	}

}

int first_byte(unsigned long mouse_byte) {
	return (BIT(3) & mouse_byte & BIT(3));
}
