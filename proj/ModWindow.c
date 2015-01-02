#include "ModWindow.h"
#include "ModGrafico.h"

int draw_square(int x, int y, Mine t){
	draw_flag(x, y);
	/*
	 *if (t->carregado == 0)
	 *	test_xpm(x,y,"square");
	 *else if (t->carregado == 1){
	 *		if (t->valor == -1){
	 *			test_xpm(x,y, "mine");
	 *			}
	 *		else if (t->valor == 0){
	 *			test_xpm(x,y,"water");
	 *			}
	 *		else test_xpm(x,y, t->valor);
	 *	}
	 *else if (t->carregado ==2){
	 *		test_xpm (x,y, "flag");
	 *}
	 *
	 *
	 */
	return 0;
}

int draw_Table(Mine** table, int difficulty){
	if (difficulty == 2){
		int i,j;
		for(i = 0;i < WIDTH_EXPERT*32;i++){
			for(j = 0; j < HEIGHT_EXPERT*32;j++){
				//draw_square(32+i*32, 156+j*32,table[i,j]);
			}
		}
		return 0;
	}
}

