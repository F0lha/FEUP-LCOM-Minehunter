#include "Jogo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main() {
	draw_table(table, 2);
}

char* create_table(int difficulty){
	if(difficulty = 2) ///expert
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
		for(i = 0; i < NUM_MINES;i++)
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
