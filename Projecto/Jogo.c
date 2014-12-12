#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Jogo.h"
#include "ModRato.h"
#include "ModTeclado.h"
#include "ModTimer.h"
#include "ModGrafico.h"




int main(int argc, char **argv) {
	sef_startup();

	int irq_set_timer,irq_set_keyboard, irq_set_mouse;
	int contador = 0,breaker = 1,two_bytes = 0,mouse_byte;
	int ipc_status, loops = 0;
	message msg;
	create_interrupts(&irq_set_timer,&irq_set_keyboard,&irq_set_mouse);

	vg_init(0x105);

	while (breaker) {
		if (driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d");
			continue;
		}
		if (is_ipc_notify(ipc_status)) {

			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_timer) {
					timer_int_handler();
					if(global_counter % 3 == 0)
						trocarBuffer();
					if (global_counter == 60) {
						loops++;
						global_counter = 0;
					}
				}
				if (msg.NOTIFY_ARG & irq_set_keyboard)
				{
					kbd_int_handler();
					if(scan_code==TWO_BYTE_CODE){
						two_bytes = 1;
					}
					else if(scan_code>>7){
						if(two_bytes) ;
						else ;
						if (scan_code==BREAK_CODE_ESC)
							breaker = 0;
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
							///fazer aqui merda
							updateMouse();
							buffer_final = buffer_secondary;
							drawRato();
						}
						continue;
					}
					contador++;

				}
				break;
			}
		}
		trocarBuffer();
	}
	vg_exit();
	stop_interrupts();
}

Mine** create_table(int difficulty){
	if(difficulty == 2) ///expert
	{
		int i;
		Mine** table;
		table = malloc(WIDTH_EXPERT *sizeof(Mine *));
		for (i = 0; i < WIDTH_EXPERT; i++)
			table[i] = malloc( HEIGHT_EXPERT *sizeof(Mine));
		int j;
		for(i = 0;i < WIDTH_EXPERT;i++)
			for(j = 0; j < HEIGHT_EXPERT;j++)
			{
				table[i,j]->valor = 0;
				table[i,j]->carregado = 0;
			}
		time_t t;
		srand((unsigned) time(&t));
		for(i = 0; i < NUM_MINES_EXPERT;i++)
		{
			int j = rand() % HEIGHT_EXPERT;
			int k = rand() % WIDTH_EXPERT ;
			if(table[k,j]->valor == -1)
			{}
			else if(table[k,j]->valor == 8)
			{}
			else{
				table[k,j]->valor = -1;
				if(k != 0) // parede lateral esquerda
					table[k-1,j]->valor += 1;
				if(k != WIDTH_EXPERT - 1) /// parede lateral direita
					table[k+1,j]->valor += 1;
				if(j != 0) /// parede vertical de cima
					table[k,j - 1]->valor += 1;
				if(j != HEIGHT_EXPERT - 1)///parede vertical de baixo
					table[k,j + 1]->valor += 1;
				if(k != 0 && j != 0) ///canto superior esquerdo
					table[k - 1,j - 1]->valor += 1;
				if(k != WIDTH_EXPERT - 1 && j != HEIGHT_EXPERT - 1)  /// canto inferior direito
					table[k + 1,j + 1]->valor += 1;
				if(k != WIDTH_EXPERT - 1 && j != 0)  /// canto superior direito
						table[k + 1,j - 1]->valor += 1;
				if(k != 0 && j != HEIGHT_EXPERT - 1)/// canto inferior esquerdo
					table[k - 1,j + 1]->valor += 1;
			}
		}
		return table;
	}
}


void create_interrupts(int *irq_set_timer,int *irq_set_keyboard,int *irq_set_mouse){
	*irq_set_timer = kbd_subscribe_int();

	*irq_set_keyboard = timer_subscribe_int();
	*irq_set_mouse = mouse_subscribe_int();
}

void stop_interrupts(){
	mouse_unsubscribe_int();
	kbd_unsubscribe_int();
	timer_unsubscribe_int();
}
