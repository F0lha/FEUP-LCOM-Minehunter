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

	rato->middleButtonDown= 0;

	rato->rightButtonDown= 0;

	rato->leftButtonReleased = 0;

	rato->middleButtonReleased = 0;

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
		rato_bmp = loadBitmap("home/lcom/Projecto/res/images/Cursor.bmp");
	else if(cor == 1)
		rato_bmp= loadBitmap("home/lcom/Projecto/res/images/Cursor_1.bmp");
	else rato_bmp= loadBitmap("home/lcom/Projecto/res/images/Cursor_2.bmp");
	drawBitmap(rato_bmp,rato->x,rato->y,ALIGN_LEFT,bufferRato);
	deleteBitmap(rato_bmp);
}

void updateMouse(){
	if(!(rato->packets[0] >> 4 & 0x01))
	{
		int16_t temp = 0;
		temp |= rato->packets[1];
		if((rato->x + (int)temp ) < 0)
			rato->x = 0;
		else if( rato->x + (int)temp > 1024)
			rato->x = 1024;
		else rato->x +=  (int)temp;
	}
	else{
		int16_t temp = 0;
		temp |= 0xFF00;
		temp |= rato->packets[1];
		if((rato->x + (int)temp ) < 0)
			rato->x = 0;
		else if( rato->x + (int)temp > 1024)
			rato->x = 1024;
		else rato->x +=  (int)temp;
	}
	if(!(rato->packets[0] >> 5 & 0x01))
	{
		int16_t temp = 0;
		temp |= rato->packets[2];
		if((rato->y - (int)temp ) < 0)
			rato->y = 0;
		else if( rato->y - (int)temp > 768)
			rato->y = 768;
		else rato->y -= (int)temp;
	}
	else{
		int16_t temp = 0;
		temp |= 0xFF00;
		temp |= rato->packets[2];
		if((rato->y - (int)temp ) < 0)
			rato->y = 0;
		else if( rato->y - (int)temp > 768)
			rato->y = 768;
		else rato->y -= (int)temp;
	}

}

int first_byte(unsigned long mouse_byte) {
	return (BIT(3) & mouse_byte & BIT(3));
}
