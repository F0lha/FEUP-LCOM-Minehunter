#ifndef __MODGRAFICO_H
#define __MODGRAFICO_H

#include <minix/syslib.h>
#include <minix/drivers.h>
//#include <sys/types.h>

#define HEIGHT_MINE_EXPERT 32
#define WIDTH_MINE_EXPERT 32
#define HEIGHT_NUMBER_EXPERT 32
#define WIDTH_NUMBER_EXPERT 32
#define HEIGHT_FLAG_EXPERT 32
#define WIDTH_FLAG_EXPERT 32
#define HEIGHT_WATER_EXPERT 32
#define WIDTH_WATER_EXPERT 32
#define HRES             1024
#define VRES		  768
#define VRAM_PHYS_ADDR	0xF0000000
#define videoMemSize HRES*VRES
static uint16_t* video_mem;
uint16_t *buffer;
uint16_t *bufferRato;
#define PATH "home/lcom/proj/code/res/images/"

#define BITS_PER_PIXEL	  16


static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

static char *phys_mem;

void update_screen(int cor,int turn,int relog);

int draw_flag(int x, int y);

int draw_table ();

int draw_string (char* str, double scale, unsigned long x, unsigned long y, unsigned long color);

int vg_exit(void);

void *vg_init(unsigned short mode);

char *read_xpm(char *map[], int *wd, int *ht);

void vg_set_pixel(unsigned int x,unsigned int y, unsigned long color);

char** retXPM(char *xpm);



void trocarRato_buffer();

void trocarVideo_Mem_Rato();

void vg_set_pixel_bufferRato(unsigned int x,unsigned int y, unsigned long color);

void vg_set_pixel_buffer(unsigned int x,unsigned int y, unsigned long color);

void darker_screen();

#endif
