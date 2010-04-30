#include <screen/screen.h>
/*#include <config/video.h>*/

static char* video= VIDEO_RAM;

//Инициализация tty
void init_tty() {
    defaultscreen->tty_cursor = 0;
    defaultscreen->tty_attribute = 7;
}

//Смена текущего атрибута символа
void textcolor(unsigned int c) {
    defaultscreen->tty_attribute = c;
}

// like pascal :)
int gotoxy(unsigned int x, unsigned int y) {
	if (x*y > VIDEO_HEIGHT*VIDEO_WIDTH) return 1;
	if ((x >= VIDEO_WIDTH) || (y >= VIDEO_HEIGHT)) return 1;
	defaultscreen->tty_cursor = y * VIDEO_WIDTH - VIDEO_WIDTH + x - 1;
	return 0;	
}

//Очистка экрана
void clear(unsigned int c) {
	int i;
	int a=0;
	for (i ^= i; i < VIDEO_HEIGHT*VIDEO_WIDTH; ++i)
	{
		*(video + i*2) = ' ';    
		*(video + 2*(a++)+1) = c ;
	}
    defaultscreen->tty_cursor ^= defaultscreen->tty_cursor;
}

//Установка текущего экрана
void setscreen(__SCREEN *s){
    defaultscreen = s;
}

/*Бегущее вперед время уносило наши мечты
Оставляя мириады крошечных существ,
пытающихся привязать нас к земле,
К жизни, тронутой медленным гниением. */
