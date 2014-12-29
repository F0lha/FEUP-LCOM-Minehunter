#ifndef __JOGO_H
#define __JOGO_H

#include "bitmap.h"

Bitmap* Zero;
Bitmap* One;
Bitmap* Two;
Bitmap* Three;
Bitmap* Four;
Bitmap* Five;
Bitmap* Six;
Bitmap* Seven;
Bitmap* Eight;
Bitmap* Nine;
Bitmap* Points;

void draw_main_menu();

void multi_menu(int irq_set_timer,int irq_set_keyboard,int irq_set_mouse);

void draw_multi_menu();

void delete_relogio();

void load_relogio();

#endif
