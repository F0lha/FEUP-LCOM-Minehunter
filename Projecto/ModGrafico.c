#include "ModGrafico.h"



int draw_flag(int x, int y){
	int i, j, k, l;
	char *mapMine = read_xpm(retXPM("pic1"),&k,&l);
	for (j = y; j < y + l; j++) {
		for (i = x; i < x + k; i++) {
			vg_set_pixel(i, j, mapMine[(j-y)*k + i-x]);
		}
	}
}


int draw_number (Sprite *sp){
	int i, j;
	for (j = sp->y; j < sp->y + sp->height; j++) {
		for (i = sp->x; i < sp->x + sp->width; i++) {
			vg_set_pixel(i, j, sp->map[(j-sp->y)*sp->width + i-sp->x]);
		}
	}
}

int draw_mine(Sprite *sp){
	int i, j;
	for (j = sp->y; j < sp->y + sp->height; j++) {
		for (i = sp->x; i < sp->x + sp->width; i++) {
			vg_set_pixel(i, j, sp->map[(j-sp->y)*sp->width + i-sp->x]);
		}
	}
}


int draw_water (Sprite *sp){

}


int draw_table (int col, int lin){

}


int draw_string (char* str, double scale, unsigned long x, unsigned long y, unsigned long color){

}

