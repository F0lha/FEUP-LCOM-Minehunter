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

Bitmap* Cron_0;
Bitmap* Cron_1;
Bitmap* Cron_2;
Bitmap* Cron_3;
Bitmap* Cron_4;
Bitmap* Cron_5;
Bitmap* Cron_6;
Bitmap* Cron_7;
Bitmap* Cron_8;
Bitmap* Cron_9;
Bitmap* Player1_table;
Bitmap* Player2_table;


Mine** create_table(int difficulty);

void create_interrupts(int *irq_set_timer,int *irq_set_keyboard,int *irq_set_mouse);

void stop_interrupts();

int right_click_screen(Mine*** table, int x, int y, int difficulty);

void click_vazio(Mine*** table, int k, int j, int difficulty,int *por_carregar,int single);

int click_screen(Mine*** table, int x, int y, int difficulty, int *filled,int *por_carregar,int single);

Mine** fill_table(Mine** table,int difficulty,int k, int j,int single,char *seed,int use); /// use é para usar a seed para criar

int post_game_state(int difficulty,int win,int time,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse);

int jogo_single_player(int difficulty,int irq_set_timer,int irq_set_keyboard,int irq_set_mouse);

void update_cronometro(int tempo);

void load_cronometro();

void delete_cronometro();

void update_multi_cronometro(int tempo, int jogador,int jogador1,int jogador2);

#endif
