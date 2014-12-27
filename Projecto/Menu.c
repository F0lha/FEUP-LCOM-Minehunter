#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ModGrafico.h"
#include "Jogo.h"
#include "ModRato.h"
#include "ModTeclado.h"
#include "ModTimer.h"
#include "bitmap.h"
#include "Menu.h"



void draw_main_menu(){
	Bitmap* fundo;
	Bitmap* back_button;
	Bitmap* opcoes;
	fundo = loadBitmap("home/lcom/Projecto/res/images/Fundo_Main.bmp");
	back_button = loadBitmap("home/lcom/Projecto/res/images/Back_Button.bmp");
	drawBitmap(fundo,0,0,ALIGN_LEFT,buffer);
	drawBitmap(back_button,0,0,ALIGN_LEFT,buffer);
	opcoes = loadBitmap("home/lcom/Projecto/res/images/Main_menu.bmp");
	drawBitmap(opcoes,0,0,ALIGN_LEFT,buffer);
	deleteBitmap(fundo);
	deleteBitmap(opcoes);
	deleteBitmap(back_button);
}

void draw_multi_menu(){
	Bitmap* fundo;
	Bitmap* back_button;
	fundo = loadBitmap("home/lcom/Projecto/res/images/Fundo_menu_multi.bmp");
	back_button = loadBitmap("home/lcom/Projecto/res/images/Back_Button.bmp");
	drawBitmap(fundo,0,0,ALIGN_LEFT,buffer);
	drawBitmap(back_button,0,0,ALIGN_LEFT,buffer);
	deleteBitmap(fundo);
	deleteBitmap(back_button);
}



void multi_menu(int irq_set_timer,int irq_set_keyboard,int irq_set_mouse){

	int ipc_status;
	message msg;
	draw_multi_menu();
	int contador = 0,breaker = 1,two_bytes = 0, mouse_byte;
	while (breaker) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		//printf("msg.notifyArg: 0x%X\n", msg.NOTIFY_ARG);
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer) {
					timer_int_handler();
					if(global_counter % 1 == 0)
					{
						update_screen(0);
					}

					if (global_counter == 60) {
						global_counter = 0;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard)
				{
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
				if (msg.NOTIFY_ARG & irq_set_mouse)
				{

					rato = getRato();
					mouse_byte = mouse_int_handler();
					if (contador == 0) {
						if (!first_byte(mouse_byte)) {
							contador = 0;
							continue;
						}
					}
					rato->packets[contador] = mouse_byte;
					if (contador == 2) {
						contador = 0;
						{
							updateMouse();

							if(rato->packets[0]&BIT(0) && rato->leftButtonDown == 0)
							{
								rato->leftButtonDown = 1;
								rato->leftButtonReleased = 0;
								if(rato->x >=0 && rato->x < 50 && rato->y >=0 && rato->y < 50)
									breaker = 0; /// escape button
								else if(rato->x >=310 && rato->x < 712 && rato->y >=110 && rato->y < 212){
									jogo_multi_player(2,irq_set_timer,irq_set_keyboard, irq_set_mouse);
									draw_multi_menu();
								}
								else if(rato->x >=340 && rato->x < 678 && rato->y >=330 && rato->y < 432)
								{
									jogo_multi_player_porta(2,irq_set_timer,irq_set_keyboard, irq_set_mouse, 1);
									draw_multi_menu();
								}
								else if(rato->x >=385 && rato->x < 620 && rato->y >=544 && rato->y < 642)
								{
									jogo_multi_player_porta(2,irq_set_timer,irq_set_keyboard, irq_set_mouse, 0);
									draw_multi_menu();
								}


							}
							else if(!(rato->packets[0]&BIT(0)))
							{
								rato->leftButtonDown = 0;
								rato->leftButtonReleased = 1;
							}
							if(rato->packets[0]&BIT(1) && rato->rightButtonDown == 0)
							{
								rato->rightButtonDown = 1;
								rato->rightButtonReleased = 0;
							}
							else if(!(rato->packets[0]&BIT(1)))
							{
								rato->rightButtonDown = 0;
								rato->rightButtonReleased = 1;
							}

						}
						continue;
					}
					contador++;

				}
			}
		}
	}
}




int main(int argc, char **argv){

	load_cronometro();
	sef_startup();
	buffer = malloc(videoMemSize * BITS_PER_PIXEL	/8);
	bufferRato =malloc(videoMemSize * BITS_PER_PIXEL	/8);
	int irq_set_timer,irq_set_keyboard, irq_set_mouse;
	int ipc_status;
	message msg;
	global_counter = 0;
	create_interrupts(&irq_set_timer,&irq_set_keyboard,&irq_set_mouse);
	int contador = 0,breaker = 1,two_bytes = 0, mouse_byte;
	video_mem = vg_init(0x117);
	draw_main_menu();
	while (breaker) {
			if (driver_receive(ANY, &msg, &ipc_status) != 0) {
				printf("driver_receive failed with: %d");
				continue;
			}
			//printf("msg.notifyArg: 0x%X\n", msg.NOTIFY_ARG);
			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & irq_set_timer) {
						timer_int_handler();
						if(global_counter % 1 == 0)
						{
							update_screen(0);
						}

						if (global_counter == 60) {
							global_counter = 0;
						}
					}
					if (msg.NOTIFY_ARG & irq_set_keyboard)
					{
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
					if (msg.NOTIFY_ARG & irq_set_mouse)
					{

						rato = getRato();
						mouse_byte = mouse_int_handler();
						if (contador == 0) {
							if (!first_byte(mouse_byte)) {
								contador = 0;
								continue;
							}
						}
						rato->packets[contador] = mouse_byte;
						if (contador == 2) {
							contador = 0;
							{
								updateMouse();

								if(rato->packets[0]&BIT(0) && rato->leftButtonDown == 0)
								{
									rato->leftButtonDown = 1;
									rato->leftButtonReleased = 0;
									if(rato->x >=0 && rato->x < 50 && rato->y >=0 && rato->y < 50)
										breaker = 0;
									else if(rato->x >=270 && rato->x < 750 && rato->y >=342 && rato->y < 405){
										jogo_single_player(2,irq_set_timer,irq_set_keyboard, irq_set_mouse);
										draw_main_menu();
									}
									else if(rato->x >=285 && rato->x < 732 && rato->y >=485 && rato->y < 552)
									{
										multi_menu(irq_set_timer,irq_set_keyboard, irq_set_mouse);
										draw_main_menu();
									}


								}
								else if(!(rato->packets[0]&BIT(0)))
								{
									rato->leftButtonDown = 0;
									rato->leftButtonReleased = 1;
								}
								if(rato->packets[0]&BIT(1) && rato->rightButtonDown == 0)
								{
									rato->rightButtonDown = 1;
									rato->rightButtonReleased = 0;
								}
								else if(!(rato->packets[0]&BIT(1)))
								{
									rato->rightButtonDown = 0;
									rato->rightButtonReleased = 1;
								}

							}
							continue;
						}
						contador++;

					}
				}
			}
	}

/// interrupts
	vg_exit();
	delete_cronometro();
	stop_interrupts();
	free(rato);
	free(buffer);
	free(bufferRato);
	printf("acabou\n");
}



