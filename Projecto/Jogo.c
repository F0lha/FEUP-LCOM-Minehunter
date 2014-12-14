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
	global_counter = 0;
	buffer = malloc(videoMemSize * BITS_PER_PIXEL	/8);
	bufferRato =malloc(videoMemSize * BITS_PER_PIXEL	/8);
	int irq_set_timer,irq_set_keyboard, irq_set_mouse;
	int contador = 0,breaker = 1,two_bytes = 0, mouse_byte;
	int ipc_status, loops = 0, teste = 0;
	message msg;
	create_interrupts(&irq_set_timer,&irq_set_keyboard,&irq_set_mouse);
	video_mem = vg_init(0x105);
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
						update_screen();
					}
					if(loops % 5 == 0)
					{
						int i,j;
						for(j = 0; j < VRES;j++){
							for(i = 0; i < HRES;i++)
							{
								vg_set_pixel_buffer(i,j,loops % 63 + 1);
							}
						}
					}
					if (global_counter == 60) {
						loops++;
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
					printf("adsad\n");
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
							teste = 1;
						}
						continue;
					}
					contador++;

				}
				break;
			}
		}

	}
	printf("sai\n");
	vg_exit();
	printf("vgexit\n");
	stop_interrupts();
	printf("stopinterrupts\n");
	free(rato);
	printf("free1\n");

	free(buffer);
	printf("free2\n");
	free(bufferRato);
	printf("free3\n");
	int i;
	int j;
	Mine** table = create_table(2);


		for(j = 0; j < HEIGHT_EXPERT;j++)
		{
			for(i = 0;i < WIDTH_EXPERT;i++){
			printf("%d ",table[i][j].valor);
		}
			printf("\n");
	}

	free(table);

}

void update_screen(){
	drawRato();
	trocarVideo_Mem_Rato();
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
				table[i][j].valor = 0;
				table[i][j].carregado = 0;
			}
		time_t t;
		srand((unsigned) time(&t));
		for(i = 0; i < NUM_MINES_EXPERT;i++)
		{

			int j = rand() % HEIGHT_EXPERT;
			int k = rand() % WIDTH_EXPERT;
			if(table[k][j].valor == -1)
			{i--;}
			else if(table[k][j].valor >= 8)
			{i--;}
			else{
				printf("x = %d // y = %d // valor-> %d\n",k,j, table[k][j].valor);
				table[k][j].valor = -1;
				if(k != 0) // parede lateral esquerda
					if(table[k-1][j].valor != -1)
						table[k-1][j].valor += 1;
				if(k != WIDTH_EXPERT - 1) /// parede lateral direita
					if(table[k+1][j].valor != -1)
						table[k+1][j].valor += 1;
				if(j != 0) /// parede vertical de cima
					if(table[k][j - 1].valor != -1)
						table[k][j - 1].valor += 1;
				if(j != HEIGHT_EXPERT - 1)///parede vertical de baixo
					if(table[k][j+1].valor != -1)
						table[k][j + 1].valor += 1;
				if(k != 0 && j != 0) ///canto superior esquerdo
					if(table[k - 1][j - 1].valor != -1)
						table[k - 1][j - 1].valor += 1;
				if(k != WIDTH_EXPERT - 1 && j != HEIGHT_EXPERT - 1)  /// canto inferior direito
					if(table[k + 1][j + 1].valor != -1)
						table[k + 1][j + 1].valor += 1;
				if(k != WIDTH_EXPERT - 1 && j != 0)  /// canto superior direito
					if(table[k + 1][ - 1].valor != -1)
						table[k + 1][j - 1].valor += 1;
				if(k != 0 && j != HEIGHT_EXPERT - 1)/// canto inferior esquerdo
					if(table[k - 1][j + 1].valor != -1)
						table[k - 1][j + 1].valor += 1;
			}
		}
		return table;
	}
}


void create_interrupts(int *irq_set_timer,int *irq_set_keyboard,int *irq_set_mouse){
	*irq_set_timer = timer_subscribe_int();
	*irq_set_keyboard = kbd_subscribe_int();
	*irq_set_mouse = mouse_subscribe_int();
	enable_packets();
}

void stop_interrupts(){
	mouse_unsubscribe_int();
	kbd_unsubscribe_int();
	timer_unsubscribe_int();
}
