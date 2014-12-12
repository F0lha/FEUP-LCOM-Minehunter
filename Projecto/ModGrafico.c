#include "ModGrafico.h"



int draw_flag(int x, int y){
	int i, j, k, l;
	char *mapMine = read_xpm(retXPM("pic1"),&k,&l);
	for (j = y; j < y + l; j++) {
		for (i = x; i < x + k; i++) {
			vg_set_pixel(i, j, mapFlag[(j-y)*k + i-x]);
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

void vg_set_pixel(unsigned int x,unsigned int y, unsigned long color){
	char *ptr = video_mem;
	ptr += x;
	ptr += H_RES*y;
	*ptr = color;
}

void vg_fill(unsigned int x,unsigned int y, unsigned int width, unsigned int height,unsigned long color){
	int i,j;
	for(j = 0; j < height;j++){
		for(i = 0; i < width;i++)
		{
			vg_set_pixel(x+i,y+j,color);
		}
	}
}

void vg_set_line(unsigned short xi, unsigned short yi,
		unsigned short xf, unsigned short yf, unsigned long color){
	float dx=xf-xi;
	float dy=yf-yi;
	float d = 2*dy - dx;
	float y = yi;
	float x = xi;






	if(xi==xf){
		if(yi > yf){
			for (yf; yf < yi; yf++)
				vg_set_pixel(xi, yf,color);
		}else{
			for (yi; yi < yf; yi++)
				vg_set_pixel(xi, yi,color);
		}

	}
	else if(yi==yf){
		if(xi > xf){
					for (xf; xf < xi; xf++)
						vg_set_pixel(xf, yi,color);
				}else{
					for (xi; xi < xf; xi++)
						vg_set_pixel(xi, yi,color);
				}
	}
	else{
			if(xf < xi)
			{
				x = xf;
				xf = xi;
				xi = x;
			}


			dx=xf-xi;
			dy=yf-yi;
			////



		float erro;

		if(dy > 0)
			erro= 2*dy - dx;
		else erro= 2*dx + dy;
		vg_set_pixel(x,y,color);


		//


		vg_set_pixel(x, y, color);
		if(dy > 0){
			while((x <= xf) && (y <= yf)){

				vg_set_pixel(x,y,color);
				if(erro > 0)
				{
					y = y + 1;
					erro = erro - 2 * dx;
				}
				else
				{
					x = x + 1;
					erro = erro + 2 * dy;
				}
			}
		}
		else{
			while((x <= xf) && (y >= xi)){
				vg_set_pixel(x,y,color);
				if(erro > 0)
				{
					y = y - 1;
					erro = erro - 2 * dx;
				}
				else
				{
					x = x + 1;
					erro = erro - 2 * dy;
				}
			}
		}



	}
}

