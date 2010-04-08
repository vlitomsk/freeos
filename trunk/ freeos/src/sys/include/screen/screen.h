#ifndef SCREEN_H
#define SCREEN_H

#include <config/video.h>

//char* video= VIDEO_RAM; // указатель на видеобуфер

struct tty{
    int tty_cursor;    //положение курсора
    int tty_attribute; //текущий атрибут символа
};

typedef struct tty __SCREEN;

__SCREEN *defaultscreen;

void init_tty();
void textcolor(unsigned int c);
int gotoxy(unsigned int x, unsigned int y);
void clear(unsigned int c);
void setscreen(__SCREEN *s);
void putchar(char c);
void put_str(const char *s);
void put_int(int a);

#endif
