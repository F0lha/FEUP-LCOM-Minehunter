#ifndef __JOGO_H
#define __JOGO_H

#include "bitmap.h"
#define HEIGHT_EXPERT 16
#define WIDTH_EXPERT 30
#define NUM_MINES_EXPERT 100

typedef struct{
	int carregado; ///0 se nao estiver, 1 se carregado,  2 se flag
	int valor; ///-1 se mina, 0 se vazia, outro valor
}Mine;


Mine** create_table(int difficulty);

void create_interrupts(int *irq_set_timer,int *irq_set_keyboard,int *irq_set_mouse);

void stop_interrupts();

int right_click_screen(Mine*** table, int x, int y, int difficulty);

void click_vazio(Mine*** table, int k, int j, int difficulty);

int click_screen(Mine*** table, int x, int y, int difficulty, int *filled);

Mine** fill_table(Mine** table,int difficulty,int k, int j);

#endif
