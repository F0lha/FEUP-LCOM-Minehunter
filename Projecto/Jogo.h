#ifndef __JOGO_H
#define __JOGO_H


#define HEIGHT_EXPERT 16
#define WIDTH_EXPERT 30
#define NUM_MINES 100

typedef struct{
	int carregado; ///0 se nao estiver, 1 se carregado,  2 se flag
	int valor; ///-1 se mina, 0 se vazia, outro valor
}Mine;


char* create_table(int difficulty);













#endif
