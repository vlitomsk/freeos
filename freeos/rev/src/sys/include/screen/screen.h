#ifndef SCREEN_H
#define SCREEN_H

#include <config/video.h>
#include <vmm_manager/memory_manager.h>

char* video; // указатель на видеобуфер

struct tty{
    int tty_cursor;    //положение курсора
    int tty_attribute; //текущий атрибут символа
    char scr[2*VIDEO_HEIGHT*VIDEO_WIDTH + VIDEO_WIDTH*2];
};

typedef struct tty __SCREEN;

int numscr;
__SCREEN **screens;
int count;

void init_video(void);
int new_tty(void);
void del_tty(int);
void textcolor(unsigned int);
int gotoxy(unsigned int, unsigned int);
void clear(unsigned int);
void set_tty(int);
void put_char(char);
void put_str(const char *);
void put_int(int);
int put_array(int, int, int, int, char*[], int*[]);
void repaint(void);

#endif
