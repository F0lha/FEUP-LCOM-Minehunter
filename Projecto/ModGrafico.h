#ifndef __MODGRAFICO_H
#define __MODGRAFICO_H

#include "sprite.h"
#define HEIGHT_MINE_EXPERT 32
#define WIDTH_MINE_EXPERT 32
#define HEIGHT_NUMBER_EXPERT 32
#define WIDTH_NUMBER_EXPERT 32
#define HEIGHT_FLAG_EXPERT 32
#define WIDTH_FLAG_EXPERT 32
#define HEIGHT_WATER_EXPERT 32
#define WIDTH_WATER_EXPERT 32



int draw_mine(Sprite (*sp));

int draw_number (Sprite (*sp));

int draw_flag(int x, int y);

int draw_water (Sprite *sp);

int draw_table (int col, int lin);

int draw_string (char* str, double scale, unsigned long x, unsigned long y, unsigned long color);












#endif
