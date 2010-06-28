#ifndef SCREEN_H
#define SCREEN_H

#include<utils.h>

extern void cls();
extern void putch(unsigned char c);
extern void puts(unsigned char *str);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

#endif
